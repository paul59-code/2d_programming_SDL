#include "SDL3/SDL_rect.h"
#include <glm/glm.hpp>
class Box : public SDL_FRect
{
private: glm::vec2 v;
public: Box() = default;
public: Box(const glm::vec2 v) {
    this->v=v;
}
public: glm::vec2 getSpeed() {
    return v;
}
public: glm::vec2 setVX(float vx) {
    v.x=vx;
}
public: glm::vec2 setVY(float vy) {
    v.y=vy;
}
};