/*
 * Copyright (c) 2005-2008 Michael Dirolf (mike at dirolf dot com)
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/01/2005
 * description: rect.h defines the interface for the Rect ADT
 ___________________________________________________________________________*/

#ifndef RECT_H
#define RECT_H

#include "vector.h"
#include "draw.h"

typedef struct Rect* Rect_T;

/* Returns a new Rect_T centered at the origin with x-extent X and y-extent Y.
   The new Rect is axis-aligned to start, and (X, Y) represents the upper
   right corner of this rectangle. */
Rect_T Rect_new(double X, double Y, double mass);

/* Frees all memory occupied by oRect.  Does nothing if oRect is NULL. */
void Rect_free(Rect_T oRect);

/* Draws oRect onto the screen in color iColor.  It is a checked runtime
   error for oRect to be NULL. */
void Rect_draw(Rect_T oRect, Color_T iColor);

/* Rotates oRect's object by angle dTheta.  It is a checked runtime error for
   oRect to be NULL. */
void Rect_rotateObj(Rect_T oRect, double dTheta);

/* Translates oRect's object by (dX, dY).  It is a checked runtime error for
   oRectto be NULL. */
void Rect_translateObj(Rect_T oRect, double dX, double dY);

/* Rotates oRect alone by angle dTheta.  It is a checked runtime error for
   oRect to be NULL. */
void Rect_rotateRect(Rect_T oRect, double dTheta);

/* Translates oRect alone by (dX, dY).  It is a checked runtime error for
   oRectto be NULL. */
void Rect_translateRect(Rect_T oRect, double dX, double dY);

/* Returns 1 if oRect1 and oRect2 intersect, and zero otherwise.  It is a
   checked runtime error for oRect1 or orRect2 to be NULL. */
int Rect_doIntersect(Rect_T oRect1, Rect_T oRect2);

/* Returns the mass of oRect.  It is a checked runtime error for oRect to
   be NULL. */
double Rect_getMass(Rect_T oRect);

/* Returns the xextent of oRect.  It is a checked runtime error for oRect to
   be NULL. */
double Rect_getXExt(Rect_T oRect);

/* Returns the yextent of oRect.  It is a checked runtime error for oRect to
   be NULL. */
double Rect_getYExt(Rect_T oRect);

/* Returns the offset vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getOffset(Rect_T oRect);

/* Returns the yaxis vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getYAxis(Rect_T oRect);

/* Returns the xaxis vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getXAxis(Rect_T oRect);

/* Returns the center vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getCenter(Rect_T oRect);

/* Returns the moment of oRect about its axis.  It is checked runtime error
   for oRect to be NULL. */
double Rect_getMoment(Rect_T oRect);

#endif
