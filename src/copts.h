#ifndef COPTS_H
#define COPTS_H

#include <string>
#include "SDL.h"

class COpts{
public:
    int screenWidth;
    int screenHeight;
    Uint32 screenOpts;
    std::string title;
    std::string port;
    std::string startImagePath;
    std::string screenPath;
    int screenInterval;
};

#endif