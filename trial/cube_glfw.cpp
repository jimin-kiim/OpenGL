// #include <cube_glfw.hpp>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// to prevent an erroror, glad should be imported beforehand other packages like glfw..
#include <glad/glad.h>  // Loading OpenGL functions
#include <GLFW/glfw3.h> // Required for creating window, handling input
#include <glm/glm.hpp>  // GLM core
#include <glm/gtc/matrix_transform.hpp> // GLM transformations
#include <glm/gtc/type_ptr.hpp> // GLM to OpenGL types

#include <iostream>
#include <sstream> // for stringstream 
#include <fstream> // for ifstream

// Function Prototypes
void display();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

std::string readShaderSource(const char* filepath);
GLuint compileShader(const std::string& source, GLenum shaderType);
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath); 

void setupBuffers();
int main(void);

// Global Variables
float rotate_y = 0.0f;
float rotate_x = 0.0f;

// Shader Program
GLuint shaderProgram;
GLuint VAO, VBO, EBO;

// Cube vertices
GLfloat vertices[] = {
    // Positions          // Colors
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // Vertex 1 - red
     1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, // Vertex 2 - green 
    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f, // Vertex 3 - blue
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f  // Vertex 4 
    // Add other cube faces as needed
};

// Indices for the cube
GLuint indices[] = {
    0, 1, 2, 2, 3, 0 // Front face
    // Add other cube faces as needed
};

std::string readShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    std::stringstream shaderStream;

    if (!file.is_open()) {
        std::cout << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    shaderStream << file.rdbuf();
    file.close();

    return shaderStream.str();
}

// Function to compile a shader from source code
GLuint compileShader(const std::string& source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* shaderSource = source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for compilation errorors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader Compilation Error: " << infoLog << std::endl;
        return 0;
    }

    return shader;
}

// Function to compile shaders and create shader program
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {

    std::string vertexShaderSource = readShaderSource(vertexPath);
    std::string fragmentShaderSource = readShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errorors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Shader Program Linking Error: " << infoLog << std::endl;
        return 0;
    }

    // Delete the individual shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


void setupBuffers() {
    // Generate and bind Vertex Array Object, Vertex Binding Object, Element Binding Object  
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);
}

void display() {
    // Clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Initialize transformation matrices
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    model = glm::rotate(model, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotation around x-axis
    model = glm::rotate(model, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation around y-axis

    // Projection matrix: 45-degree field of view, 4:3 aspect ratio, display range 0.1 unit <-> 100 units
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // View matrix: camera at (4,3,3), looking at the origin (0,0,0), up direction (0,1,0)
    glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera position
        glm::vec3(0, 0, 0), // Look at origin
        glm::vec3(0, 1, 0)  // Up vector
    );

    // Combine the matrices
    glm::mat4 mvp = projection * view * model;

    // Pass the combined matrix to the shader
    GLuint mvpLocation = glGetUniformLocation(shaderProgram, "uMVPMatrix");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    // Bind VAO
    glBindVertexArray(VAO);

    // Draw the cube
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind VAO
    glBindVertexArray(0);

    // Unbind the shader program
    glUseProgram(0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_RIGHT)
            rotate_y += 5.0f;
        else if (key == GLFW_KEY_LEFT)
            rotate_y -= 5.0f;
        else if (key == GLFW_KEY_UP)
            rotate_x += 5.0f;
        else if (key == GLFW_KEY_DOWN)
            rotate_x -= 5.0f;
    }
}

// int InitWindow() {
    
// }

// int InitGLAD() {

// }

// chatGPT 
const char* GetGLErrorString(GLenum errorCode)
{
    switch (errorCode)
    {
        case GL_NO_ERROR:              return "GL_NO_ERROR";
        case GL_INVALID_ENUM:         return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:        return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:    return "GL_INVALID_OPERATION";
        // case GL_STACK_OVERFLOW:       return "GL_STACK_OVERFLOW";
        // case GL_STACK_UNDERFLOW:      return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:        return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:                      return "Unknown OpenGL error";
    }
}

int main(void) {

    std::cout << "In - main" << std::endl; // to flush the buffer for output verification 
    // Initialize GLFW
    glfwInit();
    GLenum error =glGetError();
    std::cout << "Error verification result using glGetError()" << GetGLErrorString(error) << std::endl;
    
    while ((error= glGetError()) != 0) {
        // if (error) {
    //     std::cout << "erroror occurred after initiating glfw" << std::endl; 
    // } else {
    //     std::cout << "on erroror during glfw intialization" << std::endl; 
    // }
    // if (!glfwInit()) {
    //     // Initialization failed
    //     std::cerror << "Failed to initialize glfw" << std::endl;
    //     return -1;
    // } else {
    //     std::cout << "Successfully initizalized glfw";
    // }

    // Setting window hints; should be done before creating Window 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW windowed window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Awesome Cube", NULL, NULL);
    error = glGetError();
    if (!window) { // when failed to create window, NULL is stored in window. 
        glfwTerminate();
        std::cout << "Failed to create window" << std::endl;
        // Window creation failed
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    error = glGetError();
    if (error == GL_NO_ERROR) {
        std::cout << "Window is set as the current context." << std::endl;
    } else {
        std::cout << "Making Context Current failed." << error << std::endl;
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        // GLAD initialization failed
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable Z-buffer depth test; Z buffer is how far from the camera. 
    // determine the locations of object by comparing the Z-buffer values.
    glEnable(GL_DEPTH_TEST);

    // Compile shaders and create shader program
    // shaderProgram = createShaderProgram();
    // Create the shader program
    GLuint shaderProgram = createShaderProgram("cube.vsh", "cube.fsh");
    if (shaderProgram == 0) {
        // Shader program creation failed
        std::cout << "Failed to create shader programs" << std::endl;
        return -1;
    }

    // Set the key callback
    glfwSetKeyCallback(window, key_callback);

    // Main loop - glutMainLoop() in glut 
    while (!glfwWindowShouldClose(window)) {
        // Render here
        display();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
    }
    
}
