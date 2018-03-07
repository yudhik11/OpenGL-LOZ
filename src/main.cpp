#include "main.h"
#include "timer.h"
#include "boat.h"
#include "pool.h"
#include "rock.h"
#include "aim.h"
#include "monster.h"
#include "barrel.h"
#include "sail.h"
#include "wind.h"
#include "patches.h"
#include "gift.h"
#include "ocean.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Boat boat;
Rock rocks[100];
Rock cannon,blast;
Monster monster[51];
Gift gifts[51];
Gift barrel_gift[10];
Pool pool;
Ocean ocean;
Aim aim;
Sail sail;
Wind wind;
Barrel barrel[10];
Patches patches[100];

bool projectType = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0 , gravity = 0.5 ,level = 3,pi=3.141;
float windAngle =0;
float camera_x=0,camera_y=90,camera_z=100,target_x=0,target_y=90,target_z=0;

int flag=0;
int rockCount = 100;
int giftCount = 51;
int logCount = 10;
int patchCount = 100;
int windCounter = 500;
int windTimer = 0;
int monsterCount =51;
int kill_monster = 0;
int total_monster = 0;
int view = 1, time_cnt = 0;
int health = 100 ;
int gifttimer = 0;
int coins = 0;

char title_bar[1024];

Timer t60(1.0 / 60);

void change_camera();
void speed_camera();

float max (float a, float b){
    return a>b ? a:b;
}

float min (float a, float b){
    return a<b ? a:b;
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 0, -5, 2 );
    glm::vec3 eye(camera_x, camera_y, camera_z);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (target_x, target_y, target_z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 0, 1);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    if (windTimer > 0) wind.draw(VP);
    for(int i = 0 ; i < rockCount ; i++){
        rocks[i].draw(VP);
    }
    for(int i = 0 ; i < giftCount ; i++){
        if (gifts[i].visible) gifts[i].draw(VP);
    }
    for(int i = 0 ; i < monsterCount ; i++){
        if (monster[i].visible) monster[i].draw(VP);
    }
    for(int i = 0 ; i < logCount ; i++){
        if (barrel[i].visible) barrel[i].draw(VP);
    }
    for(int i = 0 ; i < logCount ; i++){
        if (barrel_gift[i].visible) barrel_gift[i].draw(VP);
    }
    for(int i = 0 ; i < patchCount ; i++){
        patches[i].draw(VP);
    }
    boat.draw(VP);
    sail.draw(VP);
    cannon.draw(VP);
    aim.draw(VP);
    if (flag){
        blast.draw(VP);
        blast.size -=1;
        if (!blast.size){
            blast.size = 15;
            flag = 0;
        }
    }
    //ocean.draw(VP);
pool.draw(VP);
    
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int W  = glfwGetKey(window, GLFW_KEY_W);
    int A = glfwGetKey(window, GLFW_KEY_A);
    int S = glfwGetKey(window, GLFW_KEY_S);
    int D = glfwGetKey(window, GLFW_KEY_D);
    int O = glfwGetKey(window, GLFW_KEY_O);
    int P = glfwGetKey(window, GLFW_KEY_P);
    int jump = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    int camera = glfwGetKey(window, GLFW_KEY_C);
    if (O) {
        projectType = true;
        reset_screen();
    }
    if (P) {
        projectType = false;
        reset_screen();
    }
    if (left || A) {
        boat.rotation += 5;
        cannon.rotation += 5;
        aim.rotation_z += 5;
        // pool.position.x = boat.position.x;
    }
    if (right || D) {
        boat.rotation -= 5;
        cannon.rotation -= 5;
        aim.rotation_z -= 5;
        // pool.position.x = boat.position.x;
    }
    if (up || W) {
        windTimer = 10;
        boat.position.y += boat.speed*cos(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 5:1);
        boat.position.x -= boat.speed*sin(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 5:1);
        pool.position.y = boat.position.y;
        if (cannon.position.z == level) {
            cannon.position.x = boat.position.x;
            cannon.position.y = boat.position.y;
        }
        aim.position.y = boat.position.y;
        aim.position.x = boat.position.x;

        int rck = 0;
        for (int i=0;i<rockCount ;i++)
            if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())) rck =1;
        if (rck){
            if (!gifttimer) health -=0.2;
            boat.position.y -=2* boat.speed*cos(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 5:1);
            boat.position.x += 2*boat.speed*sin(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 5:1);
            pool.position.y = boat.position.y;
            if (cannon.position.z == level) {
                cannon.position.x = boat.position.x;
                cannon.position.y = boat.position.y;
            }
            aim.position.y = boat.position.y;
            aim.position.x = boat.position.x;
        }
    }
    if (down || S) {
        boat.position.y -= boat.speed*cos(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 2:1);
        boat.position.x += boat.speed*sin(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 2:1);
        pool.position.y = boat.position.y;
        if (cannon.position.z == level) {
            cannon.position.x = boat.position.x;
            cannon.position.y = boat.position.y;
        }
        aim.position.y = boat.position.y;
        aim.position.x = boat.position.x;

        int rck = 0;
        for (int i=0;i<rockCount ;i++)
            if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())) rck =1;
        if (rck){
            if (!gifttimer) health -=0.2;
            boat.position.y += boat.speed*cos(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 2:1);
            boat.position.x -= boat.speed*sin(boat.rotation*M_PI/180.0f) * (gifttimer>0 ? 2:1);
            pool.position.y = boat.position.y;
            if (cannon.position.z == level) {
                cannon.position.x = boat.position.x;
                cannon.position.y = boat.position.y;
            }
            aim.position.y = boat.position.y;
            aim.position.x = boat.position.x;
        }
    }
    if (jump && boat.position.z == level) {
        boat.launch_speed = 6;
        cannon.launch_speed = 6;
        aim.launch_speed = 6;
    }
    if (fire && cannon.position.z == boat.position.z && aim.rotation_h > 10) {
        cannon.launch_speed = 10 * sin(aim.rotation_h*M_PI/180);
        cannon.launch_speed_x = -10 * cos(aim.rotation_h*M_PI/180) * sin(aim.rotation_z*M_PI/180);
        cannon.launch_speed_y = 10 * cos(aim.rotation_h*M_PI/180) * cos(aim.rotation_z*M_PI/180);
    }
    if (camera && time_cnt > 30) {
        change_camera();
        time_cnt = 0;
    }
}

void tick_elements() {
    if (health <= 0) exit(0);
    boat.tick();
    boat.shm();
    sail.position = boat.position;
    if (windAngle){
        boat.position.x += (rand()%2 + 1) * cos(windAngle*M_PI/180.0f) / 5.0f ;
        boat.position.y += (rand()%2 + 1) * sin(windAngle*M_PI/180.0f) / 5.0f ;
        cannon.position.x = boat.position.x;
        cannon.position.y = boat.position.y;
        aim.position.x = boat.position.x;
        aim.position.y = boat.position.y;
        int rck = 0;
        for (int i=0;i<rockCount ;i++)
            if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())) rck =1;
        if (rck){
            if (!gifttimer) health -=0.2;
            boat.position.y -= 2* (rand()%2 + 1) * cos(windAngle*M_PI/180.0f) / 5.0f;
            boat.position.x -= 2* (rand()%2 + 1) * sin(windAngle*M_PI/180.0f) / 5.0f;
            pool.position.y = boat.position.y;
            if (cannon.position.z == level) {
                cannon.position.x = boat.position.x;
                cannon.position.y = boat.position.y;
            }
            aim.position.y = boat.position.y;
            aim.position.x = boat.position.x;
        }
        for (int i = 0;i<patchCount ;i++){
            patches[i].position.x+=(rand()%2 + 1) * cos(windAngle*M_PI/180.0f) / 2.0f ;
            patches[i].position.y+=(rand()%2 + 1) * sin(windAngle*M_PI/180.0f) / 2.0f ;
        }
    }
    wind.position.x = boat.position.x - 7.0f*sin(boat.rotation*M_PI/180);
    wind.position.y = boat.position.y + 7.0f*cos(boat.rotation*M_PI/180) ;
    wind.rotation = boat.rotation;

    int angle = ((int)boat.rotation - (int)sail.rotation - (int)windAngle + 360 -65) % 360;
    if (angle > 90) sail.rotation -= 2;
    else sail.rotation +=2;
    // printf("%lf  %lf  %lf",boat.position.x,boat.position.y,boat.position.z);
    // camera_rotation_angle += 1;
    if (boat.position.z > level || cannon.position.z > level) {
        boat.launch_speed -= gravity;
        cannon.launch_speed -= gravity;
        aim.launch_speed -= gravity;
    }
    boat.position.z += boat.launch_speed;
    cannon.position.z += cannon.launch_speed;
    cannon.position.x += cannon.launch_speed_x;
    cannon.position.y += cannon.launch_speed_y;
    aim.position.z += aim.launch_speed;

    if (boat.position.z <= level) {
        boat.launch_speed = 0;
        boat.position.z = level;
        aim.launch_speed = 0;
        aim.position.z = level;
    }
//    if (monster[50].visible){
//        // printf("aa gya\n");
//        if (detect_collision(blast.bounding_box(), monster[50].bounding_box())){
//            fprintf(stderr, "fucked up!!\n");
//            monster[50].visible = 0;
//            kill_monster = 0;
//            gifts[50].visible = 1;
//            gifts[50].position = monster[50].position;
//        }
//    }
    if (flag && blast.size){
        for (int i = 0 ; i < monsterCount ; i++){
            if (monster[i].visible && detect_collision(blast.bounding_box(), monster[i].bounding_box())){
                monster[i].life --;
                if (monster[i].life) continue;
                total_monster++;
                monster[i].life = 1+rand()%2;
                kill_monster ++;
                if (i == 50){
                    kill_monster = 0;
                    monster[i].life = 3;
                }
                if (kill_monster > 3){
                    kill_monster = 0;
                    monster[50].visible = 1;
                }
                printf("%d\n",i);
                //printf ("%f %f %f\n%f",cannon.position.x,cannon.position.y,cannon.position.z, cannon.launch_speed);
                monster[i].visible = 0;
                gifts[i].visible = 1;
                gifts[i].position = monster[i].position;
                gifts[i].position.z +=5;
            }
        }
    }
    for (int i = 0 ; i < giftCount ; i++){
        if (gifts[i].visible && detect_collision(boat.bounding_box(), gifts[i].bounding_box())){
            printf("%d\n",i);
            //printf ("%f %f %f\n%f",cannon.position.x,cannon.position.y,cannon.position.z, cannon.launch_speed);
            gifts[i].visible = 0;
            if (i == 50){
                gifttimer = 1000;
                continue ;
            }
            monster[i].visible = 1;
            monster[i].position.x = boat.position.x + rand()%30 + 30;
            monster[i].position.y = boat.position.y + rand()%30 + 30;
            health = min(100, health +5);
        }
    }
    for (int i=0;i<logCount ; i++){
        if (detect_collision(boat.bounding_box(), barrel[i].jump_bounding_box()) &&
         !detect_collision(boat.bounding_box(), barrel[i].bounding_box())){
             barrel[i].visible = 0;
            barrel_gift[i].visible = 1;
            barrel_gift[i].position = barrel[i].position ;
            barrel_gift[i].position.z +=5;
         }
    }
    for (int i=0;i<logCount ; i++){
        if (barrel_gift[i].visible &&  detect_collision(boat.bounding_box(), barrel_gift[i].bounding_box())){
            barrel_gift[i].visible = 0;
            barrel[i].visible = 1;
            barrel[i].position.x = rand()%1000;
            barrel[i].position.y = rand()%1000;
            coins += 5;
        }
    }
    for (int i = 0 ;i <monsterCount;i++){
        if (monster[i].visible && detect_collision(boat.bounding_box(), monster[i].bounding_box())){
            health -= 1;
            monster[i].position.x += (rand()%2==0?1:-1)*50;
            monster[i].position.y += (rand()%2==0?1:-1)*50;
        }
    }
    for (int i = 0 ;i <monsterCount;i++){
        if (monster[i].visible){
            monster[i].launch_speed_x += ((rand()%2)==0?0.05:-0.05);
            monster[i].launch_speed_y += ((rand()%2)==0?0.05:-0.05);
            monster[i].launch_speed_x = min(1,monster[i].launch_speed_x);
            monster[i].launch_speed_x = max(-1, monster[i].launch_speed_x);
            monster[i].launch_speed_y = min(1,monster[i].launch_speed_y);
            monster[i].launch_speed_y = max(-1, monster[i].launch_speed_y);
            if (abs(monster[i].position.x - boat.position.x) < 600){
                monster[i].position.x += monster[i].launch_speed_x;
            }
            else{
                monster[i].position.x = boat.position.x + ((rand()%2)==0?1:-1)*(rand()%590 + 10) ;
            }
            if (abs(monster[i].position.y - boat.position.y) < 600){
                monster[i].position.y += monster[i].launch_speed_y;
            }
            else{
                monster[i].position.y = boat.position.y + ((rand()%2)==0?1:-1)*(rand()%590 + 10);
            }
        }
    }
    if (cannon.position.z < level) {
        flag = 1;
        blast.position.x = cannon.position.x;
        blast.position.y = cannon.position.y;
        cannon.launch_speed = 0;
        cannon.launch_speed_x = 0;
        cannon.launch_speed_y = 0;
        cannon.position.z = level;
        cannon.position.x = boat.position.x;
        cannon.position.y = boat.position.y;
    }
    speed_camera();
    sprintf(title_bar,"LOZ -- HEALTH: [%d] \t KILLED: [%d]\t AWESOME_LIFE: [%d]\t COINS: [%d windAngle: %f %d",(int)health, total_monster, gifttimer,coins,windAngle,windCounter);
    glfwSetWindowTitle(window,title_bar);
    time_cnt ++;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    ocean = Ocean(0,0,-7,COLOR_BLUE);
    boat = Boat(0, 0, COLOR_RED);
    boat.position.z += level;
    wind = Wind(0,0);
    pool = Pool(0,0, COLOR_BLUE);
    pool.position.z -= 32765;
    aim = Aim(0, 0, COLOR_RED);
    aim.position.z += level;
    cannon = Rock(boat.position.x,boat.position.y,COLOR_RED);
    cannon.position.z += level;
    blast = Rock(-1,-1,COLOR_BLAST);
    blast.size = 15;
    blast.position.z -= blast.size/2;
    sail = Sail(COLOR_SMOKE);
    for (int i = 0 ; i < rockCount ; i++) {
        float x = (rand()%2==0?1:-1)*(rand()%652 + 213);
        float y = (rand()%2==0?1:-1)*(rand()%850 + 153);
        rocks[i] = Rock(x , y, COLOR_BLACK);
        rocks[i].position.z += 1;
        rocks[i].size = (i%5 + 3 ) * 3;
    }
    for (int i= 0 ; i<patchCount ;i++){
        patches[i] = Patches(rand()%1000, rand()%1000, 0.0f, COLOR_DBLUE);
    }
    for (int i = 0 ; i < monsterCount ; i++) {
        // float x = (((i+1)*rand()+i*584)%10000)/10;
        // float y = (((i+1)*rand()+i*784)%10000)/10;
        float x = (rand()%2==0?1:-1)*rand()%785;
        float y = (rand()%2==0?1:-1)*rand()%865;
        if (i ==50){
            monster[50] = Monster((rand()%2==0?1:-1)*rand()%700 + 100, (rand()%2==0?1:-1)*rand()%700 + 100, COLOR_CUBE2);
            monster[50].position.z = 5;
            monster[50].size = 30;
            monster[i].life = 3;
            monster[50].visible = 0;
        }
        else{
            if (i%2){
                monster[i] = Monster(x , y, COLOR_GREEN);
                monster[i].position.z += 1;
                monster[i].life = 1;
                monster[i].size = (i%5 + 3)* 3;
            }
            else{
                monster[i] = Monster(x , y, COLOR_CUBE8);
                monster[i].life = 2;
                monster[i].position.z += 1;
                monster[i].size = (i%5 + 3)* 3;
            }
        }
        gifts[i] = Gift(x , y, COLOR_RED);
        gifts[i].position = monster[i].position;
        gifts[i].visible = 0;
        gifts[i].size = rand()%5 + 7;
    }
    for (int i=0;i<logCount;i++){
        barrel[i] = Barrel(rand()%500, rand()%500, 3, 10 , COLOR_BROWN);
        barrel_gift[i] = Gift(0,0,COLOR_RED);
        barrel_gift[i].position = barrel[i].position ;
        barrel_gift[i].size = 3;
        barrel_gift[i].visible = 0;
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    Matrices.Transparency = glGetUniformLocation(programID, "trans");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE);
    //glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);
    audio_init();
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        // audio_play();
        if (t60.processTick()) {
            windCounter--;
            windTimer --;
            if (windCounter == 0){
                windAngle = rand()%90 * (rand()%2==0?1:-1);
            }
            if (windCounter < 0){
                sail.rotation = windAngle * 180.0/ M_PI;
            }
            if (windCounter == (-200)){
                windAngle = 0;
                windCounter = rand()%500 + 200;
            }

            if (gifttimer) gifttimer --;
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.length + b.length )) &&
            (abs(a.z - b.z) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 15 / screen_zoom;
    float bottom = screen_center_y - 15 / screen_zoom;
    float left   = screen_center_x - 15 / screen_zoom;
    float right  = screen_center_x + 15 / screen_zoom;
    if (projectType) Matrices.projection = glm::ortho(3*left,3*right,3*bottom, 3*top, 0.01f, 500.0f);
    else Matrices.projection = glm::perspective(45.0f,1.0f,50.0f, 500.0f);
}
void change_camera(){
    view = (view+1)%5;
}

void speed_camera(){
    float theta = (boat.rotation)*(pi/180);
    if(view == 0) { //helicopter-view
        camera_z = 100;
    }
    else if(view == 1){ //follow-cam
        camera_x = boat.position.x+100*sin(theta);
        camera_y = boat.position.y-100*cos(theta);
        camera_z = 50;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }
    else if(view == 2){ //top-view
        camera_x = boat.position.x + 1;
        camera_y = boat.position.y;
        camera_z = boat.position.z+100;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }
    else if(view==3){ // boat view
        camera_x = boat.position.x;
        camera_y = boat.position.y;
        camera_z = 25;

        target_x = boat.position.x-25*sin(theta);
        target_y = boat.position.y+25*cos(theta);
        target_z = boat.position.z + 25;
        if (projectType) {
            camera_x = boat.position.x - 10 * sin(boat.rotation*M_PI/180.0f);
            camera_y = boat.position.y + 10 * cos(boat.rotation*M_PI/180.0f);
            camera_z = 5;
            target_z = 3;
        }
    }
    else if(view==4){ //tower-view
        camera_x = boat.position.x+100;
        camera_y = boat.position.y;
        camera_z = 50;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }

}

void heli_camera(float x, float y){
    if(view == 0) {
        camera_x = boat.position.x ;
        camera_y = boat.position.y - 100;
        camera_z = 100;

        target_x = boat.position.x + (x - 300)* 0.5;
        target_y = boat.position.y + (y - 300 )* -0.5 ;
        target_z = boat.position.z;
        if (projectType) {
            camera_z = 400;
        }

    }
}

void cannon_pos(float x, float y) {
    if (view != 0) {
        aim.rotation_z = boat.rotation-((3*x/5) - 180);
        aim.rotation_h = -(3*y/20) + 90;
    }
}
