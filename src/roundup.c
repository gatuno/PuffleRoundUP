/*
 * roundup.c
 * This file is part of Puffle Round UP
 *
 * Copyright (C) 2013 - Félix Arreola Rodríguez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ----------------
 * LEGAL NOTICE
 * ----------------
 *
 * This game is NOT related to Club Penguin in any way. Also,
 * this game is not intended to infringe copyrights, the graphics and
 * sounds used are Copyright of Disney.
 *
 * The new SDL code is written by Gatuno, and is released under
 * the term of the GNU General Public License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FPS (1000/24)

#define SWAP(a, b, t) ((t) = (a), (a) = (b), (b) = (t))
#define RANDOM(x) ((int) (x ## .0 * rand () / (RAND_MAX + 1.0)))

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#define PUFFLE_TILE_HEIGHT 34
#define PUFFLE_TILE_WIDTH 32

/* Enumerar las imágenes */
enum {
	IMG_FONDO,
	IMG_PUFFLE_PEN,
	
	IMG_PUFFLE_BLUE,
	
	IMG_PEN,
	IMG_WALL,
	
	NUM_IMAGES
};

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/fondo.png",
	GAMEDATA_DIR "images/puffle-pen.png",
	
	GAMEDATA_DIR "images/puffle-azul.png",
	
	GAMEDATA_DIR "images/pen.png",
	GAMEDATA_DIR "images/wall.png"
};

/* Enumerar las caras de los Puffles */
enum {
	FACE_0_0, FACE_0_1, FACE_0_2, FACE_0_3,
	FACE_1_0, FACE_1_1, FACE_1_2, FACE_1_3,
	FACE_2_0, FACE_2_1, FACE_2_2, FACE_2_3,
	FACE_3_0,
	FACE_4_0,
	FACE_5_0,
	FACE_DUMMY, /* Intencionalmente se deja en blanco para alinear cada 4 las caras */
	FACE_6_0, FACE_6_1, FACE_6_2, FACE_6_3,
	FACE_7_0, FACE_7_1, FACE_7_2, FACE_7_3,
	FACE_J0_0, FACE_J0_1, FACE_J0_2, FACE_J0_3, FACE_J0_4, FACE_J0_5, FACE_J0_6, FACE_J0_7, FACE_J0_8, FACE_J0_9, FACE_J0_10, FACE_J0_11,
	FACE_J1_0, FACE_J1_1, FACE_J1_2, FACE_J1_3, FACE_J1_4, FACE_J1_5, FACE_J1_6, FACE_J1_7, FACE_J1_8, FACE_J1_9, FACE_J1_10, FACE_J1_11,
	FACE_J2_0, FACE_J2_1, FACE_J2_2, FACE_J2_3, FACE_J2_4, FACE_J2_5, FACE_J2_6, FACE_J2_7, FACE_J2_8, FACE_J2_9, FACE_J2_10, FACE_J2_11,
	FACE_J3_0, FACE_J3_1, FACE_J3_2, FACE_J3_3, FACE_J3_4, FACE_J3_5, FACE_J3_6, FACE_J3_7, FACE_J3_8, FACE_J3_9, FACE_J3_10, FACE_J3_11,
	FACE_J4_0, FACE_J4_1, FACE_J4_2, FACE_J4_3, FACE_J4_4, FACE_J4_5, FACE_J4_6, FACE_J4_7, FACE_J4_8, FACE_J4_9, FACE_J4_10, FACE_J4_11,
	FACE_J5_0, FACE_J5_1, FACE_J5_2, FACE_J5_3, FACE_J5_4, FACE_J5_5, FACE_J5_6, FACE_J5_7, FACE_J5_8, FACE_J5_9, FACE_J5_10, FACE_J5_11,
	FACE_J6_0, FACE_J6_1, FACE_J6_2, FACE_J6_3, FACE_J6_4, FACE_J6_5, FACE_J6_6, FACE_J6_7, FACE_J6_8, FACE_J6_9, FACE_J6_10, FACE_J6_11,
	FACE_J7_0, FACE_J7_1, FACE_J7_2, FACE_J7_3, FACE_J7_4, FACE_J7_5, FACE_J7_6, FACE_J7_7, FACE_J7_8, FACE_J7_9, FACE_J7_10, FACE_J7_11
};

enum {
	PUFFLE_DIR_0,
	PUFFLE_DIR_1,
	PUFFLE_DIR_2,
	PUFFLE_DIR_3,
	PUFFLE_DIR_4,
	PUFFLE_DIR_5,
	PUFFLE_DIR_6,
	PUFFLE_DIR_7,
	PUFFLE_WALK_0,
	PUFFLE_WALK_1,
	PUFFLE_WALK_2,
	PUFFLE_WALK_3,
	PUFFLE_WALK_4,
	PUFFLE_WALK_5,
	PUFFLE_WALK_6,
	PUFFLE_WALK_7,
	
	NUM_PUFFLE_MODS
};

/* Pseudo animación para el puffle */
const int puffle_frame_normal [55] = {
	/* Cuando el puffle mira al Frente */
	FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0,
	FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0,
	FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0,
	FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0,
	FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0, FACE_0_0,
	FACE_0_1, FACE_0_1,
	FACE_0_2, FACE_0_2, FACE_0_2, FACE_0_2,
	FACE_0_3, FACE_0_3,
	-1 /* Fin de la animación */
};

const int puffle_frame_jump [13] = {
	FACE_J0_0, FACE_J0_1, FACE_J0_2, FACE_J0_3, FACE_J0_4, FACE_J0_5, FACE_J0_6, FACE_J0_7, FACE_J0_8, FACE_J0_9, FACE_J0_10, FACE_J0_11,
	-1
};

/* Codigos de salida */
enum {
	GAME_NONE = 0, /* No usado */
	GAME_CONTINUE,
	GAME_QUIT
};

/* La meta información de los puffles */
const int puffle_data[10][2] = { /* {distancia, velocidad} */
	{45, 7} /* Puffle Azul */
};

/* Estructuras */
typedef struct {
	int capturado;
	int escapado;
	int distancia;
	int velocidad;
	int color;
	int frame;
	int dir;
	int x, y;
} Puffle;

/* Prototipos de función */
int game_loop (void);
void setup (void);
SDL_Surface * set_video_mode(unsigned);
void copy_puffle_tile (int puffle_tiles, SDL_Rect *rect, int tile);
double encontrar_distancia (int dx, int dy);
int encontrar_angulo_y_dir (int x1, int y1, int x2, int y2);

/* Variables globales */
SDL_Surface * screen;
SDL_Surface * images [NUM_IMAGES];

int main (int argc, char *argv[]) {
	
	setup ();
	
	do {
		if (game_loop () == GAME_QUIT) break;
	} while (1 == 0);
	
	SDL_Quit ();
	return EXIT_SUCCESS;
}

int game_loop (void) {
	int done = 0;
	SDL_Event event;
	SDLKey key;
	Uint32 last_time, now_time;
	SDL_Rect rect;
	int imagen;
	double distancia;
	int mousex, mousey, dx, dy;
	double incrementox, incrementoy;
	int nextx, nexty;
	
	Puffle azul;
	
	azul.color = 0;
	azul.capturado = azul.escapado = FALSE;
	azul.frame = 0;
	azul.dir = PUFFLE_DIR_0;
	azul.distancia = puffle_data[azul.color][0];
	azul.velocidad = puffle_data[azul.color][1];
	azul.x = 539;
	azul.y = 76;
	
	SDL_EventState (SDL_MOUSEMOTION, SDL_IGNORE);
	
	do {
		last_time = SDL_GetTicks ();
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_KEYDOWN:
					break;
			}
		}
		
		SDL_GetMouseState (&mousex, &mousey);
		
		if (!azul.escapado) {
			dx = mousex - azul.x;
			dy = mousey - azul.y;
			
			distancia = encontrar_distancia (dx, dy);
			
			if (distancia < azul.distancia) {
				incrementox = ((double) dx) / (distancia / ((double) azul.velocidad));
				incrementoy = ((double) dy) / (distancia / ((double) azul.velocidad));
				
				nextx = azul.x - incrementox;
				nexty = azul.y - incrementoy;
				
				azul.dir = encontrar_angulo_y_dir (mousex, mousey, azul.x, azul.y) + 8;
				azul.frame = 0;
				
				/* Colisión contra el muro */
				
				/* Colisión contra el interior de la jaula, si no, capturado = FALSE */
				
				
				azul.x = nextx;
				azul.y = nexty;
				
				/* Colisión de salida */
			}
		}
		
		if (azul.dir == PUFFLE_DIR_3 || azul.dir == PUFFLE_DIR_4 || azul.dir == PUFFLE_DIR_5) {
			//anim = 0;
			imagen = FACE_3_0 + (azul.dir - PUFFLE_DIR_3);
		} else {
			if (azul.dir / 8 == 1) {
				imagen = (azul.dir - PUFFLE_WALK_0) * 12 + puffle_frame_jump[azul.frame];
				
				azul.frame++;
				/* Secuencia de brinco, si llegamos al final pasar a la otra animación */
				if (puffle_frame_jump[azul.frame] == -1) {
					azul.frame = 0;
					azul.dir = azul.dir - 8;
				}
			} else {
				imagen = (azul.dir * 4) + puffle_frame_normal[azul.frame];
				if (azul.dir > PUFFLE_DIR_5) imagen -= 8;
				
				azul.frame++;
				if (puffle_frame_normal[azul.frame] == -1) {
					azul.frame = 0;
				}
			}
		}
		
		rect.x = azul.x - 14;
		rect.y = azul.y - 22;
		
		SDL_BlitSurface (images[IMG_FONDO], NULL, screen, NULL);
		
		copy_puffle_tile (IMG_PUFFLE_BLUE, &rect, imagen);
		
		SDL_Flip (screen);
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
		
	} while (!done);
	
	return done;
}

/* Set video mode: */
/* Mattias Engdegard <f91-men@nada.kth.se> */
SDL_Surface * set_video_mode (unsigned flags) {
	/* Prefer 16bpp, but also prefer native modes to emulated 16bpp. */

	int depth;

	depth = SDL_VideoModeOK (760, 480, 16, flags);
	return depth ? SDL_SetVideoMode (760, 480, depth, flags) : NULL;
}

void setup (void) {
	SDL_Surface * image;
	int g;
	
	/* Inicializar el Video SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf (stderr,
			"Error: Can't initialize the video subsystem\n"
			"The error returned by SDL is:\n"
			"%s\n", SDL_GetError());
		exit (1);
	}
	
	/* Crear la pantalla de dibujado */
	screen = set_video_mode (0);
	
	if (screen == NULL) {
		fprintf (stderr,
			"Error: Can't setup 760x480 video mode.\n"
			"The error returned by SDL is:\n"
			"%s\n", SDL_GetError());
		exit (1);
	}
	
	for (g = 0; g < NUM_IMAGES; g++) {
		image = IMG_Load (images_names[g]);
		
		if (image == NULL) {
			fprintf (stderr,
				"Failed to load data file:\n"
				"%s\n"
				"The error returned by SDL is:\n"
				"%s\n", images_names[g], SDL_GetError());
			SDL_Quit ();
			exit (1);
		}
		
		images[g] = image;
		/* TODO: Mostrar la carga de porcentaje */
	}
	
	srand (SDL_GetTicks ());
}

void copy_puffle_tile (int puffle_tiles, SDL_Rect *rect, int tile) {
	SDL_Rect r_tile;
	
	rect->w = r_tile.w = PUFFLE_TILE_WIDTH;
	rect->h = r_tile.h = PUFFLE_TILE_HEIGHT;
	r_tile.x = (tile % 8) * PUFFLE_TILE_WIDTH;
	r_tile.y = (tile / 8) * PUFFLE_TILE_HEIGHT;
	
	SDL_BlitSurface (images[puffle_tiles], &r_tile, screen, rect);
}

int encontrar_angulo_y_dir (int x1, int y1, int x2, int y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	
	int angulo = ((int) (atan2 (dy, dx) * 57.295780)) - 68; /* 90 - 22 */
	
	if (angulo < 0) {
		angulo += 360;
	}
	
	int dir = angulo / 45;
	
	if (dir > 7) {
		dir = 0;
	}
	
	return dir;
}

inline double encontrar_distancia (int dx, int dy) {
	return sqrt (dx * dx + dy * dy);
}

