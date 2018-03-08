#include "giftbarrel.h"
#include "main.h"

Giftbarrel::Giftbarrel(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->rotation_x = 0;
    this->rotation_z = 0;
    radius=5;
    GLfloat vertex_buffer_data[6480];
    int ptr=0;
    this->visible = 0;
    float ang=0;
    for (int i=1;i<=2160;i++){
        if (i%3==0){
            vertex_buffer_data[ptr++]=0;
            vertex_buffer_data[ptr++]=0;
            vertex_buffer_data[ptr++]=0;
            ang-=M_PI/360;
        }
        else{
            vertex_buffer_data[ptr++]=radius*cos(ang);
            vertex_buffer_data[ptr++]=radius*sin(ang);
            vertex_buffer_data[ptr++]=0;
            ang+=M_PI/360;
        }
    }
    color_t colors[7]={COLOR_VIOLET,COLOR_INDIGO,COLOR_BBLUE,COLOR_GREEN,COLOR_YELLOW,COLOR_ORANGE,COLOR_RED};
    this->object = create3DObject(GL_TRIANGLES, 2160, vertex_buffer_data, colors, GL_FILL,7);
}

void Giftbarrel::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation_x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotate1    = glm::rotate((float) (this->rotation_z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate1 * rotate;
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Giftbarrel::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}
void Giftbarrel::tick() {
    this->rotation_x += 20;
    this->rotation_z += 8;
}

bounding_box_t Giftbarrel::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t bbox = { x, y, z, 10, 10 ,10 };
    return bbox;
}
