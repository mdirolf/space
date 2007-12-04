/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/02/2005
 * description: ship.h defines the interface for the Ship ADT.
 ___________________________________________________________________________*/

#ifndef SHIP_H
#define SHIP_H

#include "rect.h"
#include "draw.h"

typedef struct Ship* Ship_T;

/* Returns a new ship created from the parameters specified in sFilename.
   The input format is described in the sample ships' comments */
Ship_T Ship_new(char* sFilename);

/* Frees all memory occupied by oShip.  Does nothing if oShip is NULL. */
void Ship_free(Ship_T oShip);

/* Translates oShip by (dX, dY).  It is a checked runtime error for oShip to be
   NULL. */
void Ship_translate(Ship_T oShip, double dX, double dY);

/* Rotates oShip by dTheta.  It is a checked runtime error for oShip to be
   NULL. */
void Ship_rotate(Ship_T oShip, double dTheta);

/* Recenters oShip about its current center of mass.  It is a checked
   runtime error for oShip to be NULL. */
void Ship_recenter(Ship_T oShip);

/* Sets the mass and moment of inertia for oShip.  It is a checked runtime
   error for oShip to be NULL. */
void Ship_setMassMoment(Ship_T oShip);

/* Applies force fX, fY at offset dX, dY to oShip.  It is a
   checked runtime error for oShip to be NULL. */
void Ship_applyForce(Ship_T oShip, double fX, double fY, double dX,
                       double dY);
                       
/* Applies the ship's current velocities to change its position and
   rotation.  It is a checked runtime error for oShip to be NULL. */
void Ship_applyVelocities(Ship_T oShip);

/* Sets all velocities of oShip equal to a fraction of their value.  It is a
   checked runtime error for oShip to be NULL. */
void Ship_stopSlow(Ship_T oShip);

/* Apply force from all engines foward at full throttle.  It is a checked
   runtime error for oShip to be NULL. */
void Ship_fullThrottle(Ship_T oShip);

/* Apply full force from all right side engines.  It is a checked runtime
   error for oShip to be NULL. */
void Ship_fullRightThrottle(Ship_T oShip);

/* Apply full force from all left side engines.  It is a checked runtime
   error for oShip to be NULL. */
void Ship_fullLeftThrottle(Ship_T oShip);

/* Draws oShip to the screen.  It is a checked runtime error for oShip to be
   NULL. */
void Ship_draw(Ship_T oShip);

/* Centers the drawing window around oShip.  It is a checked runtime error
   for oShip to be NULL. */
void Ship_centerWindow(Ship_T oShip);

/* Returns the center point of oShip.  It is a checked runtime error for oShip
   to be NULL. */
Vector_T Ship_getCenter(Ship_T oShip);

/* Returns the speed of oShip.  It is a checked runtime error for oShip
   to be NULL. */
double Ship_getSpeed(Ship_T oShip);

/* Returns the x-velocity of oShip.  It is a checked runtime error for oShip
   to be NULL. */
double Ship_getVX(Ship_T oShip);

/* Returns the y-velocity of oShip.  It is a checked runtime error for oShip
   to be NULL. */
double Ship_getVY(Ship_T oShip);

/* Returns the radial velocity of oShip.  It is a checked runtime error for
   oShip to be NULL. */
double Ship_getOmega(Ship_T oShip);

/* Do the ships collide? */
int Ship_doIntersect(Ship_T oShip1, Ship_T oShip2);

/* Simple test AI function. */
void Ship_followRotation(Ship_T oShip, Ship_T oShip1);

/* Simple test AI function. */
void Ship_followPosition(Ship_T oShip, Ship_T oShip1);

#endif
