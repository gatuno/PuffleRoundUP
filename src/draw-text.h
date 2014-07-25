/*
 * draw-text.h
 * This file is part of Paddle Puffle
 *
 * Copyright (C) 2012 - Félix Arreola Rodríguez
 *
 * Paddle Puffle is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Paddle Puffle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Paddle Puffle; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#ifndef __DRAW_TEXT_H__
#define __DRAW_TEXT_H__

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif

#include <SDL.h>

/* Prototipos de función */
SDL_Surface *draw_text (TTF_Font *font, const char *cadena, SDL_Color *color);
SDL_Surface *draw_text_with_shadow (TTF_Font *font_normal, int outline, char *text, SDL_Color *fill, SDL_Color *shadow);

#endif /* __DRAW_TEXT_H__ */

