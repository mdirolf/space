/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 11/13/2005
 * description: vector.h defines the interface for the Vector ADT
 ___________________________________________________________________________*/

#ifndef VECTOR_H
#define VECTOR_H

#include "draw.h"

typedef struct Vector* Vector_T;

/* Returns a new Vector_T with x-value X and y-value Y. */
Vector_T Vector_new(double X, double Y);

/* Frees all memory occupied by oVector.  Does nothing if oVector is NULL. */
void Vector_free(Vector_T oVector);

/* Returns the x-value of oVector.  It is a checked runtime error for oVector
   to be NULL. */
double Vector_getX(Vector_T oVector);

/* Returns the y-value of oVector.  It is a checked runtime error for oVector
   to be NULL. */
double Vector_getY(Vector_T oVector);

/* Sets the x-value of oVector to be equal to X.  It is a checked runtime
   error for oVector to be NULL. */
void Vector_setX(Vector_T oVector, double X);

/* Sets the y-value of oVector to be equal to Y.  It is a checked runtime
   error for oVector to be NULL. */
void Vector_setY(Vector_T oVector, double Y);

/* Returns a deep copy of the vector oVector.  It is a checked runtime error
   for oVector to be NULL. */
Vector_T Vector_copy(Vector_T oVector);

/* Returns the resultant vector after the summation of oVec1 and oVec2.  It is
   a checked runtime error for either oVec1 or oVec2 to be NULL.
   WARNING: a line like v = Vector_sum(v, v2) will cause a memory leak, as
   a new Vector object is created to store the result of the addition. */
Vector_T Vector_sum(Vector_T oVec1, Vector_T oVec2);

/* Returns resultant vector after the subtraction of oVec2 from oVec1.  It is
   a checked runtime error for either oVec1 or oVec2 to be NULL.
   WARNING: a line like v = Vector_diff(v, v2) will cause a memory leak, as
   a new Vector object is created to store the result of the addition. */
Vector_T Vector_diff(Vector_T oVec1, Vector_T oVec2);

/* Adds oVecInc to oVecResult.  It is a checked runtime error for either
   oVecResult or oVecInc to be NULL. */
void Vector_plus(Vector_T oVecResult, Vector_T oVecInc);

/* Subtracts oVecDec from oVecResult.  It is a checked runtime error for
   either oVecResult or oVecDec to be NULL. */
void Vector_minus(Vector_T oVecResult, Vector_T oVecDec);

/* Returns the signed magnitude of the cross product taken between
   oVec1 and oVec2.  It is a checked runtime error for either oVec1 or oVec2
   to be NULL. */
double Vector_magCross(Vector_T oVec1, Vector_T oVec2);

/* Returns the resultant double after the dot product is taken between oVec1
   and oVec2.  It is a checked runtime error for either oVec1 or oVec2 to be
   NULL. */
double Vector_dot(Vector_T oVec1, Vector_T oVec2);

/* Returns the length of oVector.  It is a checked runtime error for oVector
   to be NULL. */
double Vector_length(Vector_T oVector);

/* Scales oVector by the scaling factor dScale.  It is a checked runtime error
   for oVector to be NULL. */
void Vector_scale(Vector_T oVector, double dScale);

/* Normalizes oVector.  It is a checked runtime error for oVector to be
   NULL. */
void Vector_normalize(Vector_T oVector);

/* Rotates oVector by angle dTheta.  It is a checked runtime error for oVector
   to be NULL. */
void Vector_rotate(Vector_T oVector, double dTheta);

/* Draws a pixel cooresponding to the position represented by oVector in the
   color iColor.  It is a checked runtime error for oVector to be null. */
void Vector_draw(Vector_T oVector, Color_T iColor);

/* Draws a line between the endpoints of oVec1 and oVec2 in the color iColor.
   It is a checked runtime error for oVec1 or oVec2 to be null. */
void Vector_drawBetween(Vector_T oVec1, Vector_T oVec2, Color_T iColor);

#endif

