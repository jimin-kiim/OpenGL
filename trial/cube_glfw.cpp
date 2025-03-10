// Includes
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
// to prevent an error, we should import glad beforehand other opengl libraries including glfw
#include <glad/glad.h> // loading openGL functions 
#include <GLFW/glfw3.h> // required for creating window, handling input
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
// #include <GLUT/glut.h> // GLUT is older one than glfw
#else
// #include <GL/glut.h> // legacy
#endif

// Function Prototypes
void display();
void key_callback();
// Global Variables
double rotate_y=0; 
double rotate_x=0;

double unit =  0.5;

void display() {
  // Clear screen and reset transformations
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Apply rotations
  glRotatef(rotate_x, 1.0, 0.0, 0.0);
  glRotatef(rotate_y, 0.0, 1.0, 0.0);

  // Render the cube (example for the front face)
  glBegin(GL_POLYGON);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(1.0, -1.0, -1.0);
  glVertex3f(1.0, 1.0, -1.0);
  glVertex3f(-1.0, 1.0, -1.0);
  glVertex3f(-1.0, -1.0, -1.0);
  glEnd();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      if (key == GLFW_KEY_RIGHT)
          rotate_y += 5;
      else if (key == GLFW_KEY_LEFT)
          rotate_y -= 5;
      else if (key == GLFW_KEY_UP)
          rotate_x += 5;
      else if (key == GLFW_KEY_DOWN)
          rotate_x -= 5;
  }
}

int main(void) {
  // Initialize GLFW
  if (!glfwInit()) {
      // Initialization failed
      return -1;
  }

  // Set GLFW window hints (optional, e.g., for OpenGL version)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Create a GLFW windowed window and its OpenGL context
  GLFWwindow* window = glfwCreateWindow(800, 600, "Awesome Cube", NULL, NULL);
  if (!window) {
      glfwTerminate();
      // Window creation failed
      return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      // GLAD initialization failed
      return -1;
  }

  // Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);

  // Set the key callback
  glfwSetKeyCallback(window, key_callback);

  // Main loop
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


