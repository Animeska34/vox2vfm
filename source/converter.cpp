// Copyright 2022 Jevgenij Christoforov, Nikita Fediuchin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "vox2vfm/converter.hpp"

#include <fstream>
#include <cstring>

#define OGT_VOX_IMPLEMENTATION
#include "ogt_vox.h"

extern "C"
{
#include "voxfield/core/model.h"
}

namespace vox2vfm
{
	Converter:: Converter() : buffer() {}
	Converter::Converter(size_t bufferCapacity) :
		buffer(bufferCapacity) {}

	inline static const ogt_vox_scene* readVoxFile(
		const string& filePath,
		vector<uint8_t>& buffer)
	{
		ifstream fileStream(filePath, ios::in | ios::binary | ios::ate);

		if (!fileStream.is_open())
			throw Vox2vfmException("Failed to open file");

		streamsize fileSize = fileStream.tellg();
		fileStream.seekg(0, ios::beg);
		buffer.resize(fileSize);
		fileStream.read((char*)buffer.data(), fileSize);
		fileStream.close();
		return ogt_vox_read_scene(buffer.data(), fileSize);
	}
	inline static vector<uint8_t> createVoxels(
		const uint8_t* voxVoxels,
		uint16_t sizeX, uint16_t sizeY, uint16_t sizeZ,
		size_t voxelSize)
	{
		assert(voxVoxels);
		assert(sizeX > 0);
		assert(sizeY > 0);
		assert(sizeZ > 0);
		assert(voxelSize > 0);

		vector<uint8_t> vfmVoxels(voxelSize);
		size_t voxSizeXY = sizeX * sizeY;
		size_t vfmSizeXY = sizeX * sizeZ;

		for (uint16_t z = 0; z < sizeZ; z++)
		{
			for (uint16_t y = 0; y < sizeY; y++)
			{
				for (uint16_t x = 0; x < sizeX; x++)
				{
					size_t voxIndex = x + y * sizeX + z * voxSizeXY;
					size_t vfmIndex = x + z * sizeX + y * vfmSizeXY;
					assert(voxIndex < voxelSize);
					assert(vfmIndex < voxelSize);
					vfmVoxels[vfmIndex] = voxVoxels[voxIndex];
				}
			}
		}

		return vfmVoxels;
	}
	inline static vector<VoxelData> createVoxelData(
		const ogt_vox_scene* voxScene,
		vector<uint8_t>& vfmVoxels)
	{
		assert(voxScene);

		auto colors = voxScene->palette.color;
		vector<VoxelData> voxelDataArray;

		for (uint8_t& voxel : vfmVoxels)
		{
			const ogt_vox_rgba color = colors[voxel];

			if (color.a == 0)
			{
				voxel = 0;
				continue;
			}

			bool isFound = false;

			for (size_t j = 0; j < voxelDataArray.size(); j++)
			{
				VoxelData data = voxelDataArray[j];

				if (color.r == data.color.r &&
					color.g == data.color.g &&
					color.b == data.color.b &&
					color.a == data.color.a)
				{
					voxel = (uint8_t)(j + 1);
					isFound = true;
					break;
				}
			}

			if (!isFound)
			{
				// TODO: encode materials
				VoxelData data = voxelData(srgbColor(
					color.r, color.g, color.b, color.a));
				voxelDataArray.push_back(data);
				voxel = (uint8_t)voxelDataArray.size();
			}
		}

		return voxelDataArray;
	}
	void Converter::convert(
		const string& inputFilePath,
		const string& outputFilePath,
		const string& name,
		const string& description,
		const string& creator,
		float scale,
		float iconX,
		float iconY,
		float iconZ,
		int index)
	{
		auto voxScene = readVoxFile(inputFilePath, buffer);

		if (!voxScene || voxScene->num_models == 0)
			throw Vox2vfmException("Failed to load MagicaVoxel file");

		if (voxScene->num_models > 1)
		{
			if (index < 0)
				throw Vox2vfmException("Scene have more than one model, use \"-i\" option");
			else if (index >= voxScene->num_models)
				throw Vox2vfmException("Model index is out of array bound");
		}
		else
		{
			index = 0;
		}

		auto voxModel = voxScene->models[index];
		auto sizeX = (uint16_t)voxModel->size_x;
		auto sizeY = (uint16_t)voxModel->size_y;
		auto sizeZ = (uint16_t)voxModel->size_z;
		size_t voxelSize = sizeX * sizeY * sizeZ;

		if (voxelSize == 0)
			throw Vox2vfmException("Zero model volume, probably file is damaged");
		if (voxelSize > MAX_MODEL_VOXEL_SIZE)
			throw Vox2vfmException("Model volume is too big, use \"--shrink\" option");

		auto voxels = createVoxels(
			voxModel->voxel_data,
			sizeX, sizeY, sizeZ, voxelSize);
		auto voxelData = createVoxelData(
			voxScene, voxels);

		if (!isModelDataValid8(
			voxelData.data(),
			voxelData.size(),
			voxels.data(),
			sizeX,
			sizeZ,
			sizeY,
			name.c_str(),
			name.length(),
			description.c_str(),
			description.length(),
			creator.c_str(),
			creator.length(),
			scale,
			iconX,
			iconY,
			iconZ))
		{
			throw Vox2vfmException("Invalid converted model data");
		}

		Model vfmModel = createModel8(
			voxelData.data(),
			voxelData.size(),
			voxels.data(),
			sizeX,
			sizeZ,
			sizeY,
			name.c_str(),
			name.length(),
			description.c_str(),
			description.length(),
			creator.c_str(),
			creator.length(),
			scale,
			iconX,
			iconY,
			iconZ,
			false);

		if (!vfmModel)
			throw bad_alloc();

		VoxfieldResult voxfieldResult = writeModelToFile(
			vfmModel, outputFilePath.c_str());

		if (voxfieldResult != SUCCESS_VOXFIELD_RESULT)
		{
			throw Vox2vfmException("Failed to write model to file (error: " +
				string(voxfieldResultToString(voxfieldResult)) + ")");
		}
	}
}
