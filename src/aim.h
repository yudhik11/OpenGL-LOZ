#include "main.h"

#ifndef AIM_H
#define AIM_H


class Aim {
public:
    Aim() {}
    Aim(float x, float y, color_t color);
    void set_position(float x, float y);
    glm::vec3 position;
    float rotation_h;
    float rotation_z;
    float launch_speed;
    void draw(glm::mat4 VP);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // CUBE_H
