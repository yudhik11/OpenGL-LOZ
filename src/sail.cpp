#include "sail.h"
#include "main.h"

Sail::Sail(color_t color) {
    this->position = glm::vec3(0, 0, 0);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        0.4f, 0.0f, -1.0f,
        -0.4f, 0.0f, -1.0f,
        0.0f, 0.0f, 25.0f,

        0.2f, 0.0f, 11.0f,
        10.0f, 0.0f, 11.0f,
        0.0f,  0.0f, 25.0f,

    };

    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_BROWN, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+9, color, GL_FILL);
}

void Sail::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate   = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of sail arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);

}

void Sail::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
