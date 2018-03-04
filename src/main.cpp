#include "main.h"
#include "timer.h"
#include "cube.h"
#include "pool.h"
#include "rock.h"
#include "aim.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Cube boat;
Rock rocks[100];
Pool pool;
Rock cannon,blast;
Aim aim;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0 , gravity = 0.5 ,level = 3,pi=3.141;
float camera_x=0,camera_y=90,camera_z=100,target_x=0,target_y=90,target_z=0;
int rockCount = 100,view = 1, time_cnt = 0;
Timer t60(1.0 / 60);
void change_camera();
void speed_camera();
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
    pool.draw(VP);
    for(int i = 0 ; i < rockCount ; i++){
        rocks[i].draw(VP);
    }    
    boat.draw(VP);
    cannon.draw(VP);
    aim.draw(VP);
    // blast.draw(VP);
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
    int jump = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    int camera = glfwGetKey(window, GLFW_KEY_C);

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
        boat.position.y += boat.speed*cos(boat.rotation*M_PI/180.0f);
        boat.position.x -= boat.speed*sin(boat.rotation*M_PI/180.0f);
        pool.position.y = boat.position.y;
        if (cannon.position.z == level) {
            cannon.position.x = boat.position.x;        
            cannon.position.y = boat.position.y;        
        }
        aim.position.y = boat.position.y;        
        aim.position.x = boat.position.x;        
    }
    if (down || S) {
        boat.position.y -= boat.speed*cos(boat.rotation*M_PI/180.0f);
        boat.position.x += boat.speed*sin(boat.rotation*M_PI/180.0f);
        pool.position.y = boat.position.y;        
        if (cannon.position.z == level) {
            cannon.position.x = boat.position.x;        
            cannon.position.y = boat.position.y;        
        }
        aim.position.y = boat.position.y;        
        aim.position.x = boat.position.x;        
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
    boat.tick();
    // printf("%lf  %lf  %lf",boat.position.x,boat.position.y,boat.position.z);
    // camera_rotation_angle += 1;
    boat.position.z += boat.launch_speed;
    cannon.position.z += cannon.launch_speed;
    cannon.position.x += cannon.launch_speed_x;
    cannon.position.y += cannon.launch_speed_y;
    aim.position.z += aim.launch_speed;
    if (boat.position.z > level || cannon.position.z > level) {
        boat.launch_speed -= gravity;
        cannon.launch_speed -= gravity;
        aim.launch_speed -= gravity;
    }

    if (boat.position.z <= level) {
        boat.launch_speed = 0;
        boat.position.z = level;
        aim.launch_speed = 0;
        aim.position.z = level;
}

    if (cannon.position.z <= level) {
        cannon.launch_speed = 0;
        cannon.launch_speed_x = 0;
        cannon.launch_speed_y = 0;
        cannon.position.z = level;
        cannon.position.x = boat.position.x;
        cannon.position.y = boat.position.y;
    }
    speed_camera();
    time_cnt ++;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat = Cube(0, 0, COLOR_RED);
    boat.position.z += level;
    pool = Pool(0,0, COLOR_BLUE);
    pool.position.z -= 32767;
    for (int i = 0 ; i < rockCount ; i++) {
        float x = (((i+1)*rand()+i*584)%10000)/10;  
        float y = (((i+1)*rand()+i*784)%10000)/10;
        rocks[i] = Rock(x , y, COLOR_BLACK);
        rocks[i].position.z += 1;
        rocks[i].size = i%5 * 5;
    }
    aim = Aim(0, 0, COLOR_RED);
    aim.position.z += level;
    cannon = Rock(boat.position.x,boat.position.y,COLOR_RED);
    blast = Rock(-1,-1,COLOR_BLAST);
    blast.size = 15;
    cannon.position.z += level;
    blast.position.z -= blast.size/2;
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
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

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
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
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 15 / screen_zoom;
    float bottom = screen_center_y - 15 / screen_zoom;
    float left   = screen_center_x - 15 / screen_zoom;
    float right  = screen_center_x + 15 / screen_zoom;
    Matrices.projection = glm::perspective(45.0f,1.0f,50.0f, 500.0f);
}
void change_camera(){
    view = (view+1)%5;
}

void speed_camera(){
    float theta = (boat.rotation)*(pi/180);
    if(view == 0) { //helicopter-view
        camera_z = 50;
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
        target_z = boat.position.z+25;
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
        target_x = boat.position.x+(x-300)/2;
        if(y<=300){
            target_y = boat.position.y+(300-y)/2;
        }
    }
}

void cannon_pos(float x, float y) {
    if (view != 0) {
        aim.rotation_z = boat.rotation-((3*x/5) - 180);
        aim.rotation_h = -(3*y/20) + 90;
    }
}
// void zoom_camera(int type){
//     if(view==2){
//         float l = target_x-camera_x;
//         float m = target_y-camera_y;
//         float n = target_z-camera_z;
//         if(type==1){
//             if(camera_z-10>target_z)
//                 camera_z-=10;
//         }
//         else if(type==-1){
//             camera_z+=10;
//         }
//         camera_x = l*(camera_z-target_z)/n+target_x;
//         camera_y = m*(camera_z-target_z)/n+target_y;
//     }
// }
