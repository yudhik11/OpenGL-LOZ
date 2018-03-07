#include "barrel.h"
#include "main.h"

Barrel::Barrel(float x, float y, float z, float edge, color_t Color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->edge = edge;
    this->flag = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[60*9];
    float ang = M_PI/4.0;
    float curr = ang/2.0;
    int k=0;
    this->visible = 1;
    this->radius = 3;
    for(int i=1;i<=8;i++){
        vertex_buffer_data[k++] = edge;
        vertex_buffer_data[k++] = this->radius*cos(curr);
        vertex_buffer_data[k++] = this->radius*sin(curr);

        vertex_buffer_data[k++] = edge;
        vertex_buffer_data[k++] = this->radius*cos(curr+ang);
        vertex_buffer_data[k++] = this->radius*sin(curr+ang);

        vertex_buffer_data[k++] = edge;
        vertex_buffer_data[k++] = 0.0;
        vertex_buffer_data[k++] = 0.0;

        vertex_buffer_data[k++] = -edge;
        vertex_buffer_data[k++] = this->radius*cos(curr);
        vertex_buffer_data[k++] = this->radius*sin(curr);

        vertex_buffer_data[k++] = -edge;
        vertex_buffer_data[k++] = this->radius*cos(curr+ang);
        vertex_buffer_data[k++] = this->radius*sin(curr+ang);

        vertex_buffer_data[k++] = -edge;
        vertex_buffer_data[k++] = 0.0;
        vertex_buffer_data[k++] = 0.0;

        vertex_buffer_data[k++] = -edge;
        vertex_buffer_data[k++] = this->radius*cos(curr+ang);
        vertex_buffer_data[k++] = this->radius*sin(curr+ang);

        vertex_buffer_data[k++] = edge;
        vertex_buffer_data[k++] = this->radius*cos(curr+ang);
        vertex_buffer_data[k++] = this->radius*sin(curr+ang);

        vertex_buffer_data[k++] = edge;
        vertex_buffer_data[k++] = this->radius*cos(curr);
        vertex_buffer_data[k++] = this->radius*sin(curr);

        vertex_buffer_data[k++] = -edge;
        vertex_buffer_data[k++] = this->radius*cos(curr);
        vertex_buffer_data[k++] = this->radius*sin(curr);

        vertex_buffer_data[k++] = -edge;
        vertex_buffer_data[k++] = this->radius*cos(curr+ang);
        vertex_buffer_data[k++] = this->radius*sin(curr+ang);

        vertex_buffer_data[k++] = edge;
        vertex_buffer_data[k++] = this->radius*cos(curr);
        vertex_buffer_data[k++] = this->radius*sin(curr);

        curr+=ang;
    }


    this->object = create3DObject(GL_TRIANGLES, 32*3, vertex_buffer_data, Color, GL_FILL);
}

void Barrel::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

}

void Barrel::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Barrel::bounding_box() {
    float x = this->position.x, y = this->position.y;  float z = this->position.z;
    bounding_box_t bbox = { x, y, z, 2 * this->radius, 2 * this->radius, 2*this->edge};
    return bbox;
}

bounding_box_t Barrel::jump_bounding_box() {
    float x = this->position.x, y = this->position.y;  float z = this->position.z;
    bounding_box_t bbox = { x, y, z, 2 * this->radius, 2 * this->radius, 1000};
    return bbox;
}
