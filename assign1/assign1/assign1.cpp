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

	// Enable the Depth test so that z-buffering is enabled
	glEnable(GL_DEPTH_TEST);
}

void display()
{
  /* draw 1x1 cube about origin */
  /* replace this code with your height field implementation */
  /* you may also want to precede it with your 
rotation/translation/scaling */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); // Set mode to ModelView Here, just to be sure...

	glLoadIdentity(); // Reset the Matrix

	glPushMatrix(); // Push on the new transformations that are about to be done
	
	glTranslatef(
		(translateMultDPI * -g_vLandTranslate[0]), // Inverting this value (multiplying by -1) made it so that the shape followed the mouse for a-axis translations
		(translateMultDPI * g_vLandTranslate[1]),
		(translateMultDPI * g_vLandTranslate[2])
	); // Translate the matrix

	glRotatef(g_vLandRotate[0], 1, 0, 0); // Rotate along the x-axis
	glRotatef(-g_vLandRotate[1], 0, 1, 0); // Rotate along the y-axis - This value was inverted (multiplied by -1) because it made more sense to me to invert the Y-axis rotation; it's what I am used to.
	glRotatef(g_vLandRotate[2], 0, 0, 1); // Rotate along the z-axis

	glScalef(
		(scaleMultDPI * g_vLandScale[0]),
		(scaleMultDPI * g_vLandScale[1]),
		(scaleMultDPI * g_vLandScale[2])
	); // Scale the Matrix

	// Begin drawing the heightField, well, my polygon to start

	glBegin(GL_POLYGON);
		///*
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-0.5, -0.5, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-0.5, 0.5, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.5, -0.5, 0.0);
		//*/
		// Top Face
		/*
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-0.5, 0.0, -0.5);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-0.5, 0.0, 0.5);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.5, 0.0, 0.5);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.5, 0.0, -0.5);
		*/

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
	gluPerspective(45.0, (double)w/h, 0.1, 100.0);
	
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

	/* do initialization */
	myinit();

	/* Temporary Testing To See how image stuff works. */

	

	glutMainLoop();
	return 0;
}