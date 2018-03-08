#ifndef CANVAS_H
#define CANVAS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <iostream>
#include <list>

class Pixel{
public:
    Pixel(){
      this->x = 0;
      this->y = 0;
      this->r = 0;
      this->g = 0;
      this->b = 0;
    }

    Pixel(int x, int y, int r, int g, int b){
        this->x = x;
        this->y = y;
        this->r = r;
        this->g = g;
        this->b = b;
    }

    int x;
    int y;
    int r;
    int g;
    int b;
};

class Canvas
{
private:

   SDL_Window *win;
   SDL_Renderer *ren;
   std::list<Pixel> pixels;

   int init();
   void cleanup();
   void handleEvent();
   bool quitHappend;

public:
    Canvas();
    ~Canvas();
    void tick();
    bool quit();
    void insert(int x, int y, int r, int g, int b);
    void insert(Pixel pixel);
};

#endif // CANVAS_H
