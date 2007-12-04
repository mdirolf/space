/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 11/13/2005
 * description: vector.c defines the implementation for the Vector ADT
 ___________________________________________________________________________*/

#include "vector.h"
#include "draw.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* Structure to store the Vector datatype. */
struct Vector {
       double dX;
       double dY;
       };

/* Returns a new Vector_T with x-value X and y-value Y. */
Vector_T Vector_new(double X, double Y) {
   Vector_T oNewVec = malloc(sizeof(struct Vector));
   assert(oNewVec != NULL);
   oNewVec->dX = X;
   oNewVec->dY = Y;
   return oNewVec;
}

/* Frees all memory occupied by oVector.  Does nothing if oVector is NULL. */
void Vector_free(Vector_T oVector) {
   free(oVector);
}

/* Returns the x-value of oVector.  It is a checked runtime error for oVector
   to be NULL. */
double Vector_getX(Vector_T oVector) {
   assert(oVector != NULL);
   return(oVector->dX);
}

/* Returns the y-value of oVector.  It is a checked runtime error for oVector
   to be NULL. */
double Vector_getY(Vector_T oVector) {
   assert(oVector != NULL);
   return(oVector->dY);
}

/* Sets the x-value of oVector to be equal to X.  It is a checked runtime
   error for oVector to be NULL. */
void Vector_setX(Vector_T oVector, double X) {
   assert(oVector != NULL);
   oVector->dX = X;
}

/* Sets the y-value of oVector to be equal to Y.  It is a checked runtime
   error for oVector to be NULL. */
void Vector_setY(Vector_T oVector, double Y) {
   assert(oVector != NULL);
   oVector->dY = Y;
}

/* Returns a deep copy of the vector oVector.  It is a checked runtime error
   for oVector to be NULL. */
Vector_T Vector_copy(Vector_T oVector) {
   Vector_T oResult = malloc(sizeof(struct Vector));
   assert (oVector != NULL && oResult != NULL);
   oResult->dX = oVector->dX;
   oResult->dY = oVector->dY;
   return oResult;
}

/* Returns the resultant vector after the summation of oVec1 and oVec2.  It is
   a checked runtime error for either oVec1 or oVec2 to be NULL.
   WARNING: a line like v = Vector_sum(v, v2) will cause a memory leak, as
   a new Vector object is created to store the result of the addition. */
Vector_T Vector_sum(Vector_T oVec1, Vector_T oVec2) {
   Vector_T oResult = malloc(sizeof(struct Vector));
   assert(oVec1 != NULL && oVec2 != NULL && oResult != NULL);
   oResult->dX = oVec1->dX + oVec2->dX;
   oResult->dY = oVec1->dY + oVec2->dY;
   return oResult;
}

/* Returns resultant vector after the subtraction of oVec2 from oVec1.  It is
   a checked runtime error for either oVec1 or oVec2 to be NULL.
   WARNING: a line like v = Vector_diff(v, v2) will cause a memory leak, as
   a new Vector object is created to store the result of the addition. */
Vector_T Vector_diff(Vector_T oVec1, Vector_T oVec2) {
   Vector_T oResult = malloc(sizeof(struct Vector));
   assert(oVec1 != NULL && oVec2 != NULL && oResult != NULL);
   oResult->dX = oVec1->dX - oVec2->dX;
   oResult->dY = oVec1->dY - oVec2->dY;
   return oResult;
}

/* Adds oVecInc to oVecResult.  It is a checked runtime error for either
   oVecResult or oVecInc to be NULL. */
void Vector_plus(Vector_T oVecResult, Vector_T oVecInc) {
   assert(oVecResult != NULL && oVecInc != NULL);
   oVecResult->dX = oVecResult->dX + oVecInc->dX;
   oVecResult->dY = oVecResult->dY + oVecInc->dY;
}

/* Subtracts oVecDec from oVecResult.  It is a checked runtime error for
   either oVecResult or oVecDec to be NULL. */
void Vector_minus(Vector_T oVecResult, Vector_T oVecDec) {
   assert(oVecResult != NULL && oVecDec != NULL);
   oVecResult->dX = oVecResult->dX - oVecDec->dX;
   oVecResult->dY = oVecResult->dY - oVecDec->dY;
}

/* Returns the signed magnitude of the cross product taken between
   oVec1 and oVec2.  It is a checked runtime error for either oVec1 or oVec2
   to be NULL. */
double Vector_magCross(Vector_T oVec1, Vector_T oVec2) {
   assert(oVec1 != NULL && oVec2 != NULL);
   return oVec1->dX * oVec2->dY - oVec1->dY * oVec2->dX;
}

/* Returns the resultant double after the dot product is taken between oVec1
   and oVec2.  It is a checked runtime error for either oVec1 or oVec2 to be
   NULL. */
double Vector_dot(Vector_T oVec1, Vector_T oVec2) {
   assert(oVec1 != NULL && oVec2 != NULL);
   return(oVec1->dX * oVec2->dX + oVec1->dY * oVec2->dY);
}

/* Returns the length of oVector.  It is a checked runtime error for oVector
   to be NULL. */
double Vector_length(Vector_T oVector) {
   double squares;
   assert(oVector != NULL);
   squares = oVector->dX * oVector->dX + oVector->dY * oVector->dY;
   return sqrt(squares);
}

/* Scales oVector by the scaling factor dScale.  It is a checked runtime error
   for oVector to be NULL. */
void Vector_scale(Vector_T oVector, double dScale) {
   assert(oVector != NULL);
   oVector->dX *= dScale;
   oVector->dY *= dScale;
}

/* Normalizes oVector.  It is a checked runtime error for oVector to be
   NULL. */
void Vector_normalize(Vector_T oVector) {
   assert(oVector != NULL); 
   Vector_scale(oVector, 1 / Vector_length(oVector));
}

/* Rotates oVector by angle dTheta.  It is a checked runtime error for oVector
   to be NULL. */
void Vector_rotate(Vector_T oVector, double dTheta) {
   double dCos, dSin, xNew, yNew;
   
   assert(oVector != NULL);
   dCos = cos(dTheta);
   dSin = sin(dTheta);
   xNew = oVector->dX * dCos - oVector->dY * dSin;
   yNew = oVector->dY * dCos + oVector->dX * dSin;
   oVector->dX = xNew;
   oVector->dY = yNew;
}

/* Draws a pixel cooresponding to the position represented by oVector in the
   color iColor.  It is a checked runtime error for oVector to be null. */
void Vector_draw(Vector_T oVector, Color_T iColor) {
   assert(oVector != NULL);
   Draw_drawScaledPixel(oVector->dX, oVector->dY, iColor);
}

/* Draws a line between the endpoints of oVec1 and oVec2 in the color iColor.
   It is a checked runtime error for oVec1 or oVec2 to be null. */
void Vector_drawBetween(Vector_T oVec1, Vector_T oVec2, Color_T iColor) {
   assert((oVec1 != NULL) && (oVec2 != NULL));
   Draw_drawScaledLine(oVec1->dX, oVec1->dY, oVec2->dX, oVec2->dY, iColor);
}
