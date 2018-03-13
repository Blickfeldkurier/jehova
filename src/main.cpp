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
        "\t--port/-p\tSet Listen Port\n" <<
        "\t--width/-x\tSet Window width\n";
    exit(0);
}

int main(int argc, char*argv[]){
    int opt;
    int option_index = 0;
    bool isFullscreen = false;

    COpts opts = {};
    opts.screenOpts = 0;
    opts.port = "2342";
    opts.title = "Jehova - An Pixelflut Server";
    opts.screenHeight = 0;
    opts.screenWidth = 0;

    static struct option long_options[] ={
        {"help", no_argument, nullptr, 'h'},
        {"borderless-window", no_argument, nullptr, 'b'},
        {"fullscreen", no_argument, nullptr, 'f'},
        {"port", required_argument, nullptr, 'p'},
        {"title", required_argument, nullptr, 't'},
        {"width", required_argument, nullptr, 'x'},
        {"height", required_argument, nullptr, 'y'},
        {0,0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "bfhp:t:x:y:", long_options, &option_index)) != -1){
      switch(opt){
        case 'b':{
            opts.screenOpts = opts.screenOpts || SDL_WINDOW_BORDERLESS;
        }break;
        case 'f':{
            isFullscreen = true;
            opts.screenOpts = opts.screenOpts || SDL_WINDOW_FULLSCREEN;
        }break;
        case 'h':{
            printHelp();
        }break;
        case 'p':{
            opts.port = std::string(optarg);
        }break;
        case 't':{
           opts.title = std::string(optarg);
        }break;
        case 'x':{
          opts.screenWidth = atoi(optarg);
        }break;
        case 'y':{
          opts.screenHeight = atoi(optarg);
        }break;
        default:
            printHelp();
        break;
      }
    }

    //Set sane default values for canvas if no Fullscreen and Resolution not given.
    if(isFullscreen == false && (opts.screenHeight == 0 && opts.screenWidth == 0)){
        opts.screenWidth = 1024;
        opts.screenHeight = 768;
    }

    if(isFullscreen == true &&  (opts.screenHeight == 0 && opts.screenWidth == 0)){
        opts.screenHeight = -1;
    }

    Canvas *canvas = new Canvas(opts);
    Network *network = new Network(atoi(opts.port.c_str()));

    while( !canvas->quit() ){
       canvas->markStart();
       network->tick(canvas);
       canvas->tick();
    }

    delete canvas;
    return 0;
}
