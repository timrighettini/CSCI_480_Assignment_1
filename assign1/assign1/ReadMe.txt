======================
==========
Tim Righttini's Assignment #1 Readme: CSCI_480 Computer Graphics (CG)
==========
======================

------
Hello:
------

This OpenGL program is a simulation that draws a heightfield based upon an image argument determined at the command line.

The program completes the following tasks, EXCLUDING extra credit:

1.  Renders a heightfield of a 256x256 image, with smooth image manupulations of images that size
2.  The window size is 640x480
3.  The window is rendered in perspective view, and uses all of the appropriate z buffer variables
4.  Mouse input allows the player to scale, rotate, and translate the image
5.  Heightfield is colored using a smooth gradient
6.  The heightfield can render as points, lines, triangles

Extra Credit Includes the following:
1.  The program can support BBP1 and BBP3 by taking in seprarte heightfields of each color channel and the average of the color channels.  Colors of the height field change depending on the pixel value measured
	- Purple Heightfield = Measuring Average Pixel Values for height
	- Red Heightfield = Measuring Red Pixel Values for height
	- Green Heightfield = Measuring Green Pixel Values for height
	- Blue Heightfield = Measuring Blue Pixel Values for height
	* This was tested with the smiley face image seen in the animation
2.  A fourth rendering mode is implemented:  Wireframe can be drawn on top of triangles
3.  Push/Pop matrices were used with transformations
4.  Front and Back Triangle faces can be culled by pressing certain keys
5.  Can accept images of any size, and even non square ones (test with bunny_black.jpg)
6.  Multiple types of wireframes can be drawn, based upon a key press
7.  The player can reset the position of the heightfield

---------
Controls:
---------

ROTATION:

Left Mouse Button + Drag Mouse Right = Rotate Clockwise (X Axis)
Left Mouse Button + Drag Mouse Left = Rotate Counter-Clockwise (X Axis)

Left Mouse Button + Drag Mouse Down = Rotate Clockwise (Y Axis)
Left Mouse Button + Drag Mouse Up = Rotate Counter-Clockwise (Y Axis)

Middle Mouse Button + Drag Mouse Down = Rotate Clockwise (Z Axis)
Middle Mouse Button + Drag Mouse Up = Rotate Counter-Clockwise (Z Axis)

TRANSLATION:

CONTROL + Left Mouse Button + Drag Mouse Right/Left/Up/Down = Translate Right/Left/Up/Down, respectively
CONTROL + Middle Mouse Button + Drag Mouse Up/Down = Translate Zoom out/in, respectively
*Note: The object translates in the direction the mouse moves in

SCALING:

SHIFT + Left Mouse Button + Drag Mouse Right/Left = Increase/Decrease Scale (X Axis)
SHIFT + Left Mouse Button + Drag Mouse Up/Down = Increase/Decrease Scale (Y Axis)
SHIFT + Middle Mouse Button + Drag Mouse Up/Down = Increase/Decrease Scale (Z Axis)

SWITCH BETWEEN VIEWING MODES:

Key 1 = Draw Points
Key 2 = Draw Lines
Key 3 = Draw Triangles
Key 4 = Draw Lines + Triangles
Key A = Draw Wireframe as RECTANGLES
Key S = Draw wireframe as RECTANGLES + ONE INTERSECTING LINE
Key D = Draw wireframe as RECTANGLES + TWO INTERSECTING LINEs

SWITCH BETWEEN COLOR MODES:

Key Q = Draw Average Pixel Value Heightfield
Key W = Draw RED Pixel Value Heightfield
Key E = Draw GREEN Pixel Value Heightfield
Key R = Draw BLUE Pixel Value Heightfield

SWITCH BETWEEN FACE CULLING MODES:

Key Z = Turn FACE CULLING ON/OFF
Key X = If culling is ON, CULL FRONT FACES
Key C = If culling is ON, CULL BACK FACES

RESET TRANSFORMATIONS:

Key ESCAPE = Reset Transformations of heightField

START/STOP SAVING SCREENSHOTS:

Key SPACE = Turn Screen Shot saving on/off

-------------
Special Notes
-------------

1.  I changed some of the controls around for scaling, translation, and rotation (i.e. I inverted some transformations based upon mouse dragging) to fit the style of Autodesk Maya.  Rotating and translating opposite to what I was used to in Maya for the X/Y Axes weirded me out, so I changed it around.

Besides for the notes and what I have mentioned above, this is everything you need to know about the program.

Thanks! 

-------
The End
-------

========================================================================
    CONSOLE APPLICATION : assign1 Project Overview
========================================================================

AppWizard has created this assign1 application for you.

This file contains a summary of what you will find in each of the files that
make up your assign1 application.


assign1.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

assign1.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

assign1.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named assign1.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
