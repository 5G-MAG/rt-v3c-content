# V3C DASH Packager

This directory provides a tool for preparing and processing V3C bitstreams into V3C DASH segments for compatibility with the V3C Immersive Platform.


Input is a V3C encoded bitstream compliant with V3C reference software for V-PCC (i.e. TMC2) or MIV (i.e. TMIV) :
- tmiv, v23.0, [gitlab.com/mpeg-i-visual/tmiv](https://gitlab.com/mpeg-i-visual/tmiv.git)
- tmc2, v25.0, [github.com/MPEGGroup/mpeg-pcc-tmc2](https://github.com/MPEGGroup/mpeg-pcc-tmc2.git)

Output is DASH segments according to Carriage of V3C data specification ISO/IEC 23090-10 and compatible with the V3C Immersive Platform - DASH interface. 

V-PCC encoded bitstream can be generated with `encodePLY` script provided in **vpcc-generation-tools**.

## Usage
Under Windows OS, open a bash shell (e.g. Git bash).

Use `generate.sh` script to run the v3cpackager.exe executable that will generate V3C DASH segments.

Read the help to set the arguments.
```

$ ./generate.sh
Usage:
    -i | --inDir "Input directory with V-PCC encoded bitstream"
    -f | --filelist "File that contains a list of MIV encoded bitstreams with their absolute path"
                    "Note: Use absolute path for the file list."
    -o | --outDir "Output directory to dump the generated segments"
    --fps_occ "V3C bitstream FPS for occupancy, typically 25 or 30"
    --fps_geo "V3C bitstream FPS for geometry, typically 25 or 30"
    --fps_atl "V3C bitstream FPS for atlas, typically 25 or 30"
    --fps_att "V3C bitstream FPS for attribute, typically 25 or 30"
    --fps "V3C bitstream FPS for occupancy, geometry, atlas and attribute, typically 25 or 30"
    -c | --clear "clear old output files"
    -m | --merge-inDir "build a filelist from inDir files and process extrat on built filelist"
    -h | --help

Note:
  1) Search for .bit or .bin file recursively in the input directory
  2) Directory paths may be absolute or relative
```

### Example 1 - V-PCC
```
./generate.sh -i /path/to/vpccBitstreamDir -o /tmp/outDir --fps 30 
```

### Example 2 - MIV
```
./generate.sh -f mannequin_1GOP_list.txt -o /tmp/outDir --fps 30
```

## How to generate the list of .bit files?
In the next example, we suppose the .bit files are located in the /miv/ directory.
```
$ cd bitstreams
$ ls -l -d `pwd -W`/miv/* | awk 'NF{ print $NF }' > mannequin_1GOP_list.txt
```

## License

This project is provided under 5G-MAG's Public License. For the full license terms, please see the LICENSE file distributed along with the repository or retrieve it from [here](https://drive.google.com/file/d/1cinCiA778IErENZ3JN52VFW-1ffHpx7Z/view).

v3cpackager.exe (ownership InterDigital R&D France) relies on Bento4 library and tools (Bento4 Software Copyright © Axiomatic Systems LLC , 2002-2014 . All rights reserved.).

v3cpackager.exe is also distributed with a software (binary) : libcurl, v8.6.0, https://curl.se/docs/copyright.html

