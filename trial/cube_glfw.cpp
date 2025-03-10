// Includes
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
// to prevent an error, glad should be imported beforehand other packages like glfw..
#include <glad/glad.h>  // Loading OpenGL functions
#include <GLFW/glfw3.h> // Required for creating window, handling input
#include <glm/glm.hpp>  // GLM core
#include <glm/gtc/matrix_transform.hpp> // GLM transformations
#include <glm/gtc/type_ptr.hpp> // GLM to OpenGL types

// Function Prototypes
void display();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Global Variables
float rotate_y = 0.0f;
float rotate_x = 0.0f;

// Shader Program
GLuint shaderProgram;

// Function to compile shaders and create shader program (implementation not shown)
GLuint createShaderProgram();

// Vertex Array Object and Vertex Buffer Object
GLuint VAO, VBO;


// Cube vertices
GLfloat vertices[] = {
    // Positions          // Colors
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, // Vertex 1
     1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, // Vertex 2
    -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f, // Vertex 3
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f  // Vertex 4
    // Add other cube faces as needed
};

// Indices for the cube
GLuint indices[] = {
    0, 1, 2, 2, 3, 0 // Front face
    // Add other cube faces as needed
};

// Vertex Shader Source
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
uniform mat4 uMVPMatrix;
void main()
{
    gl_Position = uMVPMatrix * vec4(aPos, 1.0);
    ourColor = aColor;
}
)";

// Fragment Shader Source
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)";


// Function to compile shaders and create shader program
GLuint createShaderProgram()
{
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compilation errors...

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compilation errors...

    // Shader Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // Check for linking errors...

    // Clean up shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


void setupBuffers()
{
    // Generate and bind Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind Element Buffer Object
    GLuint EBO;
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

void display()
{
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
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

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        // Initialization failed
        return -1;
    }

    // Set GLFW window hints for OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW windowed window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Awesome Cube", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        // Window creation failed
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // GLAD initialization failed
        return -1;
    }

    // Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    // Compile shaders and create shader program
    shaderProgram = createShaderProgram();

    // Set the key callback
    glfwSetKeyCallback(window, key_callback);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
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
