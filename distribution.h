#ifndef _DISTRIBUTION_H
#define _DISTRIBUTION_H

//include header files
#include <iostream>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

// This appears to be in the same place on OSX
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// values used for the color constants in the program
#define RED			1
#define BLUE		2
#define GREEN		3
#define PURPLE		4
#define ORANGE		5
#define CYAN		6
#define YELLOW		7
#define WHITE		8
#define PINK		9
#define BLACK		10

#define EXP		18
#define NORMAL		19
#define ZEROONE		20
#define ZEROTWO		21
#define ZEROFIVE	22
#define THIRTY		30
#define FORTY		40
#define FIFTY		50

//define PI here
# define M_PI           3.14159265358979323846  /* pi */

//File names
char *currentFile;

//input data
float *data;
int numDataPoints;
float minimum, maximum;

// added for specific puposes
float intervalSize;

//histogram
int numIntervals = 30;
float *prob;
float maxProb = -1;

//theoretical distributions
int numCurvePoints = 100;
float *curveX = new float[numCurvePoints+1];
float *curveY = new float[numCurvePoints+1];

//parameters
float mu = 0, sigma = 1;
float lamda = 1;
float parameterStep = 0.05;

//determine our distribution
int dist;

//drawing parameters
int width = 800, height = 600;
float world_x_min, world_x_max, world_y_min, world_y_max;
float axis_x_min, axis_y_min, axis_x_max, axis_y_max;

//the color arrays
float hrgb[3] = { 1.0, 0.0, 1.0 }; //default purple
float lrgb[3] = { 1.0, 1.0, 0.0 }; //default yellow

//function declerations
void computeNormalFunc(float mu, float sigma);
void computeExponentialFunc(float lamda);
void computeMean();
void computeSD();
float sizeofIntervals(int numIntervals);
void display(void);
void init(void);
int findInterval(int numIntervals, float number, float intervalSize);
void computeProbability(int numIntervals);
void readFile(std::string fileName);
void keyboard(unsigned char key, int x, int y);
void specialInput(int key, int x, int y);
void funcMenuFunction(int id);
void histogramMenuFunction(int id);
void parameterStepMenuFunction(int id);
void histogramColorMenu(int id);
void distributionColorMenu(int id);
void createMenu();
void reshape(int w, int h);
void drawRect(float xmin, float xmax, float ymin, float ymax);
void menu(int num);

#endif //end of the header file
