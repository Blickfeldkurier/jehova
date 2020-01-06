#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <cstdlib>
#include "SDL.h"
#include "canvas.h"
#include "network.h"
#include "avahiservice.h"

void printHelp(){
    std::cout <<
        "Usage: jehova [Options]\n\n" <<
        "Options:\n"
        "\t--avahi, -a\t\tAdvertise Pixelflut via Avahi\n" <<
        "\t--avmsg, -m\t\tSpecify Message for Avahi(Sets also -a)\n"
        "\t--borderless-window, -b\t\tRun in Borderles Window Mode (Default: false)\n" <<
        "\t--fullscreen, -f\t\tRun in Fullscreen (Default: false)\n" <<
        "\t--help, -h\t\tShow this help\n" <<
        "\t--height/-y\t\tSet Window height\n\t\t\t(Default: 768 for Windowed, nvative for Fullscreen)\n" <<
        "\t--port, -p\t\tSet Listen Port (Default: 2342)\n" <<
        "\t--save-path, -c\t\tPath to save Screenshots to (Default: empty)\n" <<
        "\t--save-interval, -d\t\tScreenshot capture interval (Default: 600)\n" <<
        "\t--start-image, -s\t\tPath to start Image Path (Default: empty)\n" <<
        "\t--title, -t\t\tSet Window title and OSD Text\n" <<
        "\t--width, -x\t\tSet Window width\n\t\t\t(Default: 1024 for Windowed, native for Fullscreen)\n";
    exit(0);
}

int main(int argc, char*argv[]){
    int opt;
    int option_index = 0;
    bool isFullscreen = false;
    bool hazAvahi = false;
    std::string avahiMsg = "";

    COpts opts = {};
    opts.screenOpts = 0;
    opts.port = "2342";
    opts.title = "Jehova - An Pixelflut Server";
    opts.screenHeight = 0;
    opts.screenWidth = 0;
    opts.startImagePath = "";
    opts.screenInterval = 600;
    opts.screenPath = "";

    static struct option long_options[] ={
        {"avahi", no_argument, nullptr, 'a'},
        {"avmsg", required_argument, nullptr, 'm'},
        {"help", no_argument, nullptr, 'h'},
        {"save-path", required_argument, nullptr, 'c'},
        {"save-interval", required_argument, nullptr, 'd'},
        {"borderless-window", no_argument, nullptr, 'b'},
        {"fullscreen", no_argument, nullptr, 'f'},
        {"port", required_argument, nullptr, 'p'},
        {"start-image", required_argument, nullptr, 's'},
        {"title", required_argument, nullptr, 't'},
        {"width", required_argument, nullptr, 'x'},
        {"height", required_argument, nullptr, 'y'},
        {0,0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "abc:d:fhm:p:s:t:x:y:", long_options, &option_index)) != -1){
      switch(opt){
        case 'a':{
            hazAvahi = true;
        }break;
        case 'b':{
            opts.screenOpts = opts.screenOpts || SDL_WINDOW_BORDERLESS;
        }break;
        case 'c':{
            opts.screenPath = std::string(optarg);
        }break;
        case 'd':{
            opts.screenInterval = atoi(optarg);
        }break;
        case 'f':{
            isFullscreen = true;
            opts.screenOpts = opts.screenOpts || SDL_WINDOW_FULLSCREEN;
        }break;
        case 'h':{
            printHelp();
        }break;
        case 'm':{
            avahiMsg = std::string(optarg);
            hazAvahi = true;
        }break;
        case 'p':{
            opts.port = std::string(optarg);
        }break;
        case 's':{
            opts.startImagePath = std::string(optarg);
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
    AvahiService *avahi = nullptr;
    if(hazAvahi == true){
        avahi = new AvahiService(opts, avahiMsg);
    }
    Canvas *canvas = new Canvas(opts);
    Network *network = new Network(atoi(opts.port.c_str()));

    while( !canvas->quit() ){
       canvas->markStart();
       network->tick(canvas);
       canvas->tick();
    }

    delete canvas;
    if(avahi != nullptr){
        delete avahi;
    }
    return 0;
}
