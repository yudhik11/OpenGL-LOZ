#include "main.h"

#ifndef WIND_H
#define WIND_H


class Wind {
public:
    Wind() {}
    Wind(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
    VAO *object1;
};

#endif // WIND_H
