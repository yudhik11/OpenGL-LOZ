#include "main.h"

#ifndef GIFTBARREL_H
#define GIFTBARREL_H


class Giftbarrel {
public:
    Giftbarrel() {}
    Giftbarrel(float x, float y, float z);
    glm::vec3 position;
    float rotation_x;
    float rotation_z;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    double radius;
    int visible;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // GIFTBARREL_H
