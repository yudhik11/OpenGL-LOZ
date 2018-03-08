#include "monster.h"
#include "main.h"

Monster::Monster(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    size = 1;
    // Our vertices. Three consecutive floats give 1.0f 3D vertex; Three consecutive vertices give 1.0f triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
    
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,1.0f,
        -1.0f,1.0f,1.0f,
        1.0f,1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,1.0f,-1.0f,
        1.0f,-1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,

        1.0f,1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,1.0f,1.0f,
        -1.0f,1.0f,-1.0f,
        1.0f,-1.0f,1.0f,
        -1.0f,-1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,
        1.0f,-1.0f,1.0f,
        1.0f,1.0f,1.0f,
        1.0f,-1.0f,-1.0f,

        1.0f,1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f,1.0f,1.0f,
        1.0f,-1.0f,1.0f,
        1.0f,1.0f,1.0f,
        1.0f,1.0f,-1.0f,
        -1.0f,1.0f,-1.0f,
        1.0f,1.0f,1.0f,
        -1.0f,1.0f,-1.0f,
        -1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,
        -1.0f,1.0f,1.0f,
        1.0f,-1.0f,1.0f,

        0.2,0,1,
        0.8,0,1,
        0.7,0,1.75,

        -0.2,0,1,
        -0.8,0,1,
        -0.7,0,1.75,
    };

    this->rotation = 0;
    this->launch_speed = 0;
    speed = 1;
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
    if (color.r == COLOR_GREEN.r){
        this->object1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data +108, COLOR_CUBE8, GL_FILL);
    }
    else if (color.r == COLOR_CUBE8.r) {
        this->object1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data +108, COLOR_GREEN, GL_FILL);
    }
    else {
        this->object1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data +108, COLOR_BLACK, GL_FILL);
    }
}

void Monster::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 scale    = glm::scale(glm::vec3(this->size, this->size, this->size));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model = Matrices.model * translate * rotate * scale;
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
}

bounding_box_t Monster::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t bbox = { x, y, z, 2 * this->size, 2 * this->size, 2 * this->size};
    return bbox;
}

void Monster::set_position(float x, float y) {this->position = glm::vec3(x, y, 0);}
void Monster::tick() {this->rotation += speed;}
