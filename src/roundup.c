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
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <locale.h>
#include "gettext.h"
#define _(string) gettext (string)

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cp-button.h"
#include "draw-text.h"

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
	
	IMG_BUTTON_CLOSE_UP,
	IMG_BUTTON_CLOSE_OVER,
	IMG_BUTTON_CLOSE_DOWN,
	
	IMG_PUFFLE_BLUE,
	IMG_PUFFLE_PINK,
	IMG_PUFFLE_GREEN,
	IMG_PUFFLE_BLACK,
	IMG_PUFFLE_PURPLE,
	IMG_PUFFLE_RED,
	IMG_PUFFLE_YELLOW,
	IMG_PUFFLE_WHITE,
	IMG_PUFFLE_ORANGE,
	IMG_PUFFLE_BROWN,
	
	IMG_WALL,
	
	IMG_CLOCK,
	
	NUM_IMAGES
};

#ifdef __MINGW32__
#	undef GAMEDATA_DIR
#	define GAMEDATA_DIR "./"
#endif

const char *images_names[NUM_IMAGES] = {
	GAMEDATA_DIR "images/fondo.png",
	
	GAMEDATA_DIR "images/intro.png",
	GAMEDATA_DIR "images/puffle-intro.png",
	GAMEDATA_DIR "images/title-cage.png",
	
	GAMEDATA_DIR "images/puffle-pen.png",
	
	GAMEDATA_DIR "images/boton-close-up.png",
	GAMEDATA_DIR "images/boton-close-over.png",
	GAMEDATA_DIR "images/boton-close-down.png",
	
	GAMEDATA_DIR "images/puffle-azul.png",
	GAMEDATA_DIR "images/puffle-rosa.png",
	GAMEDATA_DIR "images/puffle-verde.png",
	GAMEDATA_DIR "images/puffle-negro.png",
	GAMEDATA_DIR "images/puffle-morado.png",
	GAMEDATA_DIR "images/puffle-rojo.png",
	GAMEDATA_DIR "images/puffle-amarillo.png",
	GAMEDATA_DIR "images/puffle-blanco.png",
	GAMEDATA_DIR "images/puffle-naranja.png",
	GAMEDATA_DIR "images/puffle-cafe.png",
	
	GAMEDATA_DIR "images/wall.png",
	
	GAMEDATA_DIR "images/reloj.png"
};

enum {
	SND_CAPTURE_1,
	SND_CAPTURE_2,
	
	SND_ESCAPE,
	
	NUM_SOUNDS
};

const char *sound_names[NUM_SOUNDS] = {
	GAMEDATA_DIR "sounds/capture1.wav",
	GAMEDATA_DIR "sounds/capture2.wav",
	
	GAMEDATA_DIR "sounds/escape.wav",
};

#define MUS_ROUNDUP_1 GAMEDATA_DIR "music/roundup1.ogg"
#define MUS_ROUNDUP_2 GAMEDATA_DIR "music/roundup2.ogg"

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
int puffle_frame_normal [10][65];

/* Para el motor de botones */
enum {
	BUTTON_NONE,
	
	BUTTON_CLOSE,
	BUTTON_UI_START,
	BUTTON_UI_PLAY_MORE,
	BUTTON_UI_FINISH,
	
	NUM_BUTTONS
};

enum {
	BLANK_UP,
	BLANK_OVER,
	BLANK_DOWN
};

/* Codigos de salida */
enum {
	GAME_NONE = 0, /* No usado */
	GAME_CONTINUE,
	GAME_QUIT
};

enum {
	TEXT_TITLE,
	TEXT_PLAY_TITLE,
	TEXT_INSTRUCTIONS,
	TEXT_INSTRUCTIONS_2,
	TEXT_SCORING,
	TEXT_SCORING_2,
	TEXT_PLAY_2,
	TEXT_CAUGHT,
	TEXT_ESCAPED,
	TEXT_SCORE,
	TEXT_PLAYMORE,
	TEXT_FINISH,
	
	NUM_TEXTS
};

/* La meta información de los puffles */
const int puffle_data[10][2] = { /* {distancia, velocidad} */
	{45, 7},   /* Puffle Azul */
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
inline int map_button_in_opening (int x, int y);
inline int map_button_in_game (int x, int y);
inline int map_button_in_score (int x, int y);

/* Variables globales */
SDL_Surface * screen;
SDL_Surface * images [NUM_IMAGES];
SDL_Surface *texts [NUM_TEXTS];

int use_sound;
Mix_Chunk * sounds[NUM_SOUNDS];
Mix_Music * music[2];

TTF_Font *ttf14_outline, *ttf14_normal;
TTF_Font *ttf12_normal;

int main (int argc, char *argv[]) {
	/* Inicializar l18n */
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, LOCALEDIR);

	textdomain (PACKAGE);
	
	setup ();
	
	cp_registrar_botones (NUM_BUTTONS);
	cp_registrar_boton (BUTTON_CLOSE, IMG_BUTTON_CLOSE_UP);
	cp_registrar_boton (BUTTON_UI_START, BLANK_UP);
	cp_registrar_boton (BUTTON_UI_PLAY_MORE, BLANK_UP);
	cp_registrar_boton (BUTTON_UI_FINISH, BLANK_UP);
	cp_button_start ();
	
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
	int map;
	Uint32 color;
	SDL_Surface *trans;
	
	color = SDL_MapRGB (screen->format, 255, 255, 255);
	trans = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA, 138, 37, 32, 0, 0, 0, 0);
	SDL_FillRect (trans, NULL, color); /* Blanco */
	SDL_SetAlpha (trans, SDL_SRCALPHA, 128); /* Alpha al 50 % */
	
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
	
	/* El botón de cierre */
	rect.x = 720;
	rect.y = 12;
	rect.w = images[IMG_BUTTON_CLOSE_UP]->w;
	rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
	SDL_BlitSurface (images[IMG_BUTTON_CLOSE_UP], NULL, screen, &rect);
	
	/* Copiar el titulo del juego */
	rect.w = texts[TEXT_TITLE]->w;
	rect.x = 231 + (296 - rect.w) / 2;
	rect.y = 83;
	rect.h = texts[TEXT_TITLE]->h;
	SDL_BlitSurface (texts[TEXT_TITLE], NULL, screen, &rect);
	
	/* El texto de play del botón */
	rect.w = texts[TEXT_PLAY_TITLE]->w;
	rect.x = 392 + (134 - rect.w) / 2;
	rect.y = 279;
	rect.h = texts[TEXT_PLAY_TITLE]->h;
	SDL_BlitSurface (texts[TEXT_PLAY_TITLE], NULL, screen, &rect);
	
	Mix_VolumeMusic (MIX_MAX_VOLUME / 5); /* 20% */
	Mix_PlayMusic (music[0], 1);
	
	do {
		last_time = SDL_GetTicks ();
		
		if (!Mix_PlayingMusic ()) {
			map = RANDOM (2);
			Mix_PlayMusic (music[map], 1);
		}
		
		while (SDL_PollEvent(&event) > 0) {
			/* fprintf (stdout, "Evento: %i\n", event.type);*/
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEMOTION:
					map = map_button_in_opening (event.motion.x, event.motion.y);
					cp_button_motion (map);
					break;
				case SDL_MOUSEBUTTONDOWN:
					map = map_button_in_opening (event.button.x, event.button.y);
					cp_button_down (map);
					if (map == BUTTON_UI_START) {
						//if (use_sound) Mix_PlayChannel (-1, sounds[SND_BUTTON], 0);
					}
					break;
				case SDL_MOUSEBUTTONUP:
					map = map_button_in_opening (event.button.x, event.button.y);
					map = cp_button_up (map);
					
					switch (map) {
						case BUTTON_UI_START:
							done = GAME_CONTINUE;
							break;
						case BUTTON_CLOSE:
							done = GAME_QUIT;
							break;
					}
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
		
		if (cp_button_refresh[BUTTON_CLOSE]) {
			rect.x = 720;
			rect.y = 12;
			rect.w = images[IMG_BUTTON_CLOSE_UP]->w;
			rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
			
			SDL_BlitSurface (images[IMG_FONDO], &rect, screen, &rect);
			
			SDL_BlitSurface (images[cp_button_frames[BUTTON_CLOSE]], NULL, screen, &rect);
			cp_button_refresh[BUTTON_CLOSE] = 0;
		}
		
		if (cp_button_refresh[BUTTON_UI_START]) {
			/* Borrar el fondo con blanco */
			rect.x = 388;
			rect.y = 277;
			rect.w = 138;
			rect.h = 37;
			
			SDL_FillRect (screen, &rect, color);
			
			/* El texto de play del botón */
			rect.w = texts[TEXT_PLAY_TITLE]->w;
			rect.x = 392 + (134 - rect.w) / 2;
			rect.y = 279;
			rect.h = texts[TEXT_PLAY_TITLE]->h;
			SDL_BlitSurface (texts[TEXT_PLAY_TITLE], NULL, screen, &rect);
			
			if (cp_button_frames[BUTTON_UI_START] != BLANK_UP) {
				rect.x = 388;
				rect.y = 277;
				rect.w = 138;
				rect.h = 37;
				
				SDL_BlitSurface (trans, NULL, screen, &rect);
			}
		}
		SDL_Flip (screen);
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
	} while (!done);
	
	SDL_FreeSurface (trans);
	return done;
}

int game_loop (void) {
	int done = 0;
	SDL_Event event;
	SDLKey key;
	Uint32 last_time, now_time, timer;
	SDL_Rect rect, rect2;
	int map;
	int imagen;
	double distancia;
	int mousex, mousey, dx, dy;
	double incrementox, incrementoy;
	int nextx, nexty, g, h, i;
	Uint8 alpha, dummy;
	Uint32 *pixel;
	int capturados, escapados;
	int monedas = 0;
	char buf[10];
	SDL_Surface *text;
	SDL_Color blanco = {255, 255, 255, 0}, negro = {0, 0, 0, 0};
	
	Puffle puffles[10];
	Puffle *ordenados[10];
	
	acomodar_puffles (puffles);
	timer = SDL_GetTicks ();
	
	capturados = escapados = 0;
	
	SDL_EventState (SDL_MOUSEMOTION, SDL_IGNORE);
	
	do {
		last_time = SDL_GetTicks ();
		
		if (!Mix_PlayingMusic ()) {
			map = RANDOM (2);
			Mix_PlayMusic (music[map], 1);
		}
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEBUTTONDOWN:
					map = map_button_in_game (event.button.x, event.button.y);
					cp_button_down (map);
					break;
				case SDL_MOUSEBUTTONUP:
					map = map_button_in_game (event.button.x, event.button.y);
					map = cp_button_up (map);
					
					switch (map) {
						case BUTTON_CLOSE:
							done = GAME_QUIT;
							break;
					}
					break;
				case SDL_KEYDOWN:
					break;
			}
		}
		
		SDL_GetMouseState (&mousex, &mousey);
		
		map = map_button_in_game (mousex, mousey);
		cp_button_motion (map);
		
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
		
		/* Imprimir los textos de capturado y escapados */
		rect.w = texts[TEXT_CAUGHT]->w;
		rect.h = texts[TEXT_CAUGHT]->h;
		rect.x = 4 + (151 - rect.w);
		rect.y = 8;
		SDL_BlitSurface (texts[TEXT_CAUGHT], NULL, screen, &rect);
		
		rect.w = texts[TEXT_ESCAPED]->w;
		rect.h = texts[TEXT_ESCAPED]->h;
		rect.x = 4 + (151 - rect.w);
		rect.y = 28;
		SDL_BlitSurface (texts[TEXT_ESCAPED], NULL, screen, &rect);
		
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
					if (puffles[g].color < 8) {
						puffles[g].dir = encontrar_angulo_y_dir (mousex, mousey, puffles[g].x, puffles[g].y) + 8;
					} else {
						puffles[g].dir = encontrar_angulo_y_dir (mousex, mousey, puffles[g].x, puffles[g].y);
					}
					
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
							imagen = RANDOM (2);
							if (use_sound) Mix_PlayChannel (-1, sounds[SND_CAPTURE_1 + imagen], 0);
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
						if (use_sound) Mix_PlayChannel (-1, sounds[SND_ESCAPE], 0);
					}
				/*} else {
					if (puffles[g].dir - 8 >= 0) puffles[g].dir -= 8;*/
				}
				
				if (puffles[g].capturado) capturados++;
			} /* Si no está escapado */
			/* Insertar de forma ordenada */
			for (h = 0; h < g; h++) {
				if (ordenados[h]->y > puffles[g].y) break;
			}
			for (i = g; i > h; i--) {
				ordenados[i] = ordenados[i - 1];
			}
			ordenados[h] = &puffles[g];
		} /* Foreach puffles */
		
		for (g = 0; g < 10; g++) {
			if (!ordenados[g]->escapado) {
				/* Calcular la imagen para dibujar */
				if (ordenados[g]->dir == PUFFLE_DIR_3 || ordenados[g]->dir == PUFFLE_DIR_4 || ordenados[g]->dir == PUFFLE_DIR_5) {
					//anim = 0;
					imagen = FACE_3_0 + (ordenados[g]->dir - PUFFLE_DIR_3);
				} else {
					if (ordenados[g]->dir / 8 == 1) {
						imagen = (ordenados[g]->dir - PUFFLE_WALK_0) * 12 + FACE_J0_0 + ordenados[g]->frame;
						
						ordenados[g]->frame++;
						/* Secuencia de brinco, si llegamos al final pasar a la otra animación */
						if (ordenados[g]->color < 3 && ordenados[g]->frame == 12) {
							ordenados[g]->frame = 0;
							ordenados[g]->dir = ordenados[g]->dir - 8;
						} else if (ordenados[g]->color > 2 && ordenados[g]->frame == 8) {
							ordenados[g]->frame = 0;
							ordenados[g]->dir = ordenados[g]->dir - 8;
						}
					} else {
						imagen = (ordenados[g]->dir * 4) + puffle_frame_normal[ordenados[g]->color][ordenados[g]->frame];
						if (ordenados[g]->dir > PUFFLE_DIR_5) imagen -= 8;
						
						ordenados[g]->frame++;
						if (puffle_frame_normal[ordenados[g]->color][ordenados[g]->frame] == -1) {
							ordenados[g]->frame = 0;
						}
					}
				}
				
				/* Redibujar el puffle */
				rect.x = ordenados[g]->x - 14;
				rect.y = ordenados[g]->y - 22;
				
				copy_puffle_tile (IMG_PUFFLE_BLUE + ordenados[g]->color, &rect, imagen);
			}
		}
		
		/* Redibujar el botón de cierre */
		rect.x = 720;
		rect.y = 12;
		rect.w = images[IMG_BUTTON_CLOSE_UP]->w;
		rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
		
		SDL_BlitSurface (images[cp_button_frames[BUTTON_CLOSE]], NULL, screen, &rect);
		
		/* Actualizar los textos de capturados y escapados */
		sprintf (buf, "%i", capturados);
		text = draw_text_with_shadow (ttf14_normal, 2, buf, &blanco, &negro);
		
		rect.x = 156;
		rect.y = 8;
		rect.w = text->w;
		rect.h = text->h;
		SDL_BlitSurface (text, NULL, screen, &rect);
		
		SDL_FreeSurface (text);
		
		sprintf (buf, "%i", escapados);
		text = draw_text_with_shadow (ttf14_normal, 2, buf, &blanco, &negro);
		
		rect.x = 156;
		rect.y = 28;
		rect.w = text->w;
		rect.h = text->h;
		SDL_BlitSurface (text, NULL, screen, &rect);
		
		SDL_FreeSurface (text);
		
		/* Actualizar el reloj */
		now_time = SDL_GetTicks ();
		g = 120 - (now_time - timer) / 1000;
		
		imagen = (120 - g) / 10;
		rect.x = 671;
		rect.y = 10;
		rect.h = rect.w = rect2.w = rect2.h = 42;
		
		rect2.x = imagen * 42;
		rect2.y = 0;
		
		SDL_BlitSurface (images[IMG_CLOCK], &rect2, screen, &rect);
		
		/* Reimprimir los segundos restantes */
		sprintf (buf, "%i", g);
		text = TTF_RenderUTF8_Blended (ttf14_normal, buf, negro);
		
		rect.y = 24;
		rect.x = 692 - (text->w / 2);
		rect.w = text->w;
		rect.h = text->h;
		SDL_BlitSurface (text, NULL, screen, &rect);
		
		SDL_FreeSurface (text);
		
		SDL_Flip (screen);
		
		if (capturados + escapados == 10 || g <= 0) {
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
	SDL_Rect rect, rect2;
	Uint32 last_time, now_time;
	int map;
	char buf[10];
	SDL_Surface *text;
	SDL_Color negro = {0, 0, 0, 0};
	static Uint32 color;
	static SDL_Surface *trans = NULL;
	int score, coins;
	
	if (trans == NULL) {
		color = SDL_MapRGB (screen->format, 255, 255, 255);
		trans = SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA, 155, 34, 32, 0, 0, 0, 0);
		SDL_FillRect (trans, NULL, color); /* Blanco */
		SDL_SetAlpha (trans, SDL_SRCALPHA, 128); /* Alpha al 50 % */
	}
	
	score = segundos * capturados;
	coins = score / 10;
	*total_coins = *total_coins + coins;
	
	SDL_BlitSurface (images[IMG_FONDO], NULL, screen, NULL);
	
	/* Copiar la pantalla de introducción */
	rect.x = 204;
	rect.y = 57;
	rect.w = images[IMG_INTRO]->w;
	rect.h = images[IMG_INTRO]->h;
	
	SDL_BlitSurface (images[IMG_INTRO], NULL, screen, &rect);
	
	/* El botón de cierre */
	rect.x = 720;
	rect.y = 12;
	rect.w = images[IMG_BUTTON_CLOSE_UP]->w;
	rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
	SDL_BlitSurface (images[IMG_BUTTON_CLOSE_UP], NULL, screen, &rect);
	
	/* Dibujar los textos */
	rect.w = texts[TEXT_SCORE]->w;
	rect.h = texts[TEXT_SCORE]->h;
	rect.x = 246 + (269 - rect.w) / 2;
	rect.y = 89;
	SDL_BlitSurface (texts[TEXT_SCORE], NULL, screen, &rect);
	
	/* Segundos restantes */
	sprintf (buf, _("%i SECONDS LEFT"), segundos);
	text = TTF_RenderUTF8_Blended (ttf14_normal, buf, negro);
	
	rect.y = 126;
	rect.x = 232 + (295 - text->w) / 2;
	rect.w = text->w;
	rect.h = text->h;
	SDL_BlitSurface (text, NULL, screen, &rect);
	
	SDL_FreeSurface (text);
	
	/* La "x" */
	text = TTF_RenderUTF8_Blended (ttf12_normal, "X", negro);
	
	rect.y = 142;
	rect.x = 232 + (295 - text->w) / 2;
	rect.w = text->w;
	rect.h = text->h;
	SDL_BlitSurface (text, NULL, screen, &rect);
	
	SDL_FreeSurface (text);
	
	/* Puffles capturados */
	sprintf (buf, _("%i PUFFLES CAUGHT ="), capturados);
	text = TTF_RenderUTF8_Blended (ttf14_normal, buf, negro);
	
	rect.y = 159;
	rect.x = 232 + (295 - text->w) / 2;
	rect.w = text->w;
	rect.h = text->h;
	SDL_BlitSurface (text, NULL, screen, &rect);
	
	SDL_FreeSurface (text);
	
	/* Total score */
	sprintf (buf, _("TOTAL SCORE: %i"), score);
	text = TTF_RenderUTF8_Blended (ttf12_normal, buf, negro);
	
	rect.y = 201;
	rect.x = 232 + (295 - text->w) / 2;
	rect.w = text->w;
	rect.h = text->h;
	SDL_BlitSurface (text, NULL, screen, &rect);
	
	SDL_FreeSurface (text);
	
	/* Monedas en este round */
	sprintf (buf, _("COINS THIS ROUND: %i"), coins);
	text = TTF_RenderUTF8_Blended (ttf12_normal, buf, negro);
	
	rect.y = 220;
	rect.x = 232 + (295 - text->w) / 2;
	rect.w = text->w;
	rect.h = text->h;
	SDL_BlitSurface (text, NULL, screen, &rect);
	
	SDL_FreeSurface (text);
	
	/* Monedas totales */
	sprintf (buf, _("TOTAL COINS: %i"), *total_coins);
	text = TTF_RenderUTF8_Blended (ttf14_normal, buf, negro);
	
	rect.y = 246;
	rect.x = 232 + (295 - text->w) / 2;
	rect.w = text->w;
	rect.h = text->h;
	SDL_BlitSurface (text, NULL, screen, &rect);
	
	SDL_FreeSurface (text);
	
	/* El texto de play del botón */
	rect.w = texts[TEXT_PLAYMORE]->w;
	rect.x = 235 + (155 - rect.w) / 2;
	rect.y = 289;
	rect.h = texts[TEXT_PLAYMORE]->h;
	SDL_BlitSurface (texts[TEXT_PLAYMORE], NULL, screen, &rect);
	
	rect.w = texts[TEXT_FINISH]->w;
	rect.x = 393 + (131 - rect.w) / 2;
	rect.y = 289;
	rect.h = texts[TEXT_FINISH]->h;
	SDL_BlitSurface (texts[TEXT_FINISH], NULL, screen, &rect);
	
	SDL_EventState (SDL_MOUSEMOTION, SDL_ENABLE);
	
	do {
		last_time = SDL_GetTicks ();
		
		if (!Mix_PlayingMusic ()) {
			map = RANDOM (2);
			Mix_PlayMusic (music[map], 1);
		}
		
		while (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
				case SDL_QUIT:
					/* Vamos a cerrar la aplicación */
					done = GAME_QUIT;
					break;
				case SDL_MOUSEMOTION:
					map = map_button_in_score (event.motion.x, event.motion.y);
					cp_button_motion (map);
					break;
				case SDL_MOUSEBUTTONDOWN:
					map = map_button_in_score (event.button.x, event.button.y);
					cp_button_down (map);
					if (map == BUTTON_UI_PLAY_MORE) {
						//if (use_sound) Mix_PlayChannel (-1, sounds[SND_BUTTON], 0);
					}
					
					if (map == BUTTON_UI_FINISH) {
						
					}
					break;
				case SDL_MOUSEBUTTONUP:
					map = map_button_in_score (event.button.x, event.button.y);
					map = cp_button_up (map);
					
					switch (map) {
						case BUTTON_UI_PLAY_MORE:
							done = GAME_CONTINUE;
							break;
						case BUTTON_UI_FINISH:
							done = GAME_QUIT;
							break;
						case BUTTON_CLOSE:
							done = GAME_QUIT;
							break;
					}
					break;
			}
		}
		
		if (cp_button_refresh[BUTTON_CLOSE]) {
			rect.x = 720;
			rect.y = 12;
			rect.w = images[IMG_BUTTON_CLOSE_UP]->w;
			rect.h = images[IMG_BUTTON_CLOSE_UP]->h;
			
			SDL_BlitSurface (images[IMG_FONDO], &rect, screen, &rect);
			
			SDL_BlitSurface (images[cp_button_frames[BUTTON_CLOSE]], NULL, screen, &rect);
			cp_button_refresh[BUTTON_CLOSE] = 0;
		}
		
		if (cp_button_refresh[BUTTON_UI_PLAY_MORE]) {
			/* Borrar el fondo con blanco */
			rect.x = 235;
			rect.y = 285;
			rect.w = 155;
			rect.h = 34;
			
			SDL_FillRect (screen, &rect, color);
			
			/* El texto de play del botón */
			rect.w = texts[TEXT_PLAYMORE]->w;
			rect.x = 235 + (155 - rect.w) / 2;
			rect.y = 289;
			rect.h = texts[TEXT_PLAYMORE]->h;
			SDL_BlitSurface (texts[TEXT_PLAYMORE], NULL, screen, &rect);
			
			if (cp_button_frames[BUTTON_UI_PLAY_MORE] != BLANK_UP) {
				rect.x = 235;
				rect.y = 285;
				rect.w = 155;
				rect.h = 34;
				
				SDL_BlitSurface (trans, NULL, screen, &rect);
			}
			cp_button_refresh[BUTTON_UI_PLAY_MORE] = 0;
		}
		
		if (cp_button_refresh[BUTTON_UI_FINISH]) {
			/* Borrar el fondo con blanco */
			rect.x = 393;
			rect.y = 285;
			rect.w = 131;
			rect.h = 34;
			
			SDL_FillRect (screen, &rect, color);
			
			/* El texto de play del botón */
			rect.w = texts[TEXT_FINISH]->w;
			rect.x = 393 + (131 - rect.w) / 2;
			rect.y = 289;
			rect.h = texts[TEXT_FINISH]->h;
			SDL_BlitSurface (texts[TEXT_FINISH], NULL, screen, &rect);
			
			if (cp_button_frames[BUTTON_UI_FINISH] != BLANK_UP) {
				rect.x = 393;
				rect.y = 285;
				rect2.x = rect2.y = 0;
				rect.w = rect2.w = 131;
				rect.h = rect2.h = 34;
				
				SDL_BlitSurface (trans, &rect2, screen, &rect);
			}
			cp_button_refresh[BUTTON_UI_FINISH] = 0;
		}
		
		SDL_Flip (screen);
		
		now_time = SDL_GetTicks ();
		if (now_time < last_time + FPS) SDL_Delay(last_time + FPS - now_time);
	} while (!done);
	
	SDL_EventState (SDL_MOUSEMOTION, SDL_IGNORE);
	
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
	TTF_Font *font_normal;
	SDL_Color blanco = {255, 255, 255, 0}, negro = {0, 0, 0, 0}, otro;
	
	/* Inicializar el Video SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf (stderr,
			_("Error: Can't initialize the video subsystem\n"
			"The error returned by SDL is:\n"
			"%s\n"), SDL_GetError());
		exit (1);
	}
	
	image = IMG_Load (GAMEDATA_DIR "images/icon.png");
	
	if (image) {
		SDL_WM_SetIcon (image, NULL);
		SDL_FreeSurface (image);
	}
	
	SDL_WM_SetCaption ("Puffle Round UP", "Puffle Round UP");
	
	/* Crear la pantalla de dibujado */
	screen = set_video_mode (0);
	
	if (screen == NULL) {
		fprintf (stderr,
			_("Error: Can't setup 760x480 video mode.\n"
			"The error returned by SDL is:\n"
			"%s\n"), SDL_GetError());
		exit (1);
	}
	
	use_sound = 1;
	if (SDL_InitSubSystem (SDL_INIT_AUDIO) < 0) {
		fprintf (stdout,
			"Warning: Can't initialize the audio subsystem\n"
			"Continuing...\n");
		use_sound = 0;
	}
	
	if (use_sound) {
		/* Inicializar el sonido */
		if (Mix_OpenAudio (22050, AUDIO_S16, 2, 4096) < 0) {
			fprintf (stdout,
				"Warning: Can't initialize the SDL Mixer library\n");
			use_sound = 0;
		} else {
			Mix_AllocateChannels (3);
		}
	}
	
	for (g = 0; g < NUM_IMAGES; g++) {
		image = IMG_Load (images_names[g]);
		
		if (image == NULL) {
			fprintf (stderr,
				_("Failed to load data file:\n"
				"%s\n"
				"The error returned by SDL is:\n"
				"%s\n"), images_names[g], SDL_GetError());
			SDL_Quit ();
			exit (1);
		}
		
		images[g] = image;
		/* TODO: Mostrar la carga de porcentaje */
	}
	
	if (use_sound) {
		for (g = 0; g < NUM_SOUNDS; g++) {
			sounds[g] = Mix_LoadWAV (sound_names [g]);
			
			if (sounds[g] == NULL) {
				fprintf (stderr,
					_("Failed to load data file:\n"
					"%s\n"
					"The error returned by SDL is:\n"
					"%s\n"), sound_names [g], SDL_GetError ());
				SDL_Quit ();
				exit (1);
			}
			Mix_VolumeChunk (sounds[g], MIX_MAX_VOLUME / 2);
		}
		
		/* Cargar las músicas de fondo */
		music[0] = Mix_LoadMUS (MUS_ROUNDUP_1);
		music[1] = Mix_LoadMUS (MUS_ROUNDUP_2);
		
		if (music[0] == NULL || music[1] == NULL) {
			fprintf (stderr,
				_("Failed to load a music file.\n"
				"The error returned by SDL is:\n"
				"%s\n"), SDL_GetError ());
			SDL_Quit ();
			exit (1);
		}
	}
	
	if (TTF_Init () < 0) {
		fprintf (stderr,
			_("Error: Can't initialize the SDL TTF library\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	/* Generar todos los textos */
	font_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 28);
	
	if (!font_normal) {
		fprintf (stderr,
			_("Failed to load font file 'CCFaceFront'\n"
			"The error returned by SDL is:\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	TTF_SetFontStyle (font_normal, TTF_STYLE_ITALIC);
	
	texts [TEXT_TITLE] = draw_text_with_shadow (font_normal, 3, _("PUFFLE ROUNDUP"), &blanco, &negro);
	
	TTF_CloseFont (font_normal);
	
	/* Botón play de 22 puntos en la pantalla de bienvenida */
	font_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 22);
	
	if (!font_normal) {
		fprintf (stderr,
			_("Failed to load font file 'CCFaceFront'\n"
			"The error returned by SDL is:\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	TTF_SetFontStyle (font_normal, TTF_STYLE_ITALIC);
	
	otro.r = 0xFF; otro.g = 0xCC; otro.b = 0;
	texts [TEXT_PLAY_TITLE] = draw_text_with_shadow (font_normal, 2, _("PLAY"), &otro, &negro);
	
	TTF_CloseFont (font_normal);
	
	/* Textos de las instrucciones */
	font_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 20);
	
	if (!font_normal) {
		fprintf (stderr,
			_("Failed to load font file 'CCFaceFront'\n"
			"The error returned by SDL is:\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	TTF_SetFontStyle (font_normal, TTF_STYLE_ITALIC);
	
	texts [TEXT_INSTRUCTIONS] = draw_text_with_shadow (font_normal, 2, _("INSTRUCTIONS:"), &blanco, &negro);
	texts [TEXT_SCORING] = draw_text_with_shadow (font_normal, 2, _("SCORING:"), &blanco, &negro);
	
	TTF_CloseFont (font_normal);
	
	/* Boton PLAY de 24 puntos en la pantalla de instrucciones */
	font_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 24);
	
	if (!font_normal) {
		fprintf (stderr,
			_("Failed to load font file 'CCFaceFront'\n"
			"The error returned by SDL is:\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	TTF_SetFontStyle (font_normal, TTF_STYLE_ITALIC);
	
	otro.r = 0xFF; otro.g = 0xCC; otro.b = 0;
	texts [TEXT_PLAY_2] = draw_text_with_shadow (font_normal, 3, _("PLAY"), &otro, &negro);
	texts [TEXT_SCORE] = draw_text_with_shadow (font_normal, 3, _("SCORE:"), &blanco, &negro);
	
	TTF_CloseFont (font_normal);
	
	/* Boton PLAY de 16 puntos en la pantalla de instrucciones */
	font_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 16);
	
	if (!font_normal) {
		fprintf (stderr,
			_("Failed to load font file 'CCFaceFront'\n"
			"The error returned by SDL is:\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	TTF_SetFontStyle (font_normal, TTF_STYLE_ITALIC);
	
	otro.r = 0xFF; otro.g = 0xCC; otro.b = 0;
	texts [TEXT_PLAYMORE] = draw_text_with_shadow (font_normal, 2, _("PLAY MORE"), &otro, &negro);
	otro.r = 0x66; otro.g = 0xCC; otro.b = 0xFF;
	texts [TEXT_FINISH] = draw_text_with_shadow (font_normal, 2, _("FINISH"), &otro, &negro);
	
	TTF_CloseFont (font_normal);
	
	ttf14_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 14);
	ttf12_normal = TTF_OpenFont (GAMEDATA_DIR "ccfacefront.ttf", 12);
	
	if (!ttf14_normal || !ttf12_normal) {
		fprintf (stderr,
			_("Failed to load font file 'CCFaceFront'\n"
			"The error returned by SDL is:\n"
			"%s\n"), TTF_GetError ());
		SDL_Quit ();
		exit (1);
	}
	
	TTF_SetFontStyle (ttf14_normal, TTF_STYLE_ITALIC);
	TTF_SetFontStyle (ttf12_normal, TTF_STYLE_ITALIC);
	
	texts [TEXT_CAUGHT] = draw_text_with_shadow (ttf14_normal, 2, _("CAUGHT:"), &blanco, &negro);
	texts [TEXT_ESCAPED] = draw_text_with_shadow (ttf14_normal, 2, _("ESCAPED:"), &blanco, &negro);
	
	/* No se cierran las tipografías porque se usan después */
	
	/* Parte de la inicialización es inicializar el arreglo de animación */
	for (g = 0; g < 46; g++) {
		puffle_frame_normal[0][g] = puffle_frame_normal[1][g] = puffle_frame_normal[2][g] = FACE_0_0;
	}
	
	for (g = 0; g < 53; g++) {
		puffle_frame_normal[3][g] = FACE_0_0;
	}
	
	for (g = 0; g < 41; g++) {
		puffle_frame_normal[4][g] = FACE_0_0;
	}
	
	for (g = 0; g < 56; g++) {
		puffle_frame_normal[5][g] = FACE_0_0;
	}
	
	for (g = 0; g < 51; g++) {
		puffle_frame_normal[6][g] = puffle_frame_normal[7][g] = puffle_frame_normal[8][g] = puffle_frame_normal[9][g] = FACE_0_0;
	}
	
	for (g = 0; g < 3; g++) {
		puffle_frame_normal[g][46] = puffle_frame_normal[g][47] = FACE_0_1;
		puffle_frame_normal[g][48] = puffle_frame_normal[g][49] = puffle_frame_normal[g][50] = puffle_frame_normal[g][51] = FACE_0_2;
		puffle_frame_normal[g][52] = puffle_frame_normal[g][53] = FACE_0_3;
		puffle_frame_normal[g][54] = -1;
	}
	
	puffle_frame_normal[3][53] = puffle_frame_normal[3][54] = FACE_0_1;
	puffle_frame_normal[3][55] = puffle_frame_normal[3][56] = puffle_frame_normal[3][57] = puffle_frame_normal[3][58] = FACE_0_2;
	puffle_frame_normal[3][59] = puffle_frame_normal[3][60] = FACE_0_3;
	puffle_frame_normal[3][61] = -1;
	
	puffle_frame_normal[4][41] = puffle_frame_normal[4][42] = FACE_0_1;
	puffle_frame_normal[4][43] = puffle_frame_normal[4][44] = puffle_frame_normal[4][45] = puffle_frame_normal[4][46] = FACE_0_2;
	puffle_frame_normal[4][47] = puffle_frame_normal[4][48] = FACE_0_3;
	puffle_frame_normal[4][49] = -1;
	
	puffle_frame_normal[5][56] = puffle_frame_normal[5][57] = FACE_0_1;
	puffle_frame_normal[5][58] = puffle_frame_normal[5][59] = puffle_frame_normal[5][60] = puffle_frame_normal[5][61] = FACE_0_2;
	puffle_frame_normal[5][62] = puffle_frame_normal[5][63] = FACE_0_3;
	puffle_frame_normal[5][64] = -1;
	
	for (g = 6; g < 10; g++) {
		puffle_frame_normal[g][51] = puffle_frame_normal[g][52] = FACE_0_1;
		puffle_frame_normal[g][53] = puffle_frame_normal[g][54] = puffle_frame_normal[g][55] = puffle_frame_normal[g][56] = FACE_0_2;
		puffle_frame_normal[g][57] = puffle_frame_normal[g][58] = FACE_0_3;
		puffle_frame_normal[g][59] = -1;
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
	int paquete[10];
	int sel_mapa;
	const int (*local_mapa)[2];
	
	r = RANDOM (5);
	sel_mapa = RANDOM (8);
	local_mapa = map[sel_mapa];
	
	/* TODO: Seleccionar un paquete de colores */
	memcpy (paquete, colores[r], sizeof (paquete));
	
	for (g = 0; g < 20; g++) {
		r = RANDOM (9) + 1;
		//printf ("Intercambiando %i por %i\n", 0, r);
		sel_mapa = paquete[0];
		paquete[0] = paquete[r];
		paquete[r] = sel_mapa;
	}
	
	for (g = 0; g < 10; g++) {
		puffles[g].color = paquete [g];
		puffles[g].capturado = puffles[g].escapado = FALSE;
		puffles[g].frame = 0;
		puffles[g].dir = PUFFLE_DIR_0;
		puffles[g].distancia = puffle_data[paquete[g]][0];
		puffles[g].velocidad = puffle_data[paquete[g]][1];
		puffles[g].x = local_mapa[g][0];
		puffles[g].y = local_mapa[g][1];
	}
}

int map_button_in_opening (int x, int y) {
	if (x >= 403 && x < 541 && y >= 277 && y < 314) return BUTTON_UI_START;
	if (x >= 720 && x < 749 && y >= 12 && y < 41) return BUTTON_CLOSE;
	return BUTTON_NONE;
}

int map_button_in_game (int x, int y) {
	if (x >= 720 && x < 749 && y >= 12 && y < 41) return BUTTON_CLOSE;
	return BUTTON_NONE;
}

int map_button_in_score (int x, int y) {
	if (x >= 244 && x < 399 && y >= 285 && y < 319) return BUTTON_UI_PLAY_MORE;
	if (x >= 393 && x < 524 && y >= 285 && y < 319) return BUTTON_UI_FINISH;
	if (x >= 720 && x < 749 && y >= 12 && y < 41) return BUTTON_CLOSE;
	return BUTTON_NONE;
}
