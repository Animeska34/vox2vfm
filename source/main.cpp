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

#include "vox2vfm/defines.h"
#include "vox2vfm/converter.hpp"

#include "cmmt/angle.h"
#include <iostream>

using namespace std;
using namespace vox2vfm;

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		cout << "Usage: vox2vfm <path-to-input-file> [options]\n" <<
			"    [-o <file-path>]: Path to the output file.\n" <<
			"    [-n <name>]: Name of the model.\n" <<
			"    [-d <description>]: Description of the model.\n" <<
			"    [-c <creator>]: Creator of the model.\n" <<
			"    [-s <scale>]: Scale multiplier of the model.\n" <<
			"    [-i <index>]: Index of the model in the scene if more than one.\n" <<
			"    [-ix <iconX>]: Icon rotation along X-axis in degrees.\n" <<
			"    [-iy <iconY>]: Icon rotation along Y-axis in degrees.\n" <<
			"    [-iz <iconZ>]: Icon rotation along Z-axis in degrees.\n" <<
			"    [--shrink]: Resize model to fit it into the maximum volume.\n" <<
			"";
		return EXIT_SUCCESS;
	}

	// TODO: read --shrink

	string inputFilePath = string(argv[1]);
	string outputFilePath;
	string name = "Unnamed";
	string description =
		"Converted using vox2vfm. (v"
		VOX2VFM_VERSION_STRING ")";
	string creator = "vox2vfm";
	float scale = 1.0f;
	float iconX = degToRad((cmmt_float_t)45.0);
	float iconY = degToRad((cmmt_float_t)30.0);
	float iconZ = 0.0f;
	int index = -1;

	for (int i = 2; i < argc; i++)
	{
		auto option = string(argv[i]);

		if (option == "-o")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			outputFilePath = string(argv[++i]);
		}
		else if (option == "-n")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			name = string(argv[++i]);
		}
		else if (option == "-d")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			description = string(argv[++i]);
		}
		else if (option == "-c")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			creator = string(argv[++i]);
		}
		else if (option == "-s")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			scale = stof(string(argv[++i]));
		}
		else if (option == "-ix")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			iconX = degToRad((cmmt_float_t)stof(string(argv[++i])));
		}
		else if (option == "-iy")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			iconY = degToRad((cmmt_float_t)stof(string(argv[++i])));
		}
		else if (option == "-iz")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			iconZ = degToRad((cmmt_float_t)stof(string(argv[++i])));
		}
		else if (option == "-i")
		{
			if (i + 1 == argc)
			{
				cout << "Error: No \"" << option << "\" option value.\n";
				return EXIT_FAILURE;
			}

			index = stoi(string(argv[++i]));
		}
		else
		{
			cout << "Error: Unknown \"" << option << "\" option.\n";
			return EXIT_FAILURE;
		}
	}

	if (outputFilePath.empty())
		outputFilePath = inputFilePath + ".vfm";

	Converter converter;

	try
	{
		converter.convert(
			inputFilePath,
			outputFilePath,
			name,
			description,
			creator,
			scale,
			iconX,
			iconY,
			iconZ,
			index);
		return EXIT_SUCCESS;
	}
	catch (const exception& e)
	{
		cout << "Error: " << e.what() << ".\n";
		return EXIT_FAILURE;
	}
}
