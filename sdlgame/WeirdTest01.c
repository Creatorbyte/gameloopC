#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int initialize_window(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL. \n");
		return FALSE;
	}
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		//SDL_WINDOW_BORDERLESS
		SDL_WINDOW_FULLSCREEN
		//SDL_WINDOW_BORDERLESS
	);

	if(!window ){
		fprintf(stderr, "error creating window!!!\n");
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer) {
		fprintf(stderr, "error creating renderer!!!\n");
		return 0;
	}

	return TRUE;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type) {
		// SDL_Quit is the exit button on the window
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				game_is_running = FALSE;
			break;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////

struct box {
	float x;
	float y;
	float w;
	float h;
} box;

struct box boxes[10000];
int offx = WINDOW_WIDTH/2;
int offy = WINDOW_HEIGHT/2;

void setup() {

	for(int i = 0; i <= 10000; i++) {
		boxes[i].x = offx;
		boxes[i].y = offy;
		boxes[i].w = 1;
		boxes[i].h = 1;
	}
};

float a = 0;
float d1 = 0, d2 = 0;

void update() {
	a += 1;
	//d1 = 100 * cosf(0.01 * a);
	d2 = 120*sinf(0.01 * a);
	
	for(int i = 0; i <= 10000; i++) {
		d1 = d2 * cosf((4*M_PI/(10000))*2*i);
		//d2 = 100 * sinf(0.01 * i);
		boxes[i].x = (offx) + (50+d1)*cosf((2*M_PI/(10000))*i);
		boxes[i].y = (offy) + (50+d1)*sinf((2*M_PI/(10000))*i);
	}
	
	//box.x += 1;
	//box.y += 1;

};

void render() {
	SDL_SetRenderDrawColor(renderer, 0,0,0,50);// set draw color
	SDL_RenderClear(renderer);

	//TODO: this is where we can render stuff

	SDL_SetRenderDrawColor(renderer, 255,0,0,50);

	for(int i = 0; i <= 10000; i++) {
		
		SDL_Rect box_rect = {
		(int)boxes[i].x,
		(int)boxes[i].y,
		(int)boxes[i].w,
		(int)boxes[i].h
		};

		SDL_RenderFillRect(renderer, &box_rect);
	}

	SDL_RenderPresent(renderer); // push modified frame buffer
};

///////////////////////////////////////////////////////////////////////////////////////////////////

void destroy_window (){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

};

int main(int argc, char **argv) {
	game_is_running = initialize_window();

	setup();

	while(game_is_running) {
		printf("running...\n");
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}