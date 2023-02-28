#include <iostream>
#include <vector>
#include <random>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
using namespace glm;

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define TAU 6.283185307


//========tweak variables==========
const int GRID_WIDTH     = 15;
const int GRID_HEIGHT    = 15;
const int GRID_DEPTH     = 15;
const int GRID_SIZE      = GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH;

const unsigned int MAX_PIPE_NUMBER = 10;
const unsigned int MAX_PIPE_LENGTH = 200;

const float PIPE_RADIUS = 0.03f;
const int NUM_SEGMENTS  = 10;

const int shininess     = 100;

//camera parameters
vec3 camera_position  = vec3(0.0f, 0.0f, 3.0f);
vec3 camera_front     = vec3(0.0f, 0.0f, -3.0f);
vec3 camera_top       = vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
float Yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float Pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

//mersenne twister
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist_big(0, GRID_SIZE - 1);
std::uniform_int_distribution<> dist_small(0, 100);
std::uniform_int_distribution<> dist_smallest(0, 14);
std::uniform_int_distribution<> dist_angle(0, 360);

//=================================

int current_palette = 0;

float current_time  = 0.0;
float previous_time = 0.0;
int count           = 0;
float delta_t       = 0.0;
    

#include "shader.h"
#include "colors.h"
#include "pipe.h"
#include "cube.h"
#include "editor.h"


int main () {
    glfwInit();

    //MAKE SURE THE FOLLOWING HAPPENS BEFORE CALLING ANY GLFW EXTENSIONS!!!!!!
    //failing to do so will result in the function pointers being null,
    //causing an access violation.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 //set openGL major... 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 //... and minor version...

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //as well as the core profile
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);  //set the number of samples for anti-aliasing purposes
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "pipes", NULL, NULL);
    glfwMakeContextCurrent(window);

    //set number of screen refreshes (0 means uncapped framerate!!)
    glfwSwapInterval(1);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //check that GLAD is loading properly
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glViewport(0, 0, 800, 800);

    
    glEnable(GL_MULTISAMPLE); //enable anti-aliasing...
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE); //face culling...
    //glCullFace(GL_FRONT);   //currently our faces are wound in the opposite way to convention
    
    //load shaders
    Shader shader_pipe("shaders/pipe.shader");



    //random generation
    int q = dist_angle(gen);
    int r = (q + 17) % 15;
    mat4 model = mat4(1.0f);
    model = rotate(model, radians((float)q), vec3(0.0f, 1.0f, 0.0f));

   
    mat4 view = lookAt(camera_position, camera_position + camera_front, camera_top);
    //view = translate(view, vec3(0.0f, 0.0f, -1.0f));
    
    mat4 projection = perspective(radians((float)(pow(-1, r)*r) + 45.0f), 1.0f, 0.1f, 100.0f);

    //set shader uniforms
    shader_pipe.use();
    shader_pipe.setMat4("model", model);
    shader_pipe.setMat4("view", view);
    shader_pipe.setMat4("projection", projection);
    shader_pipe.setVec3("camera_position", camera_position);
    shader_pipe.setInt("shininess", shininess);
    
    current_palette = q % 6;

    //initialise the grid
    for (int n = 0; n < GRID_SIZE; n++) {
	int x0 = (n % GRID_WIDTH);
	int y0 = ((int)floor(n / GRID_WIDTH)) % GRID_HEIGHT;
	int z0 = (int)(floor(n / (GRID_WIDTH * GRID_HEIGHT)));
	
	grid[n].grid_x = x0;
	grid[n].grid_y = y0;
	grid[n].grid_z = z0;
	grid[n].coordinates.x = x0 * 2 / ((float)GRID_WIDTH - 1) - 1;
	grid[n].coordinates.y = y0 * 2 / ((float)GRID_HEIGHT - 1) - 1;
	grid[n].coordinates.z = -(z0 * 2 / ((float)GRID_DEPTH - 1)) + 1;
	generate_neighbours(grid, n);
    }


    //setup the pipes
    for (int n = 0; n < MAX_PIPE_NUMBER; n++) {
        init_pipe(&pipe_set[n]);
    }

    pipe_set[0].active = true;


    
    //=========================================================================
    //============================main loop====================================
    //=========================================================================
    
    while (!glfwWindowShouldClose(window)) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	    glfwSetWindowShouldClose(window, true);
	}

	
	//framerate stuff
	
	current_time = glfwGetTime();
        delta_t = current_time - previous_time;
	previous_time = current_time;
	/*
	count ++;
	if (delta_t > 1.0f) {
	    printf("Framerate: %f ms/frame\n", (delta_t * 1000.0/count));
	    previous_time = current_time;
	    count = 0;
	}
	*/
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLineWidth(1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

        //draw_cube(shader_pipe);

	if (mode == ANIMATION_MODE::ANIMATION){
	    glfwSetCursorPosCallback(window, NULL);
	    process_input(window);
	    update_pipes();
	}

	if (mode == ANIMATION_MODE::EDITOR) {
	    glfwSetCursorPosCallback(window, mouse_callback);
	    process_input_for_editor(window);
	    shader_pipe.setVec3("camera_position", camera_position);
	    view = lookAt(camera_position, camera_position + camera_front, camera_top);
	    shader_pipe.setMat4("view", view);
	}

	for (int i = 0; i < MAX_PIPE_NUMBER; i++) {
	    if (pipe_set[i].active == true) {
		draw_pipe(&pipe_set[i], shader_pipe);
	    }
	}

	if (pipe_set[MAX_PIPE_NUMBER - 1].complete == true) {
	    //printf("resetting state...\n\n\n");
	    reset_state(model, projection, shader_pipe);
	    
	    pipe_set[0].active = true;
	}
	
	glfwSwapBuffers(window);
	glfwPollEvents();
    }
    
    glfwTerminate(); 
    return 0;
}
