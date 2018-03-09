#include "aim.h"
#include "main.h"

Aim::Aim(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation_z = 0;
    this->rotation_h = 0;
    this->launch_speed = 0;
    speed = 1.00;
    static const GLfloat vertex_buffer_data[] = {
        1.0f, 0.0f,  0.0f, 
        -1.0f, 0.0f, 0.0f, 
        1.0f, 6.0f, 0.0f,
        1.0f, 6.0f, 0.0f,
        -1.0f, 6.0f, 0.0f,
        -1.0f, 0.0f, 0.0f, 
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_CUBE1, GL_FILL);        
}

void Aim::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Aim::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation_z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate    *= glm::rotate((float) (this->rotation_h * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

