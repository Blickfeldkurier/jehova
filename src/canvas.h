#ifndef CANVAS_H
#define CANVAS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "copts.h"

#include <iostream>
#include <list>
#include <chrono>

class Pixel{
public:
    Pixel(){
      this->x = 0;
      this->y = 0;
      this->r = 0;
      this->g = 0;
      this->b = 0;
      this->a = 255;
    }

    Pixel(int x, int y, int r, int g, int b, int a=255){
        this->x = x;
        this->y = y;
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    int x;
    int y;
    int r;
    int g;
    int b;
    int a;
};

class Canvas
{
private:

   const unsigned int FPS=60;
   const unsigned int DELAY_TIME = 1000.0f/FPS;
   Uint32 frameStart;

   SDL_Window *win;
   SDL_Renderer *ren;
   SDL_TimerID screenTimer;
   std::list<Pixel> pixels;
   int winHeight;
   int winWidth;
   std::string screenPath;

   int init(COpts opts);
   void cleanup();
   void handleEvent();
   bool quitHappend;

public:
    Canvas(COpts opts);
    ~Canvas();
    void markStart();
    void tick();
    bool quit();
    void insert(int x, int y, int r, int g, int b);
    void insert(int x, int y, int r, int g, int b, int a);
    void insert(Pixel pixel);
    void takeScreenshot();
};

#endif // CANVAS_H
