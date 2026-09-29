<p align="center">
  <img src=".github/banner.svg" width="100%" alt="Reference Tools · MPEG V3C Immersive Platform: Content for V3C Immersive Platform">
</p>

<p align="center">
  Configuration files and V3C test content for the
  <a href="https://github.com/5G-MAG/rt-v3c-unity-player">V3C Immersive Platform Unity Player</a>,
  with tools to generate V3C bitstreams and DASH segments.
</p>

<p align="center">
  <img alt="Status: Under Development"
    src="https://img.shields.io/badge/Status-Under%20Development-e67e22">
  <a href="https://github.com/5G-MAG/rt-v3c-content/releases"><img alt="Version"
    src="https://img.shields.io/github/v/release/5G-MAG/rt-v3c-content?label=Version"></a>
  <a href="LICENSE"><img alt="License: Multiple"
    src="https://img.shields.io/badge/License-Multiple-blue"></a>
</p>

<p align="center">
  <a href="https://www.5g-mag.com/reference-tools/v3c/">Project page</a> &nbsp;&middot;&nbsp;
  <a href="https://github.com/5G-MAG/rt-v3c-content/issues">Issues</a> &nbsp;&middot;&nbsp;
  <a href="https://www.5g-mag.com/contributing">Contributing</a>
</p>

---

## At a glance

|  |  |
|---|---|
| **Part of** | [MPEG V3C Immersive Platform](https://www.5g-mag.com/reference-tools/v3c/), alongside [rt-v3c-unity-player](https://github.com/5G-MAG/rt-v3c-unity-player), [rt-v3c-decoder-plugin](https://github.com/5G-MAG/rt-v3c-decoder-plugin), [rt-v3c-examples](https://github.com/5G-MAG/rt-v3c-examples) and [rt-media-origin](https://github.com/5G-MAG/rt-media-origin) |

## Introduction

This repository holds the files that configure and test the
[V3C Immersive Platform Unity Player](https://github.com/5G-MAG/rt-v3c-unity-player) application:
test content to play from the device and from a DASH server, and the configuration files that tell
the application what to play. It also holds tools and scripts to generate V3C encoded bitstreams
and DASH-segmented V3C bitstreams.
[rt-v3c-decoder-plugin](https://github.com/5G-MAG/rt-v3c-decoder-plugin) includes this repository
as its `V3C-Content` submodule.

| Folder | Contents |
|---|---|
| `on-device-data` | files that configure the test device running the application |
| `on-server-data` | files to install on the remote HTTP server |
| `tools/vpcc-generation-tools` | tools and scripts to generate encoded V3C V-PCC bitstreams |
| `tools/v3c-dash-packager` | tools and scripts to generate V3C DASH segments from a V3C bitstream |

The test content, the same three sequences on the device and on the server, each under its own
licence:

| Content | Encoded with | Owner |
|---|---|---|
| DanceB, 4 GOP | V3C MIV MVD profile | Philips |
| Mannequin, 5 GOP | V3C MIV MPI profile | InterDigital |
| S41C2RAR05 footprod, 1 GOP | V3C V-PCC profile | XDprod |

## Installing

### On the test device

`on-device-data` holds everything to copy to the test device (a smartphone or tablet, for example)
for the application to run:

```bash
on-device-data  
│   config.json  
├───data
│   'DanceB_4GOP_V19'
│   'Mannequin-QP32-32_5GOP_V19'  
│   'S41C2RAR05_footprod_1GOP_R24'  
│   library.json    
```

The `on-device-data/data` directory holds the content read locally, one folder per sequence:
'DanceB_4GOP_V19', 'Mannequin_QP32-32_5GOP_V19' and 'S41C2RAR05_footprod_1GOP_R24'.

### On the DASH server

`on-server-data` holds the data to copy to the DASH server:

```bash
on-server-data
├───V3Ctest
│   'Mannequin_QP32-32_5GOP_V19.zip'
│   'S41C2RAR05_footprod_1GOP_R24.zip'
│   'DanceB_4GOP_V19.zip'
```

The zip files in the `on-server-data/V3Ctest` folder hold the same V3C content (V-PCC and MIV),
encapsulated and segmented for DASH, for remote tests. Unzip them on the DASH server; the unzipped
content is ready to copy as it is to the right location for the server's set-up.

- Mannequin_QP32-32_5GOP_V19.zip: a 5 GOP MIV encoded bitstream
- S41C2RAR05_footprod_1GOP_R24.zip: a 1 GOP V-PCC encoded bitstream
- DanceB_4GOP_V19.zip: a 4 GOP MIV encoded bitstream

[README_dash_server.md](README_dash_server.md) explains how to install and set up an Apache server
as a simple DASH server.

## Configuration

### `config.json`

`config.json` configures:

- the DASH server settings, for example name, IP address and port
- the decoder settings, such as software or hardware decoding
- some scheduling adjustments
- the names of the synthesizer modules

### `library.json`

`library.json`, in the `data` folder, holds the content playlist the application parses. The test
example defines 6 blocks: 3 for local content and 3 for remote content. Each block and its streams
use these fields:

- `Mode`: `"local"` for content in the data directory, or `"dash"` for content on a DASH server.
- `Duration`: the duration of each local chunk, that is one `.bit` file (MIV encoded bitstreams) or
  one `.bin` file (V-PCC encoded bitstreams).
- `NbFrame`: the number of frames in each local chunk, normally a GOP.
- `NbSegment`: the number of local chunks in the data directory.
- `Path`: the file name template of the local chunks.
- `ServerName`: the name of the DASH server, whose settings (the hostname, for example) are in
  `config.json`.
- `Type`: the stream type. The decoder plugin recognises `vpcc`, `miv`, `haptic`, `audio`, `hevc`
  and `vvc`, compared exactly, so in lower case; it reads any other value as no type. The example
  file uses `vpcc`, `miv` and `haptic`.
- `Url`: the URL of the DASH `.mpd` file, for example "XXX/S41C2RAR05_footprod_1GOP_R24/stream.mpd",
  where XXX depends on how the server is configured (for example "V3Ctest").

## Contributing

Contributions are welcome. How to raise an issue, fork the repository and open a pull request, and
the Contributor License Agreement required before code can be merged, are described at
<https://www.5g-mag.com/contributing>.

## License

This repository contains code and example content under different licences: the example content
under the licence of each sequence, and the V-PCC generation tools and the V3C DASH Packager code
under the 5G-MAG Public License v1.0. See [LICENSE](LICENSE) for the details and the third-party
binaries distributed with the packager.
