#include "cube.h"
#include "main.h"

Cube::Cube(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->launch_speed = 0;
    speed = 1.0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        4.0f,-6.0f,-2.0f, 
        3.0f, 6.0f, 2.0f, 
        4.0f,-6.0f, 2.0f, 
        4.0f,-6.0f,-2.0f, 
        3.0f, 6.0f, 2.0f, 
        3.0f, 6.0f,-2.0f, 

          3.0f,6.0f, 2.0f,
         -3.0f,6.0f,-2.0f,
          3.0f,6.0f,-2.0f,
          3.0f,6.0f, 2.0f,
         -3.0f,6.0f,-2.0f,
         -3.0f,6.0f, 2.0f,

        -3.0f, 6.0f, 2.0f,
        -4.0f,-6.0f,-2.0f,
        -3.0f, 6.0f,-2.0f,
        -3.0f, 6.0f, 2.0f,
        -4.0f,-6.0f,-2.0f,
        -4.0f,-6.0f, 2.0f,

        -4.0f,-6.0f,-2.0f,
         4.0f,-6.0f, 2.0f,
        -4.0f,-6.0f, 2.0f,
        -4.0f,-6.0f,-2.0f,
         4.0f,-6.0f, 2.0f,
         4.0f,-6.0f,-2.0f,

         4.0f,-6.0f,-2.0f,
        -3.0f, 6.0f,-2.0f,
         3.0f, 6.0f,-2.0f,
         4.0f,-6.0f,-2.0f,
        -3.0f, 6.0f,-2.0f,
        -4.0f,-6.0f,-2.0f,

        3.0f, 6.0f, 2.0f, 
        3.0f, 6.0f,-2.0f, 
        0.0f,12.0f, 3.0f, 

         3.0f, 6.0f,-2.0f,
        -3.0f, 6.0f,-2.0f,
         0.0f,12.0f, 3.0f,

        -3.0f, 6.0f,-2.0f,
        -3.0f, 6.0f, 2.0f,
         0.0f,12.0f, 3.0f,

        -3.0f, 6.0f, 2.0f,
         3.0f, 6.0f, 2.0f,
         0.0f,12.0f, 3.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_CUBE1, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+18, COLOR_CUBE2, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+36, COLOR_CUBE3, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+54, COLOR_CUBE4, GL_FILL);
    this->object4 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+72, COLOR_CUBE5, GL_FILL);
    this->object5 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+90, COLOR_CUBE6, GL_FILL);
    this->object6 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+99, COLOR_CUBE7, GL_FILL);
    this->object7 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+108, COLOR_CUBE8, GL_FILL);
    this->object8 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+117, COLOR_CUBE9, GL_FILL);
        
}

void Cube::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
    draw3DObject(this->object4);
    draw3DObject(this->object5);
    draw3DObject(this->object6);
    draw3DObject(this->object7);
    draw3DObject(this->object8);
}

void Cube::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Cube::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

