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
#include <time.h>

/* window attributes */
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600


/* application attributes */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool is_running;
} App;

/* application media */
typedef struct {
    /* ppi display attributes */
    struct {
        SDL_Texture *display;
        SDL_Texture *sweep;
        SDL_Texture *blip;
    } ppi;
} Media;

/* function prototyping */
bool app_init(App *a, Media *m);
void app_event(App *a);
void app_render(App *a, Media *m);
void app_run(App *a, Media *m);
void app_free(App *a, Media *m);

/* Initialization -> creates the application's window and renderer */
bool app_init(App *a, Media *m) {
    a->window = SDL_CreateWindow("PPI App", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!a->window) {
        fprintf(stderr, "*ERROR* Failed to create window: %s", SDL_GetError());
        return false;
    }

    a->renderer = SDL_CreateRenderer(a->window, NULL);
    if (!a->renderer) {
        fprintf(stderr, "*ERROR* Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    m->ppi.display = IMG_LoadTexture(a->renderer, "images/ppi_display.png");
    if (!m->ppi.display) {
        fprintf(stderr, "*ERROR* Failed to load texture: %s", SDL_GetError());
        return false;
    }

    m->ppi.sweep = IMG_LoadTexture(a->renderer, "images/ppi_radial_sweep.png");
    if (!m->ppi.sweep) {
        fprintf(stderr, "*ERROR* Failed to load texture: %s", SDL_GetError());
        return false;
    }

    m->ppi.blip = IMG_LoadTexture(a->renderer, "images/ppi_blip.png");
    if (!m->ppi.blip) {
        fprintf(stderr, "*ERROR* Failed to load texture: %s", SDL_GetError());
        return false;
    }

    return true;
}

/* Event polling -> Polls events and performs tasks based on event input */
void app_event(App *a) {
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

/* Rendering -> renders content in application window */
void app_render(App *a, Media *m) {
    SDL_SetRenderDrawColor(a->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(a->renderer);
    /*********************************/
    /* == DEMONSTRATION == */
    /* This is temporary code to demonstrate rotation of the radial sweep.*/
    /* Real-time angle data should be passed from app_event to determine */
    /* angle of the radial sweep. */
    const double PI = 3.14;
    SDL_FPoint center = {WINDOW_HEIGHT/2, WINDOW_WIDTH/2};
    double angle = ((double)clock()) * PI / 200;
    /**********************************/
    SDL_RenderTextureRotated(a->renderer, m->ppi.sweep, NULL, NULL, angle, &center, SDL_FLIP_NONE);
    SDL_RenderTexture(a->renderer, m->ppi.display, NULL, NULL);
    SDL_RenderPresent(a->renderer);
}

/* Main loop -> runs application after initialization via app_init */
void app_run(App *a, Media *m) {
    a->is_running = true;

    while (a->is_running) {
        app_event(a);
        app_render(a, m);
        /*****************************/
        /* == DEMONSTRATION == */
        /* This delay should be pulled out when app_event is refactored */
        /* to poll for real-time angle and range data. */
        SDL_Delay(100);
        /*****************************/
    }
}

/* Quit -> frees memory allocated in app_init before quitting application */
void app_free(App *a, Media *m) {
    if (a->renderer) {
        SDL_DestroyRenderer(a->renderer);
        a->renderer = NULL;
    }

    if (a->window) {
        SDL_DestroyWindow(a->window);
        a->window = NULL;
    }

    if (m->ppi.blip) {
        SDL_DestroyTexture(m->ppi.blip);
        m->ppi.blip = NULL;
    }

    if (m->ppi.sweep) {
        SDL_DestroyTexture(m->ppi.sweep);
        m->ppi.sweep = NULL;
    }

    if (m->ppi.display) {
        SDL_DestroyTexture(m->ppi.display);
        m->ppi.display = NULL;
    }
}

/* init */
int main(void) {
    bool exit_status = EXIT_FAILURE;
    App app = {0};
    Media media = {0};

    if (app_init(&app, &media)) {
        app_run(&app, &media);

        exit_status = EXIT_SUCCESS;
    }
    app_free(&app, &media);

    return exit_status;
}