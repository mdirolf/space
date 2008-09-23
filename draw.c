/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 11/13/2005
 * description: draw.c is the implementation of an abstract object forming the
 * interface to SDL drawing functions.
 ___________________________________________________________________________*/

#include "draw.h"
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define XRES 800
#define YRES 600
#define DEPTH 32

/* State variables of the draw interface. */
static SDL_Surface* oScreen;
static Color_T iClearColor;
static double dScaleX;
static double dScaleY;
static double dShiftX;
static double dShiftY;

/* Initialize the drawing module.  It is a checked runtime error for the
   drawing module to be previously initialized. */
void Draw_init() {

   /* Initialize SDL. */
   assert(oScreen == NULL);
   if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      exit(1);
   }
   atexit(SDL_Quit);

   /* Set the screen. */
   oScreen = SDL_SetVideoMode(XRES, YRES, DEPTH, SDL_HWSURFACE|SDL_DOUBLEBUF);
   if (oScreen == NULL) {
      fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
      exit(1);
   }

   /* Set the initial scaling and shifting factors. */
   dScaleX = 1.0;
   dScaleY = 1.0;
   dShiftX = 0.0;
   dShiftY = 0.0;

   /* Set the background color to black. */
   iClearColor = Draw_getColor(0, 0, 0);
}

/* Draw a pixel at pixel (x, y) onto the screen, with color R, G, B.
   It is a checked runtime error for for the drawing module to be
   uninitialized. */
void Draw_drawPixel(int x, int y, Color_T color) {
   Uint32 *bufp;

   assert(oScreen != NULL);

   /* Make sure the position is sane. */
   if (x < 0 || x >= XRES || y < 0 || y >= YRES)
      return;

   bufp = (Uint32 *)oScreen->pixels + y*oScreen->pitch/4 + x;
   *bufp = color;
}

/* In house function for getting a pixel at position (x, y).  It is a checked
   runtime error for the drawing module to be uninitialized. */
Uint8* Draw_getPixel(int x, int y) {
   assert(oScreen != NULL);

	if (x < 0 || x >= XRES || y < 0 || y >= YRES)
      return 0;
	return (Uint8*)oScreen->pixels + y * oScreen->pitch + x * 4;
}

/* Blends pixel x, y onto oScreen according to alpha.  It is a checked runtime
   error for oScreen to be NULL. */
void Draw_blendPixel(int x, int y, Color_T color, Uint8 alpha) {
	Uint8 *p;
	Uint32 R, G, B;

	assert(oScreen != NULL);

	if ((p = Draw_getPixel(x, y))) {
			R = ((*(Uint32*)p & oScreen->format->Rmask) + (((color & oScreen->format->Rmask) - (*(Uint32*)p & oScreen->format->Rmask)) * alpha >> 8) ) & oScreen->format->Rmask;
			G = ((*(Uint32*)p & oScreen->format->Gmask) + (((color & oScreen->format->Gmask) - (*(Uint32*)p & oScreen->format->Gmask)) * alpha >> 8) ) & oScreen->format->Gmask;
			B = ((*(Uint32*)p & oScreen->format->Bmask) + (((color & oScreen->format->Bmask) - (*(Uint32*)p & oScreen->format->Bmask)) * alpha >> 8) ) & oScreen->format->Bmask;
			*(Uint32*)p = R | G | B;
	}
}

/* Draw a pixel at position (dX, dY) onto oScreen, with color R, G, B.  The
   position will be scaled by the screen's scaling factor and centered.  It
   is a checked runtime error for the drawing module to be uninitialized. */
void Draw_drawScaledPixel(double dX, double dY, Color_T color) {
   int x, y;
   assert(oScreen != NULL);
   x = (int)((dX - dShiftX) * dScaleX + XRES / 2);
   y = (int)((-dY + dShiftY) * dScaleY + YRES / 2);
   Draw_drawPixel(x, y, color);
}

/* Draw a line from (x1, y1) to (x2, y2) onto the screen,
   with color color and alpha alpha.  It is a checked runtime error for for
   the drawing module to be uninitialized. */
void Draw_drawLineAlpha(int x1, int y1, int x2, int y2, Color_T color, Uint8 alpha) {
	int xaa, yaa, *a, *b, *a2, *b2, da, xd, yd;
	double aa, db;
	double realb;

   if ((x1 > XRES && x2 > XRES) ||
       (x1 < 0    && x2 < 0)    ||
       (y1 > YRES && y2 > YRES) ||
       (y1 < 0    && y2 < 0))
      return;

	/* xaa:   the x aa offset value, either 1 or 0
	 * yaa:   the y aa offset value, either 1 or 0
	 * *a:    reference to either x or y, depending on which way we are
	 *        drawing.
	 * *b:    reference to either x or y, depending on which way we are
	 *        drawing.
	 * *a2:   reference to either x2 or y2, depending on which way we are
	 *        drawing.
	 * *b2:   reference to either x2 or y2, depending on which way we are
	 *        drawing.
	 * xd:    distance from x to x2
	 * yd:    distance from y to y2
	 * da:    change in a for each iteration, either 1 or -1
	 * db:    change in b with respect to a
	 * realb: real b value, as b is an int, and db is going to be a
	 *        fraction
	 * aa:    anti-aliasing value, fraction part of realb
	 */

	/* find x and y distances */
	xd = x2 - x1;
	yd = y2 - y1;

	if (abs(xd) >= abs(yd)) {
	/* draw left/right to left/right */
		xaa = 0;
		yaa = 1;
		a = &x1;
		b = &y1;
		a2 = &x2;
		b2 = &y2;
		db = (double)yd / xd;
	} else {
	/* draw top/bottom to top/bottom */
		xaa = 1;
		yaa = 0;
		a = &y1;
		b = &x1;
		a2 = &y2;
		b2 = &x2;
		db = (double)xd / yd;
	}

	/* left or right? */
	da = (*a <= *a2) ? 1 : -1;
	/* up or down? */
	db *= da;
	/* init realb */
	realb = (double)*b;
	/* we're stopping when we hit *a2, so we have to add +/-1 to it */
	*a2 += da;

	for (; *a != *a2; *a += da) {
		*b = (int)(realb + 0.5);
		aa = (realb + 0.5 - *b) * alpha;
		Draw_blendPixel(x1 + xaa, y1 + yaa, color, (int)aa);
		Draw_blendPixel(x1 - xaa, y1 - yaa, color, alpha - (int)aa);

   	if (alpha < 255)
			Draw_blendPixel(x1, y1, color, alpha);
		else
			Draw_drawPixel(x1, y1, color);

		realb += db;
	}
}

/* Draw a line from (dX1, dY1) to (dX2, dY2) onto the screen,
   with color R, G, B.  The positions will be scaled by oScreen's scaling
   factor and centered.  It is a checked runtime error for for the drawing
   module to be uninitialized. */
void Draw_drawScaledLine(double dX1, double dY1, double dX2, double dY2,
                   Color_T color) {
   double x1, x2, y1, y2;
   assert(oScreen != NULL);

   x1 = (dX1 - dShiftX) * dScaleX + XRES / 2;
   y1 = (-dY1 + dShiftY) * dScaleY + YRES / 2;
   x2 = (dX2 - dShiftX) * dScaleX + XRES / 2;
   y2 = (-dY2 + dShiftY) * dScaleY + YRES / 2;
   Draw_drawLineAlpha(x1, y1, x2, y2, color, 255);
}

/* If necessary, lock the screen for drawing.  It is a checked runtime error
   for the drawing module to be uninitialized. */
void Draw_lockScreen() {
   assert(oScreen != NULL);

   if (SDL_MUSTLOCK(oScreen))
      if (SDL_LockSurface(oScreen) < 0)
         return;
}

/* If necessary, unlock the screen after drawing.  It is a checked runtime
   error for the drawing module to be uninitialized. */
void Draw_unlockScreen() {
   assert(oScreen != NULL);
   if (SDL_MUSTLOCK(oScreen))
      SDL_UnlockSurface(oScreen);
}

/* Flip the drawing buffer and the screen buffer.  It is a checked runtime
   error for the drawing module to be uninitialized. */
void Draw_flipScreen() {
   assert(oScreen != NULL);
   SDL_Flip(oScreen);
}

/* Fill the screen with iClearColor.  It is a checked runtime
   error for the drawing module to be uninitialized. */
void Draw_clearScreen() {
   assert(oScreen != NULL);
   SDL_FillRect(oScreen, NULL, iClearColor);
}

/* Return the coorasponding Color_T to the given R, G, and B values.  It is a
   checked runtime error for the drawing module to be uninitialized. */
Color_T Draw_getColor(char R, char G, char B) {
   assert(oScreen != NULL);
   return SDL_MapRGB(oScreen->format, (Uint8) R, (Uint8) G, (Uint8) B);
}

/* Set the scale to draw at to be dScale, dScale. */
void Draw_setScale(double dScale) {
   dScaleX = dScaleY = dScale;
}

/* Scale the drawing scale by double dScale. */
void Draw_scale(double dScale) {
   dScaleX *= dScale;
   dScaleY *= dScale;
}

/* Scale the drawing window to fit (-dX, +dX) , (-dY, +dY). */
void Draw_scaleToPoint(double dX, double dY) {
   dX = abs(dX);
   dY = abs(dY);
   dScaleX = XRES / (2 * dX);
   dScaleY = YRES / (2 * dY);
}

/* Shift the drawing window to be centered around (dX, dY). */
void Draw_shiftToPoint(double dX, double dY) {
   dShiftX = dX;
   dShiftY = dY;
}
