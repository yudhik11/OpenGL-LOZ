#include "main.h"

#ifndef MONSTER_H
#define MONSTER_H


class Monster {
public:
    Monster() {}
    Monster(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float size;
    float launch_speed;
    float launch_speed_x;
    float launch_speed_y;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
    int life;
    void tick();
    double speed;
    int visible;
private:
    VAO *object;
};

#endif // BALL_H
