#include "canvas.h"

int Canvas::init(){
    this->win = nullptr;
    this->ren = nullptr;
    this->quitHappend = false;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        std::cout << "SDL could not initialize! SDL_Error:\n\t" << SDL_GetError() << std::endl;
        return -1;
    }else{
        win = SDL_CreateWindow("Jehova - Pixelflut Server", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
        if( win == nullptr ){
            std::cout << "Window could not Created:\n\t" << SDL_GetError() << std::endl;
            return -1;
        }else{
            ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (ren == nullptr){
                std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(win);
                SDL_Quit();
                return -1;
            }
        }
    }
    TTF_Init();
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
    return 0;
}

Canvas::Canvas()
{
    this->init();
}

void Canvas::cleanup(){
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

Canvas::~Canvas(){
    this->cleanup();
}

void Canvas::handleEvent(){
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 ){
        if( e.type == SDL_QUIT ){
            this->quitHappend = true;
        }
    }
}
bool Canvas::quit(){
    return this->quitHappend;
}

void Canvas::tick(){
    this->handleEvent();
    if(!this->pixels.empty()){
        for(auto pixel : this->pixels){
            SDL_SetRenderDrawColor(this->ren, pixel.r, pixel.g, pixel.b, 255);
            SDL_RenderDrawPoint(this->ren, pixel.x, pixel.y);
        }
    }
    this->pixels.clear();
    SDL_RenderPresent(ren);

    Uint32 frameTime = SDL_GetTicks();
    if(frameTime < DELAY_TIME){
        SDL_Delay((int)(DELAY_TIME - frameTime));
    }
}

void Canvas::insert(Pixel pixel){
    this->pixels.push_back(pixel);
}

void Canvas::insert(int x, int y, int r, int g, int b){
    this->insert(Pixel(x,y,r,g,b));
}

void Canvas::markStart(){
    this->frameStart = SDL_GetTicks();
}
