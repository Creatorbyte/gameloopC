#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include "main.h"

float delta_time = 0;
int nextX = 0, nextY = 0;

struct game_object {
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, wall;

// the setup that is run once at the program start
void setup(void) {
    srand(time(0));
    ball.width = 15;
    ball.height = 15;
    ball.x = 20;
    ball.y = 20;
    ball.vel_x = 30;
    ball.vel_y = 30;

    wall.width = WINDOW_WIDTH; 
    wall.height = WINDOW_HEIGHT/5;
    wall.x = 0;//(WINDOW_WIDTH / 2) - (wall.width / 2);
    wall.y =WINDOW_HEIGHT - (WINDOW_HEIGHT/3);
    wall.vel_x = 0;
    wall.vel_y = 0;
}

// the update that happens 60 times per second
void update(void) {
    // Waste some time / sleep until we reach the frame target time
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    // Get a delta time factor converted to seconds to be used to update my objects
    delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    // Store the milliseconds of the current frame
    last_frame_time = SDL_GetTicks();

    // Check for game over
    //if (ball.y + ball.height > WINDOW_HEIGHT)
        //game_is_running = FALSE;


    // check wall collisions
    if (ball.x+ball.vel_x < 0 || ball.x+ball.vel_x > WINDOW_WIDTH-ball.width) {
        //bounce on side!
        ball.vel_x *= -0.8;
    } else {
        ball.x += ball.vel_x*0.98;
    }
    
    // move ball up/down
    if (ball.y+ball.vel_y < 0 || ball.y+ball.vel_y > wall.y-ball.height) {
     // bounce on floor/ceiling
        ball.vel_y = ball.vel_y * -0.8;
    }
    
    //if bounce was too small -> bump into air
    if (abs(ball.vel_y) < 0.01) {
        ball.vel_x = rand()/1000;
        ball.vel_y = rand()/1000;
    } else {
        ball.y += ball.vel_y;
    }
    
    // gravity!
    ball.vel_y += 0.2;
}

// rednder draws to the screen
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 10);// self explanitory

    SDL_Rect BG = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

    SDL_RenderFillRect(renderer, &BG);
    //SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    // Render the left wall
    SDL_Rect wall_rect = {
        (int)wall.x,
        (int)wall.y,
        (int)wall.width,
        (int)wall.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &wall_rect);

    SDL_RenderPresent(renderer);
}


//reads keyboard events and maps them to game functions
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break; 
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                game_is_running = FALSE;
            break;
        case SDL_KEYUP:
            
            break;
    }
}

// the main game loop
int main(int argc, char **argv) {
    game_is_running = initialize_window();

    setup();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}

