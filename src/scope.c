/*
   # Desktop PPI Display #

   This program is designed to display a simple plan posiition indicator (PPI)
   in a desktop window. The PPI presents target locations based on real-time
   data gathered and processed by a device composed of multiple sensors,
   a motor, and a microprocessor. The microcontroller communicates with the desktop
   app via a linux device driver.

   Created by: Christopher Adams
   Created on: 2025/03/21
*/

/* includes */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* window dimensions */
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600


/* essential data*/
typedef struct {
    struct SDL_Window *window;
    struct SDL_Renderer *renderer;
    SDL_Event event;
    bool is_running;
} App;

/* function prototyping*/
bool app_init(App *a);
void app_run(App *a);
void app_free(App *a);

/* functions*/
bool app_init(App *a) {
    a->window = SDL_CreateWindow("PPI App", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!a->window) {
        fprintf(stderr, "*ERROR* Failed to create window: %s", SDL_GetError());
        return false;
    }

    return true;
}

void app_run(App *a) {
    a->is_running = true;

    while (a->is_running) {
        while (SDL_PollEvent(&a->event)) {
            switch (a->event.type)
            {
            case SDL_EVENT_QUIT:
                a->is_running = false;
                break;
            
            case SDL_EVENT_KEY_DOWN:
                switch (a->event.key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    a->is_running = false;
                    break;
                default:
                    break;
                }
                break;
            
            default:
                break;
            }
        }

    }
}

void app_free(App *a) {
    if (a->window) {
        SDL_DestroyWindow(a->window);
        a->window = NULL;
    }
}

/* init */
int main(void) {
    bool exit_status = EXIT_FAILURE;
    App app = {0};

    if (app_init(&app)) {
        app_run(&app);

        exit_status = EXIT_SUCCESS;
    }
    app_free(&app);

    return exit_status;
}