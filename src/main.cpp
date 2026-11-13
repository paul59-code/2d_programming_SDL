#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
    // A short automatic run is useful for checking the build without interaction.
    const bool smokeTest = argc > 1 && SDL_strcmp(argv[1], "--smoke-test") == 0;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("Ricochet | SDL3", 960, 600,
                                    SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderVSync(renderer, 1);
    SDL_SetRenderLogicalPresentation(renderer, 960, 600, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    bool running = true;
    const Uint64 start = SDL_GetTicks();
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 24, 34, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 72, 200, 170, 255);
        const SDL_FRect rectangle{380.0f, 240.0f, 200.0f, 120.0f};
        SDL_RenderFillRect(renderer, &rectangle);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
        if (smokeTest && SDL_GetTicks() - start >= 500) {
            running = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
