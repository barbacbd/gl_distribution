#include "distribution.h"
#include <string.h>
#if defined(__linux__) || defined(__APPLE__)
#define sprintf_s snprintf
#endif



using namespace std;

//compute all the points for the normal distribution
void computeNormalFunc(float mu, float sigma)
{
	float yval, xval;

	for (int i = 0; i <= numCurvePoints; i++)
	{
		xval = (i * ((maximum - minimum) / numCurvePoints)) + minimum;
		yval = (1 / (sigma * sqrt(2 * M_PI))) * exp(-pow((xval - mu), 2.0) / (2 * pow(sigma, 2.0))); 
		curveX[i] = xval;
		curveY[i] = yval;
	}
}

//compute all the points for the exponential distribution
void computeExponentialFunc(float lamda)
{
	float xval, yval;

	for (int i = 0; i <= numCurvePoints; i++)
	{
		xval = (i * ((maximum - minimum) / numCurvePoints)) + minimum;
		yval = (1 / lamda) * exp(-xval / lamda);
		curveX[i] = xval;
		curveY[i] = yval;
	}
}

/* Compute the mu or mean of the data points in the
event that it is a normal distribution

Total all of the points and divide by the number of
points. (Average)
*/
void computeMean()
{
	float total = 0.0;
	int n = 0;
	while (numDataPoints > n)
	{
		total += data[n];
		n++;
	}
	mu = float(total / numDataPoints);
	lamda = mu;
}

/* Compute the standard deviation in the event that we
are dealing with a normal distribution

The standard deviation is completed by first calculating the
variance, and then finding the square root of the variance.
*/
void computeSD()
{
	float total = 0;
	int n = 0;
	while (numDataPoints > n)
	{
		total += pow((data[n] - mu), 2.0);
		n++;
	}
	sigma = float(sqrt(total / (numDataPoints - 1)));
}

/* Find the range for the intervals based on the
maximum, minimum, and number of intervals
*/
float sizeofIntervals(int numIntervals)
{
	return ((maximum - minimum) / numIntervals);
}

/*
	Display the histogram, axes, distribution, and
	the text on the screen. 
*/
void display(void)
{
	int i;
	char temp[300];
	float shift = .05;
	//clear all pixels 
	glClear(GL_COLOR_BUFFER_BIT);
	
	//reset modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glLineWidth(1); // use the same line width for the histogram and the axes
	//draw probability histogram	
	/* the color will be able to change */
	glColor3f(hrgb[0], hrgb[1], hrgb[2]);
	for (i = 0; i < numIntervals; i++)
	{
		float xmin = (((i + 1) * intervalSize) + minimum);
		float xmax = (((i + 2) * intervalSize) + minimum);
		drawRect(xmin, xmax, 0.0, prob[i]);
	}

	/* I have chosen to draw the axes after the histogram so that the axes are able to be seen because
	if the histogram is drawn before it will cover the axes when it is 0.*/
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(axis_x_min, axis_y_min, 0.0);
		glVertex3f(axis_x_max, axis_y_min, 0.0);
		glVertex3f(axis_x_min, axis_y_min, 0.0);
		glVertex3f(axis_x_min, axis_y_max, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(axis_x_min, maxProb, 0.0);
		glVertex3f(axis_x_min + (world_x_max - axis_x_min) * 0.01, maxProb, 0.0);
	glEnd();

	glRasterPos2f(axis_x_min + (world_x_max - axis_x_min) * 0.02, maxProb);
	sprintf_s(temp, sizeof(temp), "%f", maxProb);	
	for (i = 0; i < (int)strlen(temp); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);

	glRasterPos2f(axis_x_min + (world_x_max - axis_x_min) * 0.02, (axis_y_max));
	sprintf_s(temp, sizeof(temp), "%s", "Probability Density");
	for (i = 0; i < (int)strlen(temp); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);

	glRasterPos2f(axis_x_max - (world_x_max - axis_x_max), (world_y_min - axis_y_min) / 2);
	sprintf_s(temp, sizeof(temp), "%s", "Data");
	for (i = 0; i < (int)strlen(temp); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);

	//draw distribution
	glLineWidth(1.5);
	glColor3f(lrgb[0], lrgb[1], lrgb[2]);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= numCurvePoints; i++)
	{

		glVertex3f(curveX[i], curveY[i], 0.0);
	}
	glEnd();

	glColor3f(hrgb[0], hrgb[1], hrgb[2]);
	sprintf_s(temp, sizeof(temp), "File: %s\nMin: %g\nMax: %f\nNum of Intervals: %d", currentFile, minimum, maximum, numIntervals);
	glRasterPos2f((world_x_max - (world_x_max - world_x_min) * .3), (world_y_max - (world_y_max - world_y_min) * shift));
	for (i = 0; i < (int)strlen(temp); i++)
	{
		if (temp[i] == '\n')
		{
			shift += 0.025;
			glRasterPos2f((world_x_max - (world_x_max - world_x_min) * .3), (world_y_max - (world_y_max - world_y_min) * shift));
		}
		else
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);
	}
	
	//draw theoretical distributions
	glColor3f(lrgb[0], lrgb[1], lrgb[2]);
	shift += 0.05;
	glRasterPos2f((world_x_max - (world_x_max - world_x_min) * .3), (world_y_max - (world_y_max - world_y_min) * shift));
	sprintf_s(temp, sizeof(temp), "Distribution: %s", ((dist) ? "Exponential" : "Normal"));
	for (i = 0; i < (int)strlen(temp); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);
	shift += 0.025;

	if (dist)
	{
		glRasterPos2f((world_x_max - (world_x_max - world_x_min) * .3), (world_y_max - (world_y_max - world_y_min) * shift));
		sprintf_s(temp, sizeof(temp), "Beta: %.2f", lamda);
		for (i = 0; i < (int)strlen(temp); i++)
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);
	}
	else
	{
		glRasterPos2f((world_x_max - (world_x_max - world_x_min) * .3), (world_y_max - (world_y_max - world_y_min) * shift));
		sprintf_s(temp, sizeof(temp), "Mu: %.1f\nSigma: %.1f", mu, sigma);
		for (i = 0; i < (int)strlen(temp); i++)
		{
			if (temp[i] == '\n')
			{
				shift += 0.025;
				glRasterPos2f((world_x_max - (world_x_max - world_x_min) * .3), (world_y_max - (world_y_max - world_y_min) * shift));
			}
			else
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, temp[i]);
		}
	}

	glFlush();
}

void init (void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

/* 
	Determine the interval that is associated with the number

	It is important to note that this function will return the 
	position in the array that should be updated. --
	Return interval - 1, so that it looks more appropriate when 
	setting the interval position; i.e. array[interval] not
	array[interval-1].
*/
int findInterval(int numIntervals, float number, float intervalSize)
{
	for (int i = 1; i <= numIntervals; i++)
	{
		if (number < ((float(i) * intervalSize) + minimum))
		{
			return i-1;
		}
	}
	return numIntervals - 1; //default case we return the last interval
}

/* 
	Compute the probability by finding the intervals
	and determining which bin each data point fall into.
	We divide that number by the number of data points, and 
	we divide the result by the interval size to give us the
	area. 
*/
void computeProbability(int numIntervals)
{
	int interval, i, highInterval;

	//delete previously allocated memory
	free(prob);

	prob = new float[numIntervals];
	memset(prob, 0.0, sizeof(float)* numIntervals);
	intervalSize = sizeofIntervals(numIntervals);

	//reset maxProb so we dont accidently have the same maxProb
	maxProb = -1;

	//find the appropriate interval for each data point
	for (i = 0; i < numDataPoints; i++)
	{
		interval = findInterval(numIntervals, data[i], intervalSize);
		prob[interval] += 1;
	}

	//compute the probability for each interval(update the array prob)
	for (i = 0; i < numIntervals; i++)
	{
		prob[i] = (prob[i] / numDataPoints) / intervalSize;
		//check to see if we have a new highest probability
		if (prob[i] > maxProb)
		{
			maxProb = prob[i];
			highInterval = i;
		}
	}
	//take a best guess at which distribution to use -- if wrong the user can change it
	dist = (highInterval <= 2 || highInterval >= (numIntervals - 3)) ? 1 : 0;

	//set the world coordinates
	world_x_min = minimum - (maximum - minimum) * .10;
	world_x_max = maximum + (maximum - minimum) * .10;
	world_y_min = -(maxProb * .1);
	world_y_max = maxProb + (maxProb * .1);

	//set the axis coordinates
	axis_x_min = minimum - (maximum - minimum) * .05;
	axis_x_max = maximum + (maximum - minimum) * .05;
	axis_y_min = 0.0;
	axis_y_max = maxProb + (maxProb * 0.05);

	reshape(width, height);
}

/* This is a helper function to
draw the rectangles of the histogram

Note that we are not closing the rectangle. It will
save us one operation. The bottom line will not exist
because the (x,y) plane will be connected to the bottom line
so there is no reason to have the other line present
*/
void drawRect(float xmin, float xmax, float ymin, float ymax)
{
	glBegin(GL_LINE_STRIP);
		glVertex3f(xmin, ymin, 0.0);
		glVertex3f(xmin, ymax, 0.0);
		glVertex3f(xmax, ymax, 0.0);
		glVertex3f(xmax, ymin, 0.0);
	glEnd();
}

/*
	Open the file, and read in the data. This function
	will read the data and place the data into the array
	data[]. The function will also set the minimum, maximum
	and number of data points. 
*/
void readFile(string fileName)
{
	string line; 
	ifstream inFile(fileName.c_str());
	
	if(!inFile.is_open())
	{
		cout << fileName << " couldn't be opened.\n";
		system("pause");
		exit(1);
	}
	
	getline(inFile, line);
	numDataPoints = atoi(line.c_str());		//convert the first number to an integer for storing floats
	
	//Memory management
	if (data != NULL)
		delete data;
	//data = new float[numDataPoints];
	data = (float *)malloc(numDataPoints * sizeof(float));
	
	//set up the default values for max and min
	minimum = FLT_MAX;
	maximum = -FLT_MAX;
	
	int n = 0;
	//read in all of the data and put it into the appropriate 
	//cell of the array. Update the position at the end of 
	//each addition to the array
	while (getline(inFile, line))
	{
		data[n] = atof(line.c_str());
		if(data[n] < minimum)
			minimum = data[n];
		if(data[n] > maximum)
			maximum = data[n];
		n++;
	}
	
	//compute the histogram
	computeProbability(numIntervals);

	//compute the theoretical distribution
	computeMean();
	if (dist)
	{
		computeExponentialFunc(lamda);
	}
	else
	{
		computeSD();
		computeNormalFunc(mu, sigma);
	}

	inFile.close();
}

/* Quit the program when one of these buttons is pushed */
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q' || key == 27)
		exit(0);
}

/* 
	Update the distribution on the screen by changing
	beta, mu, or sigma depending on which distribution
	is currently dispalyed. Update the screen to show the 
	new display.
*/
void specialInput(int key, int x, int y) //for the arrow keys
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (dist) lamda += parameterStep;
		else sigma += parameterStep;	
		break;
	case GLUT_KEY_DOWN:
		if (dist) lamda -= parameterStep;
		else sigma -= parameterStep;
		break;
	case GLUT_KEY_LEFT:
		if (!dist) mu -= parameterStep;
		break;
	case GLUT_KEY_RIGHT:
		if (!dist) mu += parameterStep;
		break;
	}
	if (dist) computeExponentialFunc(lamda);
	else computeNormalFunc(mu, sigma);

	glutPostRedisplay();
}

/* 
	Change the distribution type on the screen
	and update the global variable for the 
	distribution.
*/
void funcMenuFunction(int id)
{
	if (id == EXP)
	{
		computeExponentialFunc(lamda);
		dist = 1;
	}
	else if (id == NORMAL)
	{
		computeNormalFunc(mu, sigma);
		dist = 0;
	}
}

/* Change the number of intervals through the menu*/
void histogramMenuFunction(int id)
{
	//update the number of intervals and recompute the histogram
	numIntervals = id;
	computeProbability(id);
}

/*  Change the parameter step */
void parameterStepMenuFunction(int id)
{
	//update the parameter step size
    if (id == ZEROONE){
        parameterStep = 0.01;
    } else if (id == ZEROTWO) {
        parameterStep = 0.02;
    } else if (id == ZEROFIVE) {
        parameterStep = 0.05;
    }
}

/*
	The function will allow the user to change the color
	of the histogram on the screen. The parameter will
	be associated with a defined color.
*/
void histogramColorMenu(int id)
{
	switch (id)
	{
	case RED:
		hrgb[0] = 1.0;
		hrgb[1] = 0.0; 
		hrgb[2] = 0.0;
		break;
	case BLUE:
		hrgb[0] = 0.0;
		hrgb[1] = 0.0;
		hrgb[2] = 1.0;
		break;
	case GREEN:
		hrgb[0] = 0.0;
		hrgb[1] = 1.0;
		hrgb[2] = 0.0;
		  break;
	case PURPLE:
		hrgb[0] = 1.0;
		hrgb[1] = 0.0;
		hrgb[2] = 1.0;
		break;
	case ORANGE:
		hrgb[0] = 1.0;
		hrgb[1] = 0.5;
		hrgb[2] = 0.0;
		break;
	case CYAN:
		hrgb[0] = 0.0;
		hrgb[1] = 1.0;
		hrgb[2] = 1.0;
		break;
	case YELLOW:
		hrgb[0] = 1.0;
		hrgb[1] = 1.0;
		hrgb[2] = 0.0;
		break;
	case WHITE:
		hrgb[0] = 1.0;
		hrgb[1] = 1.0;
		hrgb[2] = 1.0;
		break;
	case PINK:
		hrgb[0] = 0.737255;
		hrgb[1] = 0.560784;
		hrgb[2] = 0.560784;
		break;
	case BLACK:
		hrgb[0] = 0.0;
		hrgb[1] = 0.0;
		hrgb[2] = 0.0;
		break;
	default:
		hrgb[0] = 1.0;
		hrgb[1] = 0.0;
		hrgb[2] = 1.0;
		break;
	}
}

/*
	The function will allow the user to change the color
	of the distribution on the screen. The parameter will
	be associated with a defined color.
*/
void distributionColorMenu(int id)
{
	switch (id)
	{
	case (RED + 50):
		lrgb[0] = 1.0;
		lrgb[1] = 0.0;
		lrgb[2] = 0.0;
		break;
	case (BLUE + 50):
		lrgb[0] = 0.0;
		lrgb[1] = 0.0;
		lrgb[2] = 1.0;
		break;
	case (GREEN + 50):
		lrgb[0] = 0.0;
		lrgb[1] = 1.0;
		lrgb[2] = 0.0;
		break;
	case (PURPLE + 50):
		lrgb[0] = 1.0;
		lrgb[1] = 0.0;
		lrgb[2] = 1.0;
		break;
	case (ORANGE + 50):
		lrgb[0] = 1.0;
		lrgb[1] = 0.5;
		lrgb[2] = 0.0;
		break;
	case (CYAN + 50):
		lrgb[0] = 0.0;
		lrgb[1] = 1.0;
		lrgb[2] = 1.0;
		break;
	case (YELLOW + 50):
		lrgb[0] = 1.0;
		lrgb[1] = 1.0;
		lrgb[2] = 0.0;
		break;
	case (WHITE + 50):
		lrgb[0] = 1.0;
		lrgb[1] = 1.0;
		lrgb[2] = 1.0;
		break;
	case (PINK + 50):
		lrgb[0] = 0.737255;
		lrgb[1] = 0.560784;
		lrgb[2] = 0.560784;
		break;
	case (BLACK + 50):
		lrgb[0] = 0.0;
		lrgb[1] = 0.0;
		lrgb[2] = 0.0;
		break;
	default:
		lrgb[0] = 1.0;
		lrgb[1] = 1.0;
		lrgb[2] = 0.0;
		break;
	}
}

/* split the menu into the 
correct functions
*/
void menu(int num)
{
	if (num == 0)
		exit(0);
	else
	{
		switch (num)
		{
			case BLUE:
			case BLACK:
			case CYAN:
			case GREEN:
			case ORANGE:
			case PINK:
			case PURPLE:
			case RED:
			case WHITE:
			case YELLOW:
				histogramColorMenu(num);
				break;
			case (BLUE + 50):
			case (BLACK + 50):
			case (CYAN + 50):
			case (GREEN + 50):
			case (ORANGE + 50):
			case (PINK + 50):
			case (PURPLE + 50):
			case (RED + 50):
			case (WHITE + 50):
			case (YELLOW + 50):
				distributionColorMenu(num);
				break;
			case EXP:
			case NORMAL:
				funcMenuFunction(num);
				break;
			case THIRTY:
			case FORTY:
			case FIFTY:
				histogramMenuFunction(num);
				break;
			case ZEROONE:
			case ZEROTWO:
			case ZEROFIVE:
				parameterStepMenuFunction(num);
				break;
			default:
				break;
		}
	}
	glutPostRedisplay();
}

/* 
	Create the menu and all of the submenus that 
	the user should be able to select from.
*/
void createMenu()
{
	int subcolor_id, subdist_id, subhist_id, subparam_id, menu_id, subcolorline_id;
	subcolor_id = glutCreateMenu(menu);
	glutAddMenuEntry("Black", BLACK);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Cyan", CYAN);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Orange", ORANGE);
	glutAddMenuEntry("Pink", PINK);
	glutAddMenuEntry("Purple", PURPLE);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("White", WHITE);
	glutAddMenuEntry("Yellow", YELLOW);
	subcolorline_id = glutCreateMenu(menu);
	glutAddMenuEntry("Black", (BLACK+50));
	glutAddMenuEntry("Blue", (BLUE + 50));
	glutAddMenuEntry("Cyan", (CYAN + 50));
	glutAddMenuEntry("Green", (GREEN + 50));
	glutAddMenuEntry("Orange", (ORANGE + 50));
	glutAddMenuEntry("Pink", (PINK + 50));
	glutAddMenuEntry("Purple", (PURPLE + 50));
	glutAddMenuEntry("Red", (RED + 50));
	glutAddMenuEntry("White", (WHITE + 50));
	glutAddMenuEntry("Yellow", (YELLOW + 50));
	subdist_id = glutCreateMenu(menu);
	glutAddMenuEntry("Exponential", EXP);
	glutAddMenuEntry("Normal", NORMAL);
	subhist_id = glutCreateMenu(menu);
	glutAddMenuEntry("30", THIRTY);
	glutAddMenuEntry("40", FORTY);
	glutAddMenuEntry("50", FIFTY);
	subparam_id = glutCreateMenu(menu);
	glutAddMenuEntry("0.01", ZEROONE);
	glutAddMenuEntry("0.02", ZEROTWO);
	glutAddMenuEntry("0.05", ZEROFIVE);
	menu_id = glutCreateMenu(menu);
	glutAddSubMenu("Distribution", subdist_id);
	glutAddSubMenu("Histogram", subhist_id);
	glutAddSubMenu("Parameter Step", subparam_id);
	glutAddSubMenu("Histogram Color", subcolor_id);
	glutAddSubMenu("Distribution Color", subcolorline_id);
	glutAddMenuEntry("Exit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// The function should be called when the window size changes.
void reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, GLsizei (w), GLsizei (h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(world_x_min, world_x_max, world_y_min, world_y_max);
}

int main(int argc, char **argv)
{
  // check that a file was selected and that the file is good

	if (argc != 2)
	{
	    printf("No filename provided");
	    system("pause");
	    exit(1);
	}
	currentFile = argv[1];
	readFile(currentFile);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Distributions");
	createMenu();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialInput);
	glutMainLoop();

	system("pause");
	delete data; // clear memory
	return 0;
}
