<h1 align="center">V3C Immersive Platform - Simple DASH servers setup for tests</h1>
V3C Immersive platform has been tested with the following HTTP servers for DASH V3C content.
Click one to install and setup.
<p/>

<details><summary><b>Apache</b></summary>
  
# Using Apache

Apache server can be used as a simple DASH server.  This readme describes how to set it up to make it run with V-PCC and MIV content.  

## Pre-requisites
Install Apache server on your computer.
It can be installed on C: drive.
>**Example:**  
>C:/Apache24/
  
If installed in another location, update the following instructions accordingly.

## Basic configuration  

- Open httpd.conf file in C:/apache24/conf/  
- Go to the line where SRVROOT is defined  
- Update the SRVROOT variable with the right location "C:/Apache24"  
>**Example:**  
>Define SRVROOT "C:/Apache24"  
- Update DocumentRoot with the location where the contents will be.  
>**Example:**
>DocumentRoot "${SRVROOT}/mediaContent"

><Directory "${SRVROOT}/mediaContent">  
- You may want to update the server's name and the associated Port, then update ServerName.  
>**Example:**  
>ServerName localhost:8080  
- The port where Apache listens to is defined with Listen  
>**Example:**  
>Listen 8080  

## Server content

Create a new folder on the server's public folder and copy the V-PCC and MIV content there.

**Example:**

- Create a new folder `V3Ctest` in directory 'C:/Apache24/mediaContent'
- Then copy the V3C test contents (unzip V-PCC and MIV content) in this `V3Ctest` folder.

## Running Apache  

Open a console and enter the Apache bin directory and launch the httpd.exe binary.  
**Example:** 

```shell
cd C:\Apache24\bin
httpd.exe
```
</details>

<details><summary><b>Node.JS Simple Express</b></summary>
  
# Using Node.JS Simple Express

Simple Express server can be used as a simple DASH server. 

This readme describes how to set it up to make it run with V-PCC and MIV content.  

# Pre-requisites
Install Simple Express server on your computer by following instructions here:

https://github.com/5G-MAG/rt-common-shared/tree/main/simple-express-server

It can be installed on C: drive.
>**Example:**  
>C:/rt-common-shared/simple-express-server
  
If installed in another location, update the following instructions accordingly.

# Basic configuration  

You can keep the default configuration, or change the default port as indicated in https://github.com/5G-MAG/rt-common-shared/tree/main/simple-express-server

# Server content

Create a new folder on the server's public folder and copy the V-PCC and MIV content there.

**Example:**

- Create a new folder `V3Ctest` in directory 'C:/rt-common-shared/simple-express-server/public'
- Then copy the V3C test contents (unzip V-PCC and MIV content) in this `V3Ctest` folder.

# Start the server  

Open a console (git bash console under Windows) and enter the command:
**Example:** 

```shell
C:/rt-common-shared/simple-express-server
npm start
```
</details>
