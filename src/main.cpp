#include <iostream>
#include <getopt.h>
#include "canvas.h"
#include "network.h"

void printHelp(){
    std::cout <<
        "--help\tShow this help\n" <<
        "--fullscreen\tRun in Fullscreen";
    exit(1);
}

int main(int argc, char*argv[]){
    const char* const short_opts = "fh?";
    int optionIndex;
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
    };

    while(int opt = getopt_long(argc, argv, short_opts, nullptr, nullptr) != -1){
      switch(opt){
        case 'f':{
        std::cout << "FULLSCREEN\n";
      }break;
        case 'h':
        case '?':{
            printHelp();
        }break;
      }
    }

    Canvas *canvas = new Canvas();
    Network *network = new Network();

    while( !canvas->quit() ){
       canvas->markStart();
       network->tick(canvas);
       canvas->tick();
    }

    delete canvas;
    return 0;
}
