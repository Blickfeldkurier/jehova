#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <cstdlib>
#include "SDL.h"
#include "canvas.h"
#include "network.h"

void printHelp(){
    std::cout <<
        "Usage: jehova [Options]\n\n" <<
        "Options:\n"
        "\t--borderless-window/-b\tRun in Borderles Window Mode\n" <<
        "\t--fullscreen/-f\tRun in Fullscreen\n" <<
        "\t--help/-h\tShow this help\n" <<
        "\t--height/-y\tSet Window height\n" <<
        "\t--width/-x\tSet Window width\n";
    exit(0);
}

int main(int argc, char*argv[]){
    int opt;
    int option_index = 0;
    Uint32 screenOpts = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    bool isFullscreen = false;

    static struct option long_options[] ={
        {"help", no_argument, nullptr, 'h'},
        {"borderless-window", no_argument, nullptr, 'b'},
        {"fullscreen", no_argument, nullptr, 'f'},
        {"width", required_argument, nullptr, 'x'},
        {"height", required_argument, nullptr, 'y'},
        {0,0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "bhfx:y:", long_options, &option_index)) != -1){
      switch(opt){
        case 'b':{
            screenOpts = screenOpts || SDL_WINDOW_BORDERLESS;
        }break;
        case 'f':{
            isFullscreen = true;
            screenOpts = screenOpts || SDL_WINDOW_FULLSCREEN;
        }break;
        case 'h':{
            printHelp();
        }break;
        case 'x':{
          screenWidth = atoi(optarg);
        }break;
        case 'y':{
          screenHeight = atoi(optarg);
        }break;
        default:
            printHelp();
        break;
      }
    }

    //Set sane default values for canvas if no Fullscreen and Resolution not given.
    if(isFullscreen == false && (screenHeight == 0 && screenWidth == 0)){
        screenWidth = 1024;
        screenHeight = 768;
    }

    if(isFullscreen == true &&  (screenHeight == 0 && screenWidth == 0)){
        screenHeight = -1;
    }

    Canvas *canvas = new Canvas(screenWidth, screenHeight, screenOpts);
    Network *network = new Network();

    while( !canvas->quit() ){
       canvas->markStart();
       network->tick(canvas);
       canvas->tick();
    }

    delete canvas;
    return 0;
}
