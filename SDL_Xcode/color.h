//
//  color.h
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/14/24.
//

#ifndef color_h
#define color_h

#include <stdint.h>

// NOTE: pixel format is ABGR
#define BLACK_COLOR (0xFF000000)
#define WHITE_COLOR (0xFFFFFFFF)

uint32_t blend_color(uint32_t x, uint32_t y);
uint32_t color_from_hsv(float h, float s, float b, float a);

#endif /* color_h */
