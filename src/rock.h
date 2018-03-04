#include "main.h"

#ifndef ROCK_H
#define ROCK_H


class Rock {
public:
    Rock() {}
    Rock(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float size;
    float launch_speed;
    float launch_speed_x;
    float launch_speed_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H