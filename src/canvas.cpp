#include "canvas.h"

int Canvas::init(COpts opts){
    this->win = nullptr;
    this->ren = nullptr;
    this->quitHappend = false;

    int localX = opts.screenWidth;
    int localY = opts.screenHeight;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        std::cout << "SDL could not initialize! SDL_Error:\n\t" << SDL_GetError() << std::endl;
        return -1;
    }else{
        //If no resolition is given at command line, we use the native one in Fullscreen mode
        if(localY == -1){
            SDL_DisplayMode DM;
            if(SDL_GetDesktopDisplayMode(0,&DM) == 0){
                std::cout << "" << DM.w << "/" << DM.h << "\n";
                localX = DM.w;
                localY = DM.h;
            }
        }

        win = SDL_CreateWindow(opts.title.c_str(), 100, 100, localX, localY, opts.screenOpts);
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

    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    TTF_Init();
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    if(opts.startImagePath.empty() == false){
           SDL_Surface * image = IMG_Load(opts.startImagePath.c_str());
           SDL_Texture * texture = SDL_CreateTextureFromSurface(ren, image);
           SDL_RenderCopy(ren, texture, NULL, NULL);
    }
    SDL_RenderPresent(ren);
    return 0;
}

Canvas::Canvas(COpts opts)
{
    this->init(opts);
}

void Canvas::cleanup(){
    TTF_Quit();
    IMG_Quit();
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
            SDL_SetRenderDrawColor(this->ren, pixel.r, pixel.g, pixel.b, pixel.a);
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

void Canvas::insert(int x, int y, int r, int g, int b, int a){
    this->insert(Pixel(x,y,r,g,b,a));
}

void Canvas::markStart(){
    this->frameStart = SDL_GetTicks();
}
