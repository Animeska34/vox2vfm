## Usage

```
vox2vfm <path-to-input-file> [options]
```

### Options

| Option   | Description                                       | Default value              |
|----------|---------------------------------------------------|----------------------------|
| -o       | Path to the output file                           | `<path-to-input-file>.vfm` |
| -n       | Name of the model                                 | `Unnamed`                  |
| -d       | Description of the model                          | `Converted using vox2vfm.` |
| -c       | Creator of the model                              | `vox2vfm`                  |
| -s       | Scale multiplier of the model                     | `1.0`                      |
| -i       | Index of the model in the scene if more than one. | `0`                        |
| -ix      | Icon rotation along X-axis in degrees.            | `45`                       |
| -iy      | Icon rotation along Y-axis in degrees.            | `30`                       |
| -ix      | Icon rotation along Z-axis in degrees.            | `0`                        |
| --shrink | Resize model to fit it into the maximum volume    |                            |

## Supported operating systems

* Ubuntu
* MacOS
* Windows

## Cloning

```
git clone --recursive https://github.com/Animeska34/vox2vfm
```

## Third-party

* [opengametools](https://github.com/jpaver/opengametools/) (MIT license)
* [voxfield-core](https://github.com/cfnptr/voxfield-core/) (Apache-2.0 License)
