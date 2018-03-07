#include "wind.h"
#include "main.h"

Wind::Wind(float x, float y) {
    this->position = glm::vec3(x, y, -3.0f);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        6.4f, -28.0f, 3.0f,
        -6.4f, -28.0f, 3.0f,
        0.0f, 0.0f, 3.0f,

        3.4f, -28.0f, 3.0f,
        -3.4f, -28.0f, 3.0f,
        0.0f, 0.0f, 3.0f,

    };

    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_DBLUE, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+9, COLOR_BLUE, GL_FILL);
}

void Wind::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate   = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of wind around which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);

}

void Wind::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
