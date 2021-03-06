#include "main.h"

#ifndef POOL_H
#define POOL_H


class Pool {
public:
    Pool() {}
    Pool(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float launch_speed;
    float level_angle;
    float angular_speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void shm();
    double speed;
private:
    VAO *object;
};

#endif // POOL_H
