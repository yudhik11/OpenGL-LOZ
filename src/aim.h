#include "main.h"

#ifndef AIM_H
#define AIM_H


class Aim {
public:
    Aim() {}
    Aim(float x, float y, color_t color);
    glm::vec3 position;
    float rotation_z;
    float rotation_h;
    float launch_speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
    VAO *object1;
    VAO *object2;
    VAO *object3;
    VAO *object4;
    VAO *object5;
    VAO *object6;
    VAO *object7;
    VAO *object8;
};

#endif // CUBE_H
