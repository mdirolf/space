/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 11/13/2005
 * description: draw.h is an abstract object forming the interface to SDL
 * drawing functions.
 ___________________________________________________________________________*/
 
#ifndef DRAW_H
#define DRAW_H

#include <SDL/SDL.h>

/* Define the Color_T type, making it easier to pass colors around between
   functions and data types. */
typedef Uint32 Color_T;

/* Initialize the drawing module.  It is a checked runtime error for the
   drawing module to be previously initialized. */
void Draw_init();

/* Draw a pixel at pixel (x, y) onto the screen, with color R, G, B.
   It is a checked runtime error for for the drawing module to be
   uninitialized. */
void Draw_drawPixel(int x, int y, Color_T col);

/* Draw a pixel at position (dX, dY) onto oScreen, with color R, G, B.  The
   position will be scaled by the screen's scaling factor and centered.  It
   is a checked runtime error for the drawing module to be uninitialized. */
void Draw_drawScaledPixel(double dX, double dY, Color_T color);

/* Draw a line from (x1, y1) to (x2, y2) onto the screen,
   with color color and alpha alpha.  It is a checked runtime error for for
   the drawing module to be uninitialized. */
void Draw_drawLineAlpha(int x1, int y1, int x2, int y2, Color_T color, Uint8 alpha);

/* Draw a line from (dX1, dY1) to (dX2, dY2) onto the screen,
   with color R, G, B.  The positions will be scaled by oScreen's scaling
   factor and centered.  It is a checked runtime error for for the drawing
   module to be uninitialized. */
void Draw_drawScaledLine(double dX1, double dY1, double dX2, double dY2,
                         Color_T color);

/* If necessary, lock the screen for drawing.  It is a checked runtime error
   for the drawing module to be uninitialized. */
void Draw_lockScreen();

/* If necessary, unlock the screen after drawing.  It is a checked runtime
   error for the drawing module to be uninitialized. */
void Draw_unlockScreen();

/* Flip the drawing buffer and the screen buffer.  It is a checked runtime
   error for the drawing module to be uninitialized. */
void Draw_flipScreen();

/* Fill the screen with iClearColor.  It is a checked runtime
   error for the drawing module to be uninitialized. */
void Draw_clearScreen();

/* Return the coorasponding Color_T to the given R, G, and B values.  It is a
   checked runtime error for the drawing module to be uninitialized. */
Color_T Draw_getColor(char R, char G, char B);

/* Set the scale to draw at to be dScale, dScale. */
void Draw_setScale(double dScale);

/* Scale the drawing scale by double dScale. */
void Draw_scale(double dScale);

/* Scale the drawing window to fit (-dX, +dX) , (-dY, +dY). */
void Draw_scaleToPoint(double dX, double dY);

/* Shift the drawing window to be centered around (dX, dY). */
void Draw_shiftToPoint(double dX, double dY);

#endif

