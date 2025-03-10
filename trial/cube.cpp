// Includes
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <glad/glad.h> // loading openGL functions 
#include <GLFW/glfw3.h> // required for creating window, handling input
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
// #include <GLUT/glut.h> // GLUT is older one than glfw
#else
#include <GL/glut.h>
#endif

// Function Prototypes
void display_cubemap_not_recommende();
void display_cubemap_glut();
void display();
void specialKeys();
// Global Variables
double rotate_y=0; 
double rotate_x=0;

double unit =  0.5;


void display_cubemap_not_recommended(){

  //  Clear screen; existing drawings and resetting Z-buffer; depth - distance from the camera
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // every drawing is staring with glBegin and ending with glEnd

  /* 
  // Multi-colored side - FRONT
  glBegin(GL_POLYGON);
  // GL_POLYGON, GL_LINE, GL_QUAD.. 

  // Defining Vertices 
  // in counter clock wise 
  glVertex3f( -1*unit, -1*unit, -1*unit);       // Point 1
  glVertex3f( -1*unit,  1*unit, -1*unit);       // Point 2
  glVertex3f(  1*unit,  1*unit, -1*unit);       // Point 3
  glVertex3f(  1*unit, -1*unit, -1*unit);       // Point 4

  glEnd();
*/
  // Reset transformations
  glLoadIdentity();

  // Rotate when user changes rotate_x and rotate_y
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );


  //Multi-colored side - FRONT
  glBegin(GL_POLYGON);
  // 3: 3 dimension
  // f: floating point 
  // color: 0 to 1. all 0s: black 
  glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  1*unit, -1*unit, -1*unit );      // P1 is red
  glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  1*unit,  1*unit, -1*unit );      // P2 is green
  glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -1*unit,  1*unit, -1*unit );      // P3 is blue
  glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -1*unit, -1*unit, -1*unit );      // P4 is purple

  glEnd();

// White side - BACK
  glBegin(GL_POLYGON);
  glColor3f(   1.0,  1.0, 1.0 ); // white 
  glVertex3f(  1*unit, -1*unit, 1*unit );
  glVertex3f(  1*unit,  1*unit, 1*unit );
  glVertex3f( -1*unit,  1*unit, 1*unit );
  glVertex3f( -1*unit, -1*unit, 1*unit );
  glEnd();

  // Purple side - RIGHT
  glBegin(GL_POLYGON);
  glColor3f(  1.0,  0.0,  1.0 );
  glVertex3f( 1*unit, -1*unit, -1*unit );
  glVertex3f( 1*unit,  1*unit, -1*unit );
  glVertex3f( 1*unit,  1*unit,  1*unit );
  glVertex3f( 1*unit, -1*unit,  1*unit );
  glEnd();

  // Green side - LEFT
  glBegin(GL_POLYGON);
  glColor3f(   0.0,  1.0,  0.0 );
  glVertex3f( -1*unit, -1*unit,  1*unit );
  glVertex3f( -1*unit,  1*unit,  1*unit );
  glVertex3f( -1*unit,  1*unit, -1*unit );
  glVertex3f( -1*unit, -1*unit, -1*unit );
  glEnd();

  // Blue side - TOP
  glBegin(GL_POLYGON);
  glColor3f(   0.0,  0.0,  1.0 );
  glVertex3f(  1*unit,  1*unit,  1*unit );
  glVertex3f(  1*unit,  1*unit, -1*unit );
  glVertex3f( -1*unit,  1*unit, -1*unit );
  glVertex3f( -1*unit,  1*unit,  1*unit );
  glEnd();

  // Red side - BOTTOM
  glBegin(GL_POLYGON);
  glColor3f(   1.0,  0.0,  0.0 );
  glVertex3f(  1*unit, -1*unit, -1*unit );
  glVertex3f(  1*unit, -1*unit,  1*unit );
  glVertex3f( -1*unit, -1*unit,  1*unit );
  glVertex3f( -1*unit, -1*unit, -1*unit );
  glEnd();

  // Other transformations
  glTranslatef( 0.1, 0.0, 0.0 );
  glRotatef( 180, 0.0, 1.0, 0.0 );
  glScalef( 2.0, 2.0, 0.0 );

  glFlush();
  glutSwapBuffers();
}

void display_cubemap_glut() {
  GLuint cubemap;
  glGenTextures(1, &cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

  // Allocate storage for each face
  for (GLuint i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  }

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
  // settig GL_TEXTURE_MIN_FILTER as GL_LINEAR; using linear interpolation 
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
  // setting GL_TEXTURE_WRAP_S, T, R as GL_CLAMP_TO_EDGE; 
  // S, T, R represents X,Y, W axes used in texture coordinates. 
  // GL_CLAMP_TO_EDGE is for making  seams to be natural as much as possible 
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  
  /* 
  //using FBO to improve rendering
  GLuint fbo;
  glGenFramebuffers(1,&fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  */


  // using fish eye pictures to cubemap
  /*
  vec3 dir;
  dir.x = cos(phi) * sin(theta);
  dir.y = sin(phi);
  dir.z = cos(phi) * cos(theta);

  dir = normalize(faceDir + dir);  // Align with cubemap face

  vec2 uv = vec2(0.5 + 0.5 * dir.x, 0.5 + 0.5 * dir.y);
  FragColor = texture(fisheyeTexture, uv);

  glm::vec3 faceDirs[6] = {
  glm::vec3(1, 0, 0),  // Right
  glm::vec3(-1, 0, 0), // Left
  glm::vec3(0, 1, 0),  // Up
  glm::vec3(0, -1, 0), // Down
  glm::vec3(0, 0, 1),  // Front
  glm::vec3(0, 0, -1)  // Back
  };

  for (int i = 0; i < 6; i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  */
}

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

void specialKeys_glut( int key, int x, int y ) {

//  Right arrow - increase rotation by 5 degree
if (key == GLUT_KEY_RIGHT)
  rotate_y += 5;
  
//  Left arrow - decrease rotation by 5 degree
else if (key == GLUT_KEY_LEFT)
  rotate_y -= 5;

else if (key == GLUT_KEY_UP)
  rotate_x += 5;

else if (key == GLUT_KEY_DOWN)
  rotate_x -= 5;
  
//  Request display update
glutPostRedisplay();
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

int main_glut(int argc, char* argv[]){

//  Initialize GLUT and process user parameters
glutInit(&argc,argv);
  
//  Request double buffered true color window with Z-buffer
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

// Create window
glutCreateWindow("Awesome Cube");

//  Enable Z-buffer depth test
glEnable(GL_DEPTH_TEST);

// Callback functions
glutDisplayFunc(display_cube_map_glut);
glutSpecialFunc(specialKeys);

//  Pass control to GLUT for events
glutMainLoop();

//  Return to OS
return 0;

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


