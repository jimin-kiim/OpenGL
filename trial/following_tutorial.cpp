// Includes
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Function Prototypes
void display();
void specialKeys();
// Global Variables
double rotate_y=0; 
double rotate_x=0;