#include "main.h"

#ifndef PATCHES_H
#define PATCHES_H


class Patches {
public:
    Patches() {}
    Patches(float x, float y,float z, color_t color);
    glm::vec3 position;
    int shmAng;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void shm();
    double speed;
private:
    VAO *object;
    VAO *object1;
};

#endif // PATCHES_H
