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
#include <string.h>

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

/* X, Y donde los puffles realmente chocan. Invisible */
#define WALL_X 296
#define WALL_Y 248

/* X, Y de la imagen. No utilizada para las colisiones */
#define PUFFLE_PEN_X 301
#define PUFFLE_PEN_Y 241

/* Rectangulo de la zona "capturada". Zona invisible */
#define PEN_X 340
#define PEN_Y 271
#define PEN_W 80
#define PEN_H 97

/* Enumerar las imágenes */
enum {
	IMG_FONDO,
	
	IMG_INTRO,
	IMG_PUFFLE_INTRO,
	IMG_INTRO_CAGE,
	
	IMG_PUFFLE_PEN,
	
	IMG_PUFFLE_BLUE,
	IMG_PUFFLE_PINK,
	IMG_PUFFLE_GREEN,
	IMG_PUFFLE_BLACK,
	IMG_PUFFLE_PURPLE,
	
	IMG_WALL,
	
	IMG_CLOCK,
	
	NUM_IMAGES
};

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/fondo.png",
	
	GAMEDATA_DIR "images/intro.png",
	GAMEDATA_DIR "images/puffle-intro.png",
	GAMEDATA_DIR "images/title-cage.png",
	
	GAMEDATA_DIR "images/puffle-pen.png",
	
	GAMEDATA_DIR "images/puffle-azul.png",
	GAMEDATA_DIR "images/puffle-rosa.png",
	GAMEDATA_DIR "images/puffle-verde.png",
	GAMEDATA_DIR "images/puffle-negro.png",
	GAMEDATA_DIR "images/puffle-morado.png",
	
	GAMEDATA_DIR "images/wall.png",
	
	GAMEDATA_DIR "images/reloj.png"
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
	{45, 7},   /* Puffle puffles[g] */
	{55, 11},  /* Puffle Rosa */
	{90, 12},  /* Puffle Verde */
	{100, 9},  /* Puffle Negro */
	{100, 10}, /* Puffle Morado */
	{120, 10}, /* Puffle Rojo */
	{70, 7},   /* Puffle Amarillo */
	{40, 5},   /* Puffle Blanco */
	{110, 12}, /* Puffle Naranja */
	{110, 12}  /* Puffle Café */
};

/* Las posiciones del mapa */
const int map[8][10][2] = {
	{{383, 206}, {580, 332}, {180, 332}, {271,  76}, {200, 152}, {260, 352}, {540, 272}, {420, 132}, {460, 192}, {300, 192}},
	{{170, 119}, {142, 162}, {240, 127}, {246,  86}, {200, 152}, {558, 259}, {568, 192}, {281, 159}, {249, 235}, {207, 194}},
	{{197, 261}, {550, 389}, {227, 387}, {402, 188}, {156, 160}, {379, 395}, {578, 285}, {574, 197}, {532, 247}, {213, 241}},
	{{383, 123}, {580, 242}, {180, 252}, {319, 133}, {200, 242}, {260, 252}, {540, 252}, {420, 133}, {460, 123}, {300, 123}},
	{{409, 243}, {617, 406}, {364, 261}, {214, 118}, {321, 195}, {235, 258}, {562, 153}, {392, 150}, {460, 192}, {134, 192}},
	{{185, 166}, {160, 358}, {141, 184}, {545, 215}, {652, 291}, {567, 355}, {329, 161}, {262, 245}, {236, 115}, {466, 289}},
	{{258, 132}, {137, 122}, {303, 113}, {160, 285}, {534, 211}, {620, 148}, {191, 374}, {376, 103}, {395, 214}, {239, 211}},
	{{583, 260}, {611, 217}, {248, 171}, {507, 293}, {552, 227}, {194, 120}, {184, 187}, {547, 101}, {503, 144}, {545, 187}}
};

const int colores[5][10] = {
	{0, 1, 2, 9, 3, 8, 4, 5, 9, 7},
	{0, 1, 8, 8, 2, 3, 3, 4, 4, 4},
	{0, 0, 1, 2, 3, 4, 9, 5, 5, 5},
	{0, 1, 1, 2, 9, 9, 3, 8, 4, 5},
	{0, 0, 9, 1, 8, 2, 3, 3, 4, 6}
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
int game_intro (void);
int game_loop (void);
int game_score (int segundos, int capturados, int *total_coins);
void setup (void);
SDL_Surface * set_video_mode(unsigned);
void copy_puffle_tile (int puffle_tiles, SDL_Rect *rect, int tile);
double encontrar_distancia (int dx, int dy);
int encontrar_angulo_y_dir (int x1, int y1, int x2, int y2);
void acomodar_puffles (Puffle *puffles);

/* Variables globales */
SDL_Surface * screen;
SDL_Surface * images [NUM_IMAGES];

int main (int argc, char *argv[]) {
	
	setup ();
	
	do {
		if (game_intro () == GAME_QUIT) break;
		if (game_loop () == GAME_QUIT) break;
	} while (1 == 0);
	
	SDL_Quit ();
	return EXIT_SUCCESS;
}

int game_intro (void) {
	int done = 0;
	SDL_Event event;
	SDLKey key;
	SDL_Rect rect;
	Uint32 last_time, now_time;
	//int last_button = BUTTON_NONE, old_map = BUTTON_NONE, map;
	
	SDL_BlitSurface (images[IMG_FONDO], NULL, screen, NULL);
	
	/* Copiar la pantalla de introducción */
	rect.x = 204;
	rect.y = 57;
	rect.w = images[IMG_INTRO]->w;
	rect.h = images[IMG_INTRO]->h;
	
	SDL_BlitSurface (images[IMG_INTRO], NULL, screen, &rect);
	
	/* Copiar el puffle */
	rect.x = 242;
	rect.y = 156;
	rect.w = images[IMG_PUFFLE_INTRO]->w;
	rect.h = images[IMG_PUFFLE_INTRO]->h;
	
	SDL_BlitSurface (images[IMG_PUFFLE_INTRO], NULL, screen, &rect);
	
	/* Copiar la jaula */
	rect.x = 422;
	rect.y = 204;
	rect.w = images[IMG_INTRO_CAGE]->w;
	rect.h = images[IMG_INTRO_CAGE]->h;
	
	SDL_BlitSurface (images[IMG_INTRO_CAGE], NULL, screen, &rect);
	
	do {
		last_time = SDL_GetTicks ();
		
		while (SDL_PollEvent(&event) > 0) {
			/* fprintf (stdout, "Evento: %i\n", event.type);*/
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					/* Tengo un Mouse Down */
					break;
				case SDL_MOUSEBUTTONUP:
					/* Tengo un mouse Up */
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						done = GAME_QUIT;
					}
					if (event.key.keysym.sym == SDLK_c) {
						done = GAME_CONTINUE;
					}
				break;
			}
		}
		
		SDL_Flip (screen);
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
	} while (!done);
	
	return done;
}

int game_loop (void) {
	int done = 0;
	SDL_Event event;
	SDLKey key;
	Uint32 last_time, now_time, timer;
	SDL_Rect rect, rect2;
	int imagen;
	double distancia;
	int mousex, mousey, dx, dy;
	double incrementox, incrementoy;
	int nextx, nexty, g;
	Uint8 alpha, dummy;
	Uint32 *pixel;
	int capturados, escapados;
	int monedas = 0;
	
	Puffle puffles[10];
	
	acomodar_puffles (puffles);
	timer = SDL_GetTicks ();
	
	capturados = escapados = 0;
	
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
		
		SDL_BlitSurface (images[IMG_FONDO], NULL, screen, NULL);
		
		/*rect.x = WALL_X;
		rect.y = WALL_Y;
		rect.w = images[IMG_WALL]->w;
		rect.h = images[IMG_WALL]->h;
		
		SDL_BlitSurface (images[IMG_WALL], NULL, screen, &rect);*/
		
		rect.x = PUFFLE_PEN_X;
		rect.y = PUFFLE_PEN_Y;
		rect.w = images[IMG_PUFFLE_PEN]->w;
		rect.h = images[IMG_PUFFLE_PEN]->h;
		
		SDL_BlitSurface (images[IMG_PUFFLE_PEN], NULL, screen, &rect);
		
		/*rect.x = PEN_X;
		rect.y = PEN_Y;
		rect.w = images[IMG_PEN]->w;
		rect.h = images[IMG_PEN]->h;
		
		SDL_BlitSurface (images[IMG_PEN], NULL, screen, &rect);*/
		
		capturados = 0;
		for (g = 0; g < 10; g++) {
			if (!puffles[g].escapado) {
				dx = mousex - puffles[g].x;
				dy = mousey - puffles[g].y;
				
				distancia = encontrar_distancia (dx, dy);
			
				if (distancia < puffles[g].distancia) {
					if (distancia == 0.0) distancia = 0.1;
					
					incrementox = ((double) dx) / (distancia / ((double) puffles[g].velocidad));
					incrementoy = ((double) dy) / (distancia / ((double) puffles[g].velocidad));
					nextx = puffles[g].x - incrementox;
					nexty = puffles[g].y - incrementoy;
					
					if (puffles[g].dir / 8 == 0) puffles[g].frame = 0;
					puffles[g].dir = encontrar_angulo_y_dir (mousex, mousey, puffles[g].x, puffles[g].y) + 8;
					
					/* Colisión contra el muro */
					if (nextx >= WALL_X && nextx < WALL_X + images[IMG_WALL]->w && nexty >= WALL_Y && nexty < WALL_Y + images[IMG_WALL]->h) {
						pixel = images[IMG_WALL]->pixels + ((nexty - WALL_Y) * images[IMG_WALL]->pitch) + ((nextx - WALL_X) * images[IMG_WALL]->format->BytesPerPixel);
						SDL_GetRGBA (*pixel, images[IMG_WALL]->format, &dummy, &dummy, &dummy, &alpha);
						
						if (alpha == SDL_ALPHA_OPAQUE) {
							/* Colisión contra el muro, a rebotar */
							nextx = puffles[g].x + (incrementox * 1.5);
							nexty = puffles[g].y + (incrementoy * 1.5);
						}
					}
					
					/* Colisión contra el interior de la jaula, si no, capturado = FALSE */
					if (nextx >= PEN_X && nextx < PEN_X + PEN_W && nexty >= PEN_Y && nexty < PEN_Y + PEN_H) {
						if (!puffles[g].capturado) {
							puffles[g].capturado = TRUE;
							capturados++;
							/* TODO: Reproducir sonido de capturado */
						}
					} else {
						puffles[g].capturado = FALSE;
					}
					
					puffles[g].x = nextx;
					puffles[g].y = nexty;
					
					/* Colisión de salida */
					if (nextx < 80 || nextx > 680 || nexty < 43 || nexty > 443) {
						puffles[g].escapado = TRUE;
						escapados++;
						continue;
					}
				/*} else {
					if (puffles[g].dir - 8 >= 0) puffles[g].dir -= 8;*/
				}
				
				if (puffles[g].capturado) capturados++;
				
				/* Calcular la imagen para dibujar */
				if (puffles[g].dir == PUFFLE_DIR_3 || puffles[g].dir == PUFFLE_DIR_4 || puffles[g].dir == PUFFLE_DIR_5) {
					//anim = 0;
					imagen = FACE_3_0 + (puffles[g].dir - PUFFLE_DIR_3);
				} else {
					if (puffles[g].dir / 8 == 1) {
						imagen = (puffles[g].dir - PUFFLE_WALK_0) * 12 + puffle_frame_jump[puffles[g].frame];
						
						puffles[g].frame++;
						/* Secuencia de brinco, si llegamos al final pasar a la otra animación */
						if (puffle_frame_jump[puffles[g].frame] == -1) {
							puffles[g].frame = 0;
							puffles[g].dir = puffles[g].dir - 8;
						}
					} else {
						imagen = (puffles[g].dir * 4) + puffle_frame_normal[puffles[g].frame];
						if (puffles[g].dir > PUFFLE_DIR_5) imagen -= 8;
						
						puffles[g].frame++;
						if (puffle_frame_normal[puffles[g].frame] == -1) {
							puffles[g].frame = 0;
						}
					}
				}
				
				/* Redibujar el puffle */
				rect.x = puffles[g].x - 14;
				rect.y = puffles[g].y - 22;
				
				copy_puffle_tile (puffles[g].color, &rect, imagen);
			} /* Si no está escapado */
		} /* Foreach puffles */
		
		/* TODO: Actualizar textos */
		now_time = SDL_GetTicks ();
		g = 120 - (now_time - timer) / 1000;
		
		imagen = (120 - g) / 10;
		rect.x = 671;
		rect.y = 10;
		rect.h = rect.w = rect2.w = rect2.h = 42;
		
		rect2.x = imagen * 42;
		rect2.y = 0;
		
		SDL_BlitSurface (images[IMG_CLOCK], &rect2, screen, &rect);
		
		SDL_Flip (screen);
		
		if (capturados + escapados == 10 || g < 0) {
			/* Correr la pantalla de resultados */
			done = game_score (g, capturados, &monedas);
			if (done == GAME_CONTINUE) done = GAME_NONE;
			
			/* Regenerar los puffles */
			acomodar_puffles (puffles);
			timer = SDL_GetTicks ();
	
			capturados = escapados = 0;
		}
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
	} while (!done);
	
	return done;
}

int game_score (int segundos, int capturados, int *total_coins) {
	int done = 0;
	SDL_Event event;
	SDLKey key;
	SDL_Rect rect;
	Uint32 last_time, now_time;
	//int last_button = BUTTON_NONE, old_map = BUTTON_NONE, map;
	
	int score, coins;
	
	score = segundos * capturados;
	coins = score / 10;
	*total_coins = *total_coins + coins;
	
	SDL_BlitSurface (images[IMG_FONDO], NULL, screen, NULL);
	
	/* Copiar la pantalla de introducción */
	rect.x = 204;
	rect.y = 57;
	rect.w = images[IMG_INTRO]->w;
	rect.h = images[IMG_INTRO]->h;
	
	/* Dibujar los textos */
	SDL_BlitSurface (images[IMG_INTRO], NULL, screen, &rect);
	
	do {
		last_time = SDL_GetTicks ();
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					/* Tengo un Mouse Down */
					break;
				case SDL_MOUSEBUTTONUP:
					/* Tengo un mouse Up */
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						done = GAME_QUIT;
					}
					if (event.key.keysym.sym == SDLK_c) {
						done = GAME_CONTINUE;
					}
					if (event.key.keysym.sym == SDLK_f) {
						done = GAME_QUIT;
					}
				break;
			}
		}
		
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

void acomodar_puffles (Puffle *puffles) {
	int g, r;
	int paquete[10] = {0, 0, 1, 1, 2, 2, 0, 1, 2, 0};
	int sel_mapa;
	const int (*local_mapa)[2];
	
	r = RANDOM (5);
	sel_mapa = RANDOM (8);
	local_mapa = map[sel_mapa];
	
	/* TODO: Seleccionar un paquete de colores */
	//memcpy (paquete, colores[r], sizeof (paquete));
	
	for (g = 0; g < 20; g++) {
		r = RANDOM (9) + 1;
		//printf ("Intercambiando %i por %i\n", 0, r);
		sel_mapa = paquete[0];
		paquete[0] = paquete[r];
		paquete[r] = sel_mapa;
	}
	
	for (g = 0; g < 10; g++) {
		puffles[g].color = IMG_PUFFLE_BLUE + paquete [g];
		puffles[g].capturado = puffles[g].escapado = FALSE;
		puffles[g].frame = 0;
		puffles[g].dir = PUFFLE_DIR_0;
		puffles[g].distancia = puffle_data[paquete[g]][0];
		puffles[g].velocidad = puffle_data[paquete[g]][1];
		puffles[g].x = local_mapa[g][0];
		puffles[g].y = local_mapa[g][1];
	}
}
