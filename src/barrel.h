#ifndef BARRELL_H
#define BARRELL_H
#include "main.h"


class Barrel {
public:
    Barrel() {}
    Barrel(float x, float y, float z, float edge, color_t Color);
    glm::vec3 position;
    float rotation;
    float edge;
    int flag;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    int visible;
    float radius;
    bounding_box_t bounding_box();
    bounding_box_t jump_bounding_box();
private:
    VAO *object;
};


#endif // BARRELL_H

