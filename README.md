<h1 align="center">V3C Immersive Platform - Default test configuration</h1>
<p align="center">
  <img src="https://img.shields.io/badge/Status-Under_Development-yellow" alt="Under Development">
  <img src="https://img.shields.io/badge/License-Multiple-blue" alt="License">
</p>

This folder contains the files to configure and test the [V3CImmersivePlatform - Unity Player](https://github.com/5G-MAG/rt-v3c-unity-player) application.

It is organized with:
- `on-device-data` : folder with files to configure the test device running the application
- `on-server-data` : folder with files to install on the remote HTTP server

Contents are provided with specific licenses.

# Description of `on-device-data` folder

It contains all the data that shall be copied on the test device, e.g. a smartphone, a tablet, to run the application correctly.  
It is organized as follows:  

```bash
on-device-data  
│   config.json  
├───data
│   'DanceB_4GOP_V19'
│   'Mannequin-QP32-32_5GOP_V19'  
│   'S41C2RAR05_footprod_1GOP_R24'  
│   library.json    
```

## Configuration file `config.json`

It is used to configure:  

- the DASH server settings, e.g. name, IP address and port  
- video encoder settings like SW or HW decoding  
- some scheduling adjustment settings  
- synthetizers module naming  

## `on-device-data/data` folder

The data directory contains contents to be read locally:

- 'DanceB_4GOP_V19' : test content encoded with V3C MIV MVD profile (ownership Philips)
- 'Mannequin_QP32-32_5GOP_V19' : test content encoded with V3C MIV MPI profile (ownership InterDigital)
- 'S41C2RAR05_footprod_1GOP_R24' : test content encoded with V3C V-PCC profile (ownership XDprod)

These content are provided with specific licenses.

## Configuration file `library.json`  

It contains the content playlist parsed by the Application.

In the **test example** configuration provided, 6 blocks are defined:
 - 3 for local contents,
 - 3 for remote contents.

**mode** shall be set to "local" or "dash" depending on what contents is to be played, local content present in data directory or DASH contents present on a DASH server.  
**duration** specifies the duration of each local chunk, i.e. duration of one .bit file (for MIV encoded bitstreams), or one .bin (for V-PCC encoded bitstreams)  
**NbFrame** specifies the number of frames in each local chunk, basically a GOP.  
**NbSegment** specifies the number of local chunks present in the data directory.  
**path** specifies the template of all the local chunks.  
**serverName** specifies the name of the DASH server. The DASH server settings are specified in the config.json file, e.g. hostname.  
**type** specifies the video type, it can be VPCC, MIV or 2d.  
**url** specifies the URL of the DASH .mpd file, e.g. "XXX/S41C2RAR05_footprod_1GOP_R24/stream.mpd", where XXX depends on how the server has been configured (e.g. "V3Ctest").  

# Description of `on-server-data` folder

It contains the data that shall be copied on the DASH server.  

It is organized as follows:

```bash
on-server-data
├───V3Ctest
│   'Mannequin_QP32-32_5GOP_V19.zip'
│   'S41C2RAR05_footprod_1GOP_R24.zip'
│   'DanceB_4GOP_V19.zip'
```

If needed, a [readme](https://github.com/5G-MAG/rt-v3c-decoder-plugin/Tests/README_dash_server.md) is also provided to explain how to install and set-up an Apache server as a DASH server for simple usage.

# `on-server-data/V3Ctest` folder

It contains zip files with V3C content (V-PCC and MIV). These files can be unzip on the DASH server for remote tests.

These content are provided with specific licenses.

- Mannequin_QP32-32_5GOP_V19.zip : test content encoded with V3C MIV MPI profile (ownership InterDigital)

A 5 GOP MIV encoded bitstream that has been encapsulated and segmented for DASH.  
Unzipped content is ready to be copied as is on the DASH server, in the proper location, according to the server set-up.  

- S41C2RAR05_footprod_1GOP_R24.zip : test content encoded with V3C V-PCC profile (ownership XDprod)

A 1 GOP V-PCC encoded bitstream and that has been encapsulated and segmented for DASH.
Unzipped content is ready to be copied as is on the DASH server, in the proper location, according to the server set-up.

- DanceB_4GOP_V19.zip : test content encoded with V3C MIV MVD profile

A 4 GOP MIV encoded bitstream that has been encapsulated and segmented for DASH.  
Unzipped content is ready to be copied as is on the DASH server, in the proper location, according to the server set-up.  

