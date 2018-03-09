#include <iostream>
#include "canvas.h"
#include "network.h"

int main(int argc, char*argv[]){
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
