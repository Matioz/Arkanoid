#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<cstdlib>
#include<ctime>
#include<string.h>
#include "Arkanoid__defines.h"
extern "C" {
#ifdef BIT64
#include"./sdl64/include/SDL.h"
#include"./sdl64/include/SDL_main.h"

#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
#endif
}
