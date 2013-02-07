// assign1.cpp : Defines the entry point for the console application.
//

/*
  CSCI 480 Computer Graphics
  Assignment 1: Height Fields
  C++ starter code
*/

#include "stdafx.h"
#include <pic.h>
#include <windows.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

// Include other libraries
#include <iostream>

int g_iMenuId;

int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;

/* state of the world */
float g_vLandRotate[3] = {0.0, 0.0, 0.0};
float g_vLandTranslate[3] = {0.0, 0.0, 0.0};
float g_vLandScale[3] = {1.0, 1.0, 1.0};

/***** Values for scaling mouse enabled rotation/translation/scaling to make mouse movement easier*****/
float translateMultDPI = 0.5;
float rotationMultDPI = 1;
float scaleMultDPI = 1;
/***** *****/

/***** Multiplier used to change the starting size of the heightfield *****/
float HF_SCALE = 2;

/***** Constants for pixel depth *****/
float PIX_DEPTH = 255;
float HEIGHT_DECREMENT = 0.25;
float GRID_OFFSET = 0.000001; // For the triangle grid, the lines will draw slightly ABOVE the triangles so they are visible

/***** Values that determine the state of what to diaplay *****/
enum drawType {VERTEX, GRID, TRIANGLE, TRIANGLE_GRID}; // Will determine HOW to draw the heightfield
enum drawType drawState; // Instantiate the enum

enum drawColor {RED, GREEN, BLUE, ALL}; // Will determine WHAT COLOR to DRAW and MAKE HEIGHT VALUES from
enum drawColor drawColorType; // Instantiate the enum

/* see <your pic directory>/pic.h for type Pic */
Pic* g_pHeightData;
int g_pSizeData; // This value will hold the image size of the related Pic Struct Pointer above

/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
  int i;//, j;
  Pic *in = NULL;

  if (filename == NULL)
    return;

  /* Allocate a picture buffer */
  in = pic_alloc(640, 480, 3, NULL);

  printf("File to save to: %s\n", filename);

  for (i=479; i>=0; i--) {
    glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
                 &in->pix[i*in->nx*in->bpp]);
  }

  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);
}

void positionCamera() { // This method will set the camera to point and be in the appropriate places
	gluLookAt(
		0.0, 0.0, -3.0, // Where camera should point
		0.0, 0.0, 0.0,  // Camera Placement
		0.0, 1.0, 0.0   // The "up" vector, which in my case, is the Unit Y Vector
	);
}

void myinit()
{
	/* setup gl view here */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* Set up the shading type*/
	glShadeModel(GL_SMOOTH);

	// Enable the Depth Test and Mask so that z-buffering is enabled correctly
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	// Set my state enums
	drawState = VERTEX;
	drawColorType = ALL;
}

void createVertex(int x, int y) { // This function takes in an x,y coordinate and creates a vertex -- can be used in all drawing functions -- all that has to be changed is the ordering of the loop
		float redPixValue = (float)PIC_PIXEL(g_pHeightData, x, y, 0); // Will get the red pixel value from the image
		float bluPixValue = (float)PIC_PIXEL(g_pHeightData, x, y, 1); // Will get the red pixel value from the image
		float grnPixValue = (float)PIC_PIXEL(g_pHeightData, x, y, 2); // Will get the red pixel value from the image

		// Set up position for the height field
		float lengthOfImageX = g_pHeightData->nx;
		float lengthOfImageZ = g_pHeightData->ny;

		// Find the x position of the point in the heightfield
		float xPosition = (((float)x/lengthOfImageX) * HF_SCALE) - (HF_SCALE/2);
		/* How this works:
		1.  Normalize the position of the pixel in one axis relative to the size of the image IN THAT AXIS
		2.  Multiply this value by a scaling constant that increases the size of this normalized value
		3.  Subtract this value by half the scaling constant to align this pixel to a position relative to the center of the screen

		-- This algorithm will be used to find the posiiton of the z-value too
		*/

		// Find the z (NOT the y) position  "                "
		float zPosition = (((float)y/lengthOfImageZ) * HF_SCALE) - (HF_SCALE/2);

		// Find the height (THIS is y)      "                "
			
		// Find the value first in terms of pixel depth
		float heightValue = 0; // This value will detemine the pixel's height values in terms of the depth field
			
		if (g_pHeightData->bpp == 1) { // The image is ALL, therefore, average the values for the height
			heightValue = (redPixValue + bluPixValue + grnPixValue)/3;
		}
		if (g_pHeightData->bpp == 3) { // The image is colored, therefore, get the height value for the channel that is selected -- COMING SOON!
				if (drawColorType == RED) { 
					heightValue = redPixValue;
				}
				else if (drawColorType == GREEN) { 
					heightValue = grnPixValue;
				}
				else if (drawColorType == BLUE) { 
					heightValue = bluPixValue;
				}
				else if (drawColorType == ALL) { 
					heightValue = (redPixValue + bluPixValue + grnPixValue)/3;;
				}
		}

		// Now, begin actually making the point

		// Step 1:  Set up color relative to the height and the color channel selected

		// Normalize this heightValue
		heightValue = heightValue/PIX_DEPTH;

		if (drawColorType == ALL) { 
			glColor3f(1.0, heightValue, 1.0); 
		}
		else if (drawColorType == RED) { 
			glColor3f(1.0, heightValue, heightValue); 
		}
		else if (drawColorType == GREEN) { 
			glColor3f(heightValue, 1.0, heightValue); 
		}
		else if (drawColorType == BLUE) { 
			glColor3f(heightValue, heightValue, 1.0); 
		}

		// Step 2: Set up the point -- Scale the y
		glVertex3f(xPosition, heightValue*HEIGHT_DECREMENT, zPosition);
	return;
}

void drawPoints() { // Will draw the points of a heightField
	for (int y = 0; y < g_pHeightData->ny; y++) {
		for (int x = 0; x < g_pHeightData->nx; x++) {
			createVertex(x, y); // Will create the vertex -- saves on code rewriting
		}
	}
}

void drawLines(float offset) {

}

void drawTriangles() {

}

void display()
{
  /* draw 1x1 cube about origin */
  /* replace this code with your height field implementation */
  /* you may also want to precede it with your 
rotation/translation/scaling */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Obviously, clear these values, or KABOOM!!!

	glMatrixMode(GL_MODELVIEW); // Set mode to ModelView Here, just to be sure...

	glLoadIdentity(); // Reset the Matrix

	glPushMatrix(); // Push on the new transformations that are about to be done
	
	glTranslatef(
		(translateMultDPI * -g_vLandTranslate[0]), // Inverting this value (multiplying by -1) made it so that the shape followed the mouse for a-axis translations
		(translateMultDPI * g_vLandTranslate[1]),
		(translateMultDPI * -g_vLandTranslate[2])
	); // Translate the matrix

	glRotatef(-g_vLandRotate[0], 1, 0, 0); // Rotate along the x-axis - This value was inverted (multiplied by -1) because it made more sense to me to invert the X-axis rotation; it's what I am used to. (Autodesk Maya usage)
	glRotatef(-g_vLandRotate[1], 0, 1, 0); // Rotate along the y-axis - This value was inverted (multiplied by -1) because it made more sense to me to invert the Y-axis rotation; it's what I am used to. (Autodesk Maya usage)
	glRotatef(g_vLandRotate[2], 0, 0, 1); // Rotate along the z-axis

	glScalef(
		(scaleMultDPI * g_vLandScale[0]),
		(scaleMultDPI * g_vLandScale[1]),
		(scaleMultDPI * g_vLandScale[2])
	); // Scale the Matrix

	// Begin drawing the heightField, well, my polygon to start
	if (drawState == VERTEX) { // Then draw points
		glBegin(GL_POINTS);
			drawPoints();
	}
	else if (drawState == GRID) { // Then draw lines
		glBegin(GL_LINES);
			drawLines(0);
	}
	else if (drawState == TRIANGLE) { // Then draw triangles
		glBegin(GL_TRIANGLE_STRIP);
			drawTriangles();
	}
	else if (drawState == TRIANGLE_GRID) { // Then draw the trangle grid
		glBegin(GL_POINTS);
			drawTriangles();
		glEnd();
			glBegin(GL_LINES);
			drawLines(GRID_OFFSET);
	}

	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h) { // This function will project the contents of the program correctly
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); // Set the clipping area of the window to be correct

	// Set the matrix mode to projection, so that this will actually work
	glMatrixMode(GL_PROJECTION);

	// Now begin the actual reshaping
	glLoadIdentity(); // Reset the matrix

	// Set up the perspective projection matrix
	gluPerspective(60.0, (double)w/h, 0.01, 1000.0);
	
	positionCamera(); // Sets the camera position

	/*
		Viewing Angle: 45
		Aspect Ratio: 1.333 (4:3)
		Near Clipping Plane: 0.5
		Far  "            ": 1.0
	*/
	// Set the matrix mode back to modelView, so things do not get messed up
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Reset the matrix
}

void menufunc(int value)
{
  switch (value)
  {
    case 0:
      exit(0);
      break;
  }
}

void doIdle()
{
  /* do some stuff... */

  /* make the screen update */
  glutPostRedisplay();
}

/* converts mouse drags into information about 
rotation/translation/scaling */
void mousedrag(int x, int y)
{
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
  
  switch (g_ControlState)
  {
    case TRANSLATE:  
      if (g_iLeftMouseButton)
      {
        g_vLandTranslate[0] += vMouseDelta[0]*0.01;
        g_vLandTranslate[1] -= vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandTranslate[2] += vMouseDelta[1]*0.01;
      }
      break;
    case ROTATE:
      if (g_iLeftMouseButton)
      {
        g_vLandRotate[0] += vMouseDelta[1];
        g_vLandRotate[1] += vMouseDelta[0];
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandRotate[2] += vMouseDelta[1];
      }
      break;
    case SCALE:
      if (g_iLeftMouseButton)
      {
        g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
        g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
      }
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
 
  switch(glutGetModifiers())
  {
    case GLUT_ACTIVE_CTRL:
      g_ControlState = TRANSLATE;
      break;
    case GLUT_ACTIVE_SHIFT:
      g_ControlState = SCALE;
      break;
    default:
      g_ControlState = ROTATE;
      break;
  }

  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

// New keyboard function -- switches between drawing states for triagles, lines, points, and colors
void keyPressed(unsigned char key, int x, int y) {	
	
	if (key == 'q') { // then set drawing to points
		drawState = VERTEX;
	}
	if (key == 'w') { // then set drawing to lines
		drawState = GRID;
	}
	if (key == 'e') { // then set drawing to triangles
		drawState = TRIANGLE;
	}
	if (key == 'r') { // then set drawing to triangle mesh
		drawState = TRIANGLE_GRID;
	}

	if (key == 'a') { // then set drawing color to ALL
			drawColorType = ALL;
	}
	if (key == 's') { // then set drawing color to red
			drawColorType = RED;
	}
	if (key == 'd') { // then set drawing color to green
			drawColorType = GREEN;
	}
	if (key == 'f') { // then set drawing color to blue
			drawColorType = BLUE;
	}

}

int main(int argc, char* argv[])
{
	// I've set the argv[1] to spiral.jpg.
	// To change it, on the "Solution Explorer",
	// right click "assign1", choose "Properties",
	// go to "Configuration Properties", click "Debugging",
	// then type your texture name for the "Command Arguments"
	
	if (argc<2)
	{  
		printf ("usage: %s heightfield.jpg\n", argv[0]);
		exit(1);
	}

	g_pHeightData = jpeg_read((char*)argv[1], NULL); // This will return a Pic struct with information about the image relating to the heightField

	if (!g_pHeightData)
	{
	    printf ("error reading %s.\n", argv[1]);
	    exit(1);
	}
	

	for (int i = 0; i < argc; i++)
		std::cout << "argv[" << i << "] = " << argv[i] << std::endl;


	std::cout << "Image x Size: " << g_pHeightData->nx << " Image Y Size: " << g_pHeightData->ny << " BPP:" << g_pHeightData->bpp << std::endl;
	std::cout << "Testing run through values." << std::endl;
	for (int y = 0; y >= 0; y--) {
		for (int x = 0; x >= 0; x--) {
			std::cout << "Height Value at Pixel Red: " << x << ", " << y << " = " << (float)PIC_PIXEL(g_pHeightData, x, y, 0) << std::endl;
			std::cout << "Height Value at Pixel Green: " << x << ", " << y << " = " << (float)PIC_PIXEL(g_pHeightData, x, y, 1) << std::endl;
			std::cout << "Height Value at Pixel Blue: " << x << ", " << y << " = " << (float)PIC_PIXEL(g_pHeightData, x, y, 2) << std::endl;
		}
	}

	std::cout << (int)g_pHeightData->pix[256] << std::endl;

	glutInit(&argc,argv);
  
	/*
		create a window here..should be double buffered and use depth testing
  
	    the code past here will segfault if you don't have a window set up....
	    replace the exit once you add those calls.
	*/

	//exit(0);

	// Set up the window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(200,200);
	glutCreateWindow(argv[0]);

	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);

	// Add in the reshape function, so that the camera is aligned to the proper aspect ratio, and that the window draws correctly
	glutReshapeFunc(reshape);
  
	/* allow the user to quit using the right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
  
	/* replace with any animate code */
	glutIdleFunc(doIdle);

	/* callback for mouse drags */
	glutMotionFunc(mousedrag);
	/* callback for idle mouse movement */
	glutPassiveMotionFunc(mouseidle);
	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);

	// Callback for keyboard function
	glutKeyboardFunc(keyPressed);

	/* do initialization */
	myinit();

	/* Temporary Testing To See how image stuff works. */

	

	glutMainLoop();
	return 0;
}