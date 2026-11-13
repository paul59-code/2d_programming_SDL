#include <iostream>
#include <SDL3/SDL.h>
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

using namespace std;

typedef struct {
    SDL_FRect rect;
    float vx,vy;
}Box;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Box box;
    Uint64 last_time;
}AppState;

int main() {
    return 0;
}