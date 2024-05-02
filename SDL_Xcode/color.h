//
//  color.h
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/14/24.
//

#ifndef color_h
#define color_h

#include <stdint.h>


typedef uint32_t color_abgr_t;

// NOTE: pixel format is ABGR

#define ABGR_BLACK (0xFF000000)
#define ABGR_WHITE (0xFFFFFFFF)

color_abgr_t blend_color(color_abgr_t x, color_abgr_t y);

color_abgr_t color_from_hsv(double h, double s, double v, double a);

#endif /* color_h */
