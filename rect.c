/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/01/2005
 * description: rect.c defines the implementation for the Rect ADT
 ___________________________________________________________________________*/

#include "rect.h"
#include "vector.h"
#include "draw.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* Structure to store the Rect datatype. */
struct Rect {
       double extentX;
       double extentY;
       double mass;
       Vector_T objCenter; /* Center of the object the rect belongs to. */
       Vector_T offset;    /* Offset of the rect from that center point. */
       Vector_T axisX;     /* X-axis of the rect. */
       Vector_T axisY;     /* Y-axis of the rect. */
       };

/* Returns a new Rect_T centered at the origin with x-extent X and y-extent Y.
   The new Rect is axis-aligned to start, and (X, Y) represents the upper
   right corner of this rectangle. */
Rect_T Rect_new(double X, double Y, double Mass) {
   Rect_T oNewRect = (Rect_T)malloc(sizeof(struct Rect));
   assert(oNewRect != NULL);
   oNewRect->extentX    = X;
   oNewRect->extentY    = Y;
   oNewRect->mass       = Mass;
   oNewRect->objCenter  = Vector_new(0, 0);
   oNewRect->offset     = Vector_new(0, 0);
   oNewRect->axisX      = Vector_new(1, 0);
   oNewRect->axisY      = Vector_new(0, 1);
   return oNewRect;
}

/* Frees all memory occupied by oRect.  Does nothing if oRect is NULL. */
void Rect_free(Rect_T oRect) {
   if (oRect == NULL)
      return;
   Vector_free(oRect->objCenter);
   Vector_free(oRect->offset);
   Vector_free(oRect->axisX);
   Vector_free(oRect->axisY);
   free(oRect);
}

/* Draws oRect onto the screen in color iColor.  It is a checked runtime
   error for oRect to be NULL. */
void Rect_draw(Rect_T oRect, Color_T iColor) {
   Vector_T upRight, upLeft, downLeft, downRight;
   Vector_T upperHyp, lowerHyp;
   Vector_T center;
   
   assert(oRect != NULL);
   
   Vector_scale(oRect->axisX, oRect->extentX);
   Vector_scale(oRect->axisY, oRect->extentY);
   
   center = Vector_sum(oRect->objCenter, oRect->offset);
   upperHyp = Vector_sum(oRect->axisX, oRect->axisY);
   lowerHyp = Vector_diff(oRect->axisX, oRect->axisY);
   
   Vector_normalize(oRect->axisX);
   Vector_normalize(oRect->axisY);
   
   upRight = Vector_sum(center, upperHyp);
   upLeft = Vector_diff(center, lowerHyp);
   downLeft = Vector_diff(center, upperHyp);
   downRight = Vector_sum(center, lowerHyp);
   
   Vector_free(center);
   Vector_free(upperHyp);
   Vector_free(lowerHyp);
   
   Vector_drawBetween(upRight, upLeft, iColor);
   Vector_drawBetween(upLeft, downLeft, iColor);
   Vector_drawBetween(downLeft, downRight, iColor);
   Vector_drawBetween(downRight, upRight, iColor);
/*
   This would draw the individaul corners, but it just doesn't
   look right at this moment

   Vector_draw(upRight, iColor);
   Vector_draw(upLeft, iColor);
   Vector_draw(downLeft, iColor);
   Vector_draw(downRight, iColor);
*/
   Vector_free(upRight);
   Vector_free(upLeft);
   Vector_free(downLeft);
   Vector_free(downRight);
}

/* Rotates oRect's object by angle dTheta.  It is a checked runtime error for
   oRect to be NULL. */
void Rect_rotateObj(Rect_T oRect, double dTheta) {
   assert(oRect != NULL);
   Vector_rotate(oRect->offset, dTheta);
   Vector_rotate(oRect->axisX, dTheta);
   Vector_rotate(oRect->axisY, dTheta);
}

/* Translates oRect's object by (dX, dY).  It is a checked runtime error for
   oRectto be NULL. */
void Rect_translateObj(Rect_T oRect, double dX, double dY) {
   assert(oRect != NULL);
   Vector_setX(oRect->objCenter, Vector_getX(oRect->objCenter) + dX);
   Vector_setY(oRect->objCenter, Vector_getY(oRect->objCenter) + dY);
}

/* Rotates oRect alone by angle dTheta.  It is a checked runtime error for
   oRect to be NULL. */
void Rect_rotateRect(Rect_T oRect, double dTheta) {
   assert(oRect != NULL);
   Vector_rotate(oRect->axisX, dTheta);
   Vector_rotate(oRect->axisY, dTheta);
}

/* Translates oRect alone by (dX, dY).  It is a checked runtime error for
   oRectto be NULL. */
void Rect_translateRect(Rect_T oRect, double dX, double dY) {
   assert(oRect != NULL);
   Vector_setX(oRect->offset, Vector_getX(oRect->offset) + dX);
   Vector_setY(oRect->offset, Vector_getY(oRect->offset) + dY);
}

/* Returns 1 if oRect1 and oRect2 intersect, and zero otherwise.  It is a
   checked runtime error for oRect1 or orRect2 to be NULL. */
int Rect_doIntersect(Rect_T oRect1, Rect_T oRect2) {
   Vector_T T, L;
   double R1, R2, scale;
   
   assert((oRect1 != NULL) && (oRect2 != NULL));
   
   /* T is the distance between centers. */
   T = Vector_diff(oRect2->objCenter, oRect1->objCenter);
   Vector_plus(T, oRect2->offset);
   Vector_minus(T, oRect1->offset);
   
/* We must scale L by the length of T in order to make sure that the dot
   product of T and L will be determined by T's length, not L's.  Here L
   is the separting axis we are currently testing for. */
   scale = Vector_length(T);
   L = Vector_copy(oRect1->axisX);
   Vector_scale(L, scale);
   
   R1 = oRect1->extentX * abs(Vector_dot(oRect1->axisX, L)) +
        oRect1->extentY * abs(Vector_dot(oRect1->axisY, L));
   R2 = oRect2->extentX * abs(Vector_dot(oRect2->axisX, L)) +
        oRect2->extentY * abs(Vector_dot(oRect2->axisY, L));
   if (abs(Vector_dot(T, L)) > R1 + R2) {
      Vector_free(L);
      Vector_free(T);
      return 0;
   }
   Vector_free(L);
   
   L = Vector_copy(oRect1->axisY);
   Vector_scale(L, scale);
   R1 = oRect1->extentX * abs(Vector_dot(oRect1->axisX, L)) +
        oRect1->extentY * abs(Vector_dot(oRect1->axisY, L));
   R2 = oRect2->extentX * abs(Vector_dot(oRect2->axisX, L)) +
        oRect2->extentY * abs(Vector_dot(oRect2->axisY, L));
   if (abs(Vector_dot(T, L)) > R1 + R2) {
      Vector_free(L);
      Vector_free(T);
      return 0;
   }
   Vector_free(L);
   
   L = Vector_copy(oRect2->axisX);
   Vector_scale(L, scale);
   R1 = oRect1->extentX * abs(Vector_dot(oRect1->axisX, L)) +
        oRect1->extentY * abs(Vector_dot(oRect1->axisY, L));
   R2 = oRect2->extentX * abs(Vector_dot(oRect2->axisX, L)) +
        oRect2->extentY * abs(Vector_dot(oRect2->axisY, L));
   if (abs(Vector_dot(T, L)) > R1 + R2) {
      Vector_free(L);
      Vector_free(T);
      return 0;
   }
   Vector_free(L);
   
   L = Vector_copy(oRect2->axisY);
   Vector_scale(L, scale);
   R1 = oRect1->extentX * abs(Vector_dot(oRect1->axisX, L)) +
        oRect1->extentY * abs(Vector_dot(oRect1->axisY, L));
   R2 = oRect2->extentX * abs(Vector_dot(oRect2->axisX, L)) +
        oRect2->extentY * abs(Vector_dot(oRect2->axisY, L));
   if (abs(Vector_dot(T, L)) > R1 + R2) {
      Vector_free(L);
      Vector_free(T);
      return 0;
   }
   Vector_free(L);

   Vector_free(T);
   return 1;
}

/* Returns the mass of oRect.  It is a checked runtime error for oRect to
   be NULL. */
double Rect_getMass(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->mass;
}

/* Returns the xextent of oRect.  It is a checked runtime error for oRect to
   be NULL. */
double Rect_getXExt(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->extentX;
}

/* Returns the yextent of oRect.  It is a checked runtime error for oRect to
   be NULL. */
double Rect_getYExt(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->extentY;
}

/* Returns the offset vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getOffset(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->offset;
}

/* Returns the yaxis vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getYAxis(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->axisY;
}

/* Returns the xaxis vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getXAxis(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->axisX;
}

/* Returns the center vector for oRect.  It is a checked runtime
   error for oRect to be NULL. */
Vector_T Rect_getCenter(Rect_T oRect) {
   assert(oRect != NULL);
   return oRect->objCenter;
}

/* Returns the moment of oRect about its axis.  It is checked runtime error
   for oRect to be NULL. */
double Rect_getMoment(Rect_T oRect) {
   double I;
   double x, y;

   assert(oRect != NULL);
   
   x = oRect->extentX;
   y = oRect->extentY;
   I = oRect->mass * (x*x + y*y) / 3.0;
   
   I += oRect->mass * Vector_length(oRect->offset);
   
   return I;
}
   
      

