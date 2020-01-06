# Jehova - A Pixelflut server

Yet another Pixelfut server. Might have some Features other servers lack.

## Requirements
 
 * SDL2
 * SDL TTF
 * SDL Image
 * CMake
 * Some C++ Compiler

 For Mint do something like this to install all build requirements:
 ```
 apt install libsdl2-dev libsdl-ttf2.0-dev libsdl2-image-dev cmake build-essential g++
 ```

## Build
In the Project root directory do:

```
mkdir build;
cd build;
cmake ../;
make
```

## Run / Options

If you run the server without any options it will open a Window with a Resolution of 1024x768 and listen to port 2342.

* --avahi / -a : Advertise Pixelflut via Avahi
* --avmsg, -m <string\>	: Specify Message for Avahi(Sets also -a)
* --port / -p <number\> :  Set the port the server will listen to.
* --fullscreen / -f : Run in Fullscreen mode. If --width and hight are given defaults to the native display resolution.
* --height / -y <height\> :  Set display height. Defaults to 768 in windowed mode and to the native resolution in fullscreen mode.
* --width / -x <width\> : Set display width. Defaults to 1024 in windowed mode and to the native resolution in fullscreen mode.
* --title / -t <title\> : Set the window title.
* --save-path / -c <path\> :  If this path is set, the server will start to save screenshots of the display at the interval given in --save-interval. Please note that the Path should end with a "/". All saved files will have the form: jehova_<epoch\>.png
* --save-interval / -d <interval in ms\> : Interval to trigger screenshots given in milliseconds. The default value is 600.
* --start-image / -s <path\> : Displays the image given in the path at server startup.

## Protocol

Currently the only command supportet is:

```
PX <X position> <Y postions> <RGB Values as hex>
```
