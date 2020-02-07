#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"
#include "main.h"

struct game_object {
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle;

// the setup that is run once at the program start
void setup(void) {
    ball.width = 15;
    ball.height = 15;
    ball.x = 20;
    ball.y = 20;
    ball.vel_x = 300;
    ball.vel_y = 300;

    paddle.width = 100; 
    paddle.height = 20;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.vel_x = 0;
    paddle.vel_y = 0;
}

// the update that happens 60 times per second
void update(void) {
    // Waste some time / sleep until we reach the frame target time
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    // Get a delta time factor converted to seconds to be used to update my objects
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    // Store the milliseconds of the current frame
    last_frame_time = SDL_GetTicks();

    // Check for ball collision with the walls
    if (ball.x + ball.width > WINDOW_WIDTH || ball.x < 0)
        ball.vel_x = -ball.vel_x;
    if (ball.y < 0)
        ball.vel_y = -ball.vel_y;

    // Check for ball collision with the paddle
    if (ball.y + ball.height >= paddle.y && ball.x + ball.width > paddle.x && ball.x < paddle.x + paddle.width)
        ball.vel_y = -ball.vel_y;

    // Check for game over
    if (ball.y + ball.height > WINDOW_HEIGHT)
        game_is_running = FALSE;

    // update ball position
    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    // update paddle position
    paddle.x += paddle.vel_x * delta_time;
    paddle.y += paddle.vel_y * delta_time;
}

// rednder draws to the screen
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);// self explanitory
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    // Render the left paddle
    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

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
            if (event.key.keysym.sym == SDLK_LEFT)
                paddle.vel_x = -400;
            if (event.key.keysym.sym == SDLK_RIGHT)
                paddle.vel_x = +400;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LEFT)
                paddle.vel_x = 0;
            if (event.key.keysym.sym == SDLK_RIGHT)
                paddle.vel_x = 0;
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

