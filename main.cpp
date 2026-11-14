#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "SDL3/SDL_render.h"
#include <iostream>
#include "Box.cpp"
#include <vector>
#define vy_rects 30
#define RECTS_SIZE 40
#define D 10
    using namespace std;

void genereazaCerc(SDL_Renderer* renderer, float centruX, float centruY, float raza) {
    int diametru = (raza * 2);
    int x = (raza - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int eroare = (tx - diametru);
    while (x >= y) {
        SDL_RenderPoint(renderer, centruX + x, centruY - y);
        SDL_RenderPoint(renderer, centruX + x, centruY + y);
        SDL_RenderPoint(renderer, centruX - x, centruY - y);
        SDL_RenderPoint(renderer, centruX - x, centruY + y);
        SDL_RenderPoint(renderer, centruX + y, centruY - x);
        SDL_RenderPoint(renderer, centruX + y, centruY + x);
        SDL_RenderPoint(renderer, centruX - y, centruY - x);
        SDL_RenderPoint(renderer, centruX - y, centruY + x);
        if (eroare <= 0) {
            ++y;
            eroare += ty;
            ty += 2;
        }
        if (eroare > 0) {
            --x;
            tx += 2;
            eroare += tx - diametru;
        }
    }
}

class obiectGeneral {
protected:
    glm::vec2 pozitie;
    glm::vec2 viteza;
    float latime;
    float inaltime;
    bool alive;
public:
    obiectGeneral(glm::vec2 startPos, float w, float h)
        : pozitie(startPos), latime(w), inaltime(h), viteza(0.0f, 0.0f), alive(true) {}

    virtual ~obiectGeneral() = default;

    virtual void update(float deltaTime) = 0;

    virtual void render(SDL_Renderer* renderer) = 0;

    glm::vec2 getPosition() const {
	return pozitie;
    }
    bool getIsAlive() const {
        return alive;
    }
    void destroy() {
        alive = false;
        }

    virtual SDL_FRect getBoundingBox() const
    {
        return {
            pozitie.x - latime / 2.0f,
            pozitie.y - inaltime / 2.0f,
            latime,
            inaltime
        };
    }
};


class PatratPlayer : public obiectGeneral {
private:
    SDL_Color color; // verde

public:
    PatratPlayer(glm::vec2 startPos)
        : obiectGeneral(startPos, 50.0f, 50.0f), color({0, 255, 0, 255}) {}

    void update(float deltaTime) override {
        pozitie += viteza * deltaTime;
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = getBoundingBox();
        SDL_RenderFillRect(renderer, &rect);
    }

    void setColor(SDL_Color newColor) { color = newColor; }

    void handleInput(float mouseRelX, float mouseRelY) {
        pozitie.x += mouseRelX;
        pozitie.y += mouseRelY;
        if (pozitie.x < latime / 2.0f) {
            pozitie.x = latime / 2.0f;
        }
        if (pozitie.x > 700.0f - latime / 2.0f) {
            pozitie.x = 700.0f - latime / 2.0f;
        }
        if (pozitie.y < inaltime / 2.0f) {
            pozitie.y = inaltime / 2.0f;
        }
        if (pozitie.y > 800.0f - inaltime / 2.0f) {
            pozitie.y = 800.0f - inaltime / 2.0f;
        }
    }
};

class PatratTarget : public obiectGeneral {
private:
    float fireTimer; // cqnd tinta trebuie sa traga cu bile

public:
    PatratTarget(glm::vec2 startPos, float initialSpeed)
        : obiectGeneral(startPos, 50.0f, 50.0f), fireTimer(0.0f) {
        viteza.y = initialSpeed;
    }

    void update(float deltaTime) override {
        pozitie.y += viteza.y * deltaTime;
        fireTimer += deltaTime;
        // la fiecare x secunde se trage
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 167, 0, 255);
        SDL_FRect rect = getBoundingBox();
        SDL_RenderFillRect(renderer, &rect);
    }
};

class Proiectil : public obiectGeneral {
private:
    SDL_Color color;
    float radius;

public:
    Proiectil(glm::vec2 startPos, glm::vec2 dirVelocity, SDL_Color projColor)
        : obiectGeneral(startPos, 20.0f, 20.0f), color(projColor), radius(10.0f) {
        viteza = dirVelocity;
    }

    void update(float deltaTime) override {
        pozitie += viteza * deltaTime;
    }

    void render(SDL_Renderer* renderer) override {
        // desenez cerc folosind functii geometrice
        // creez fill circle
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        genereazaCerc(renderer, pozitie.x, pozitie.y, radius);
    }
};

constexpr int WINDOW_WIDTH{700};
constexpr int WINDOW_HEIGHT{800};

SDL_Window *window{nullptr};
SDL_Renderer *renderer{nullptr};
SDL_Event currentEvent;
SDL_Color backgroundColor{255, 255, 255, 255};

bool quit{false};

float mouseX{-1.0f}, mouseY{-1.0f};

float displayScale{1.0f};



bool initWindow() {
    bool success{true};
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL initialization failed: %s\n", SDL_GetError());
        success = false;
    } else {
        displayScale = SDL_GetDisplayContentScale(1);
        if (!SDL_CreateWindowAndRenderer(
            "SDL Hello World Example",
            static_cast<int>(displayScale * WINDOW_WIDTH),
            static_cast<int>(displayScale * WINDOW_HEIGHT),
            0,
            &window, &renderer)) {
            SDL_Log("Failed to create window and renderer: %s\n", SDL_GetError());
            success = false;
        } else {
            SDL_SetRenderScale(renderer, displayScale, displayScale);

            SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b,
                                   backgroundColor.a);
            SDL_RenderClear(renderer);
        }
    }

    return success;
}

void processEvents() {
    while (SDL_PollEvent(&currentEvent)) {
        if (currentEvent.type == SDL_EVENT_QUIT) {
            quit = true;
        }
        if (currentEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (currentEvent.button.button == SDL_BUTTON_LEFT) {
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseX /= displayScale;
                mouseY /= displayScale;
            }
        }

        if (currentEvent.type == SDL_EVENT_MOUSE_MOTION) {
            float relX = currentEvent.motion.xrel;
            float relY = currentEvent.motion.yrel;
           // PatratPlayer.handleInput(relX, relY);
        }
    }
}

void drawFrame() {
    //Clear the background
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    //Draw scene
    //TODO

    //Update window
    SDL_RenderPresent(renderer);
}

void cleanup() {
    //Destroy renderer
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    //Destroy window
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    //Quit SDL
    SDL_Quit();
}

int main() {
    Box rects[RECTS_SIZE];
    int i;
    for (i=0;i<RECTS_SIZE/2;i++) {
        rects[i].setVX(0);
        rects[i].setVY(vy_rects);
        rects[i].x=0;
        rects[i].y=i*D;
        rects[i+20].y=i*D;
        rects[i+20].setVX(0);
        rects[i+20].setVY(vy_rects);
        rects[i+20].x=WINDOW_WIDTH;
    }


    if (!initWindow()) {
        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }
    SDL_zero(currentEvent);
    glm::vec2 playerStartPos(350.0f, 800.0f - 25.0f);
    PatratPlayer player(playerStartPos);
    std::vector<PatratTarget> enemies;
    enemies.push_back(PatratTarget(glm::vec2(116.0f, 100.0f), 0.0f));
    enemies.push_back(PatratTarget(glm::vec2(350.0f, 200.0f), 0.0f));
    enemies.push_back(PatratTarget(glm::vec2(583.0f, 150.0f), 0.0f));


    int lastTime = SDL_GetTicks();
    SDL_SetWindowRelativeMouseMode(window, true);
    while (!quit) {
        int currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        processEvents();
        player.update(deltaTime);
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }
        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_RenderClear(renderer);
        player.render(renderer);
        for (auto& enemy : enemies) {
            enemy.render(renderer);
        }
        SDL_RenderPresent(renderer);
    }
    cleanup();
    return 0;
}