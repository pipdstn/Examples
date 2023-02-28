bool wireframe_active = false;

bool F1_being_pressed = false;
bool F2_being_pressed = false;

enum ANIMATION_MODE {
    ANIMATION,
    EDITOR
};

int mode = ANIMATION_MODE::ANIMATION;

void toggle_editor() {
    if (mode == ANIMATION_MODE::ANIMATION) {
	mode = ANIMATION_MODE::EDITOR;
    }
    else {mode = ANIMATION_MODE::ANIMATION;}
}

void toggle_wireframe () {
    if (!wireframe_active) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	wireframe_active = true;
    }
    else {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	wireframe_active = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if(Pitch > 89.0f){
	Pitch = 89.0f;
    }
    if(Pitch < -89.0f){
	Pitch = -89.0f;
    }

    vec3 direction;
    direction.x = cos(radians(Yaw)) * cos(radians(Pitch));
    direction.y = sin(radians(Pitch));
    direction.z = sin(radians(Yaw)) * cos(radians(Pitch));
    camera_front = normalize(direction);
}

void process_input(GLFWwindow* window) {
    if ((glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) && !F1_being_pressed) {
	toggle_wireframe();
	F1_being_pressed = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) && F1_being_pressed) {
	F1_being_pressed = false;
    }

    if ((glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) && !F2_being_pressed) {
	toggle_editor();
	F2_being_pressed = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) && F2_being_pressed) {
	F2_being_pressed = false;
    }

    
}

void process_input_for_editor(GLFWwindow* window) {
    float camera_speed = 1.0f * delta_t;
    if ((glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) && !F1_being_pressed) {
	F1_being_pressed = true;
	toggle_wireframe();
    }
    else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_RELEASE) {F1_being_pressed = false;}

    if ((glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) && !F2_being_pressed) {
	toggle_editor();
	F2_being_pressed = true;
    }
    if ((glfwGetKey(window, GLFW_KEY_F2) == GLFW_RELEASE) && F2_being_pressed) {
	F2_being_pressed = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	camera_position += camera_front * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
	camera_position -= normalize(cross(camera_front, camera_top)) * camera_speed; 
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
	camera_position -= camera_front * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	camera_position += normalize(cross(camera_front, camera_top)) * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
	camera_position += vec3(0.0f, 1.0f, 0.0f) * camera_speed;
    }
}

//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //  glViewport(0, 0, width, height);
//}
