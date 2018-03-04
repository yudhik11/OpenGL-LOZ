#include "main.h"

#ifndef POOL_H
#define POOL_H


class Pool {
public:
    Pool() {}
    Pool(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // POOL_H
