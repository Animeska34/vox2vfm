# vox2vfm
Utility to convert MagicVoxel models to Voxfield models
## Features
* Convert vox to vfm
* Check vox files
## Usage
#### Converting a vox file to vfm
```
 vox2vfm test.vox -t test.vfm -n "Some model" -d "Some model description" -s 1
```
#### Checking vox file
```
 vox2vfm test.vox -c
```
### Arguments description
| Arg | Description                             | Default value | Required |
|-----|-----------------------------------------|---------------|----------|
|     | First argument is a filePath to convert |               |    Yes   |
| -t  | Destination file path                   |filePath+`.vfm`|    No    |
| -n  | Object name                             |    `None`     |    No    |
| -d  | Object description                      |    `None`     |    No    |
| -s  | Object scale                            |    `1.0`      |    No    |
| -c  | Only check .vox file (no conversion     |               |    No    |

## Supported operating systems
* Ubuntu
* MacOS
* Windows

## Download
All versions cam be dovnloaded from https://github.com/Animeska34/vox2vfm/releases

## Cloning
```
git clone --recursive https://github.com/Animeska34/vox2vfm
```

## Third-party
* [opengametools](https://github.com/jpaver/opengametools/) (MIT license)
* [voxfield-core](https://github.com/cfnptr/voxfield-core/) (Apache-2.0 License)
