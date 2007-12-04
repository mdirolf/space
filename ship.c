/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/02/2005
 * description: ship.c defines the implementation for the Ship ADT.
 *    An ship is a collection of Rectangles.
 ___________________________________________________________________________*/

#include "rect.h"
#include "draw.h"
#include "ship.h"
#include "particlesys.h"
#include "engine.h"
#include "timer.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE_LENGTH 100
#define SLOW_FACTOR 0.3

/* Structure to store the ship datatype. */
struct Ship {
       /* Array of rectangles, pointers to the engine rectangles, and the
          individual forces for each engine. */
       Rect_T* poRectArray;
       Engine_T* poEngines;
       int iNumRects; /* Number of Rects in the ship. */
       int iNumEngines; /* Number of engines in the ship. */
       
       /* Velocites. */
       double dXAc;
       double dXVel;
       double dYAc;
       double dYVel;
       double dAlpha;
       double dOmega;
       
       double dMaxOmega;
       double dMaxSpeed;
       
       /* Foward Direction */
       Vector_T oFoward;
       
       /* Mass and moment of inertia. */
       double dMass;
       double dMoment;
       
       /* Color (could be changed to an array of individual colors). */
       Color_T iColor;
       
       /* Time of last update. */
       Time_T iLastUpdate;
       
       /* Last update t units. */
       double dLastTLength;
       
       /* Needed for the PID controllers for following... can't think of
          anywhere better to put these. */
       double lastErrorRot;
       double lastLastErrorRot;
       double lastErrorPos;
       double lastLastErrorPos;
       };

/* Returns a new ship created from the parameters specified in sFilename.
   The input format is described in the ship.c */
Ship_T Ship_new(char* sFilename) {
   FILE* inputStream;
   Ship_T oNewShip; /* Our new ship. */
   int num, numEngines, i;
   int r, g, b; /* The colors. */
   char dummyString[MAX_LINE_LENGTH]; /* Used to skip lines. */
   double xext, yext, xoffs, yoffs, theta, mass, thrust;
   
   /* Open the stream. */
   inputStream = fopen(sFilename, "r");
   
   /* Skip lines one and two, then read the number of rects. */
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);
   fscanf(inputStream, "%d\n", &num);
   
   /* Skip line four and read the number of engines. */
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);
   fscanf(inputStream, "%d\n", &numEngines);
   
   /* Allocate space for the new ship. */
   oNewShip = (Ship_T)malloc(sizeof(struct Ship));
   assert(oNewShip != NULL);
   
   /* Allocate space for the array of rectangles, and store how many. */
   oNewShip->poRectArray = (Rect_T*)malloc(num * sizeof(Rect_T));
   assert(oNewShip->poRectArray != NULL);
   oNewShip->iNumRects = num;
   
   /* Allocate space for the array of engines, and store how many. */
   oNewShip->poEngines = (Engine_T*)malloc(numEngines * sizeof(Engine_T));
   assert(oNewShip->poEngines != NULL);
   oNewShip->iNumEngines = numEngines;
   
   /* Skip line six. */
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);
   
   /* Read in the rectangles that aren't engines. */
   for(i = 0; i < (num - numEngines); i++) {
      fscanf(inputStream, "%lf %lf %lf %lf %lf %lf\n", &xext, &yext,
             &xoffs, &yoffs, &theta, &mass);
      oNewShip->poRectArray[i] = Rect_new(xext, yext, mass);
      Rect_translateRect(oNewShip->poRectArray[i], xoffs, yoffs);
      Rect_rotateRect(oNewShip->poRectArray[i], theta);
   }
   
   /* Skip the precurser to the engines list. */
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);
   

   /* Read in the engines. */
   for(i = 0; i < numEngines; i++) {
      fscanf(inputStream, "%lf %lf %lf %lf %lf %lf %lf\n", &xext, &yext,
             &xoffs, &yoffs, &theta, &mass, &thrust);
      oNewShip->poRectArray[i + num - numEngines] = Rect_new(xext, yext, mass);
      Rect_translateRect(oNewShip->poRectArray[i + num - numEngines],
                         xoffs, yoffs);
      Rect_rotateRect(oNewShip->poRectArray[i + num - numEngines], theta);
      
      oNewShip->poEngines[i] = Engine_new(oNewShip->poRectArray[i + num - numEngines], thrust);
   }
   
   /* Skip the precurser to the color. */
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);
   
   /* Get the color. */
   fscanf(inputStream, "%d %d %d\n", &r, &g, &b);
   oNewShip->iColor = Draw_getColor((char)r, (char)g, (char)b);
   
   /* Skip the precurser to the max speeds. */
   fgets(dummyString, MAX_LINE_LENGTH, inputStream);

   /* Get the max speeds. */
   fscanf(inputStream, "%lf %lf\n", &oNewShip->dMaxSpeed, &oNewShip->dMaxOmega);
   
   /* Close the input stream. */
   fclose(inputStream);
   
   /* Center the ship and get its moment. */
   Ship_recenter(oNewShip);
   Ship_setMassMoment(oNewShip);
   
   /* Initialize direction and motion vectors. */
   oNewShip->oFoward = Vector_new(0, 1);
   oNewShip->dXAc = oNewShip->dYAc = oNewShip->dAlpha = 0;
   oNewShip->dXVel = oNewShip->dYVel = oNewShip->dOmega = 0;

   /* Initialize timing values. */
   oNewShip->iLastUpdate = Timer_getTime();
   oNewShip->dLastTLength = 0;
   
   /* Initialize error values for PID control. */
   oNewShip->lastErrorRot = 0;
   oNewShip->lastLastErrorRot = 0;
   oNewShip->lastErrorPos = 0;
   oNewShip->lastLastErrorPos = 0;
   
   return oNewShip;
}

/* Frees all memory occupied by oShip.  Does nothing if oShip is NULL. */
void Ship_free(Ship_T oShip) {
   int i;
   if (oShip == NULL) return;
   
   for(i = 0; i < oShip->iNumRects; i++) {
      Rect_free(oShip->poRectArray[i]);
   }
   
   for(i = 0; i < oShip->iNumEngines; i++) {
      Engine_free(oShip->poEngines[i]);
   }
   
   Vector_free(oShip->oFoward);
   
   free(oShip->poEngines);
   free(oShip->poRectArray);
   free(oShip);
}

/* Translates oShip by (dX, dY).  It is a checked runtime error for oShip to be
   NULL. */
void Ship_translate(Ship_T oShip, double dX, double dY) {
   int i;
   assert(oShip != NULL);
   
   for(i = 0; i < oShip->iNumRects; i++) {
      Rect_translateObj(oShip->poRectArray[i], dX, dY);
   }
}

/* Rotates oShip by dTheta.  It is a checked runtime error for oShip to be
   NULL. */
void Ship_rotate(Ship_T oShip, double dTheta) {
   int i;
   assert(oShip != NULL);

   Vector_rotate(oShip->oFoward, dTheta);

   for(i = 0; i < oShip->iNumRects; i++) {
      Rect_rotateObj(oShip->poRectArray[i], dTheta);
   }
}

/* Recenters oShip about its current center of mass.  It is a checked
   runtime error for oShip to be NULL. */
void Ship_recenter(Ship_T oShip) {
   double totMass = 0, xMass = 0, yMass = 0, deltX = 0, deltY = 0;
   int i;
   Vector_T currVector;
   
   assert(oShip != NULL);
   
   for(i = 0; i < oShip->iNumRects; i++) {
      totMass += Rect_getMass(oShip->poRectArray[i]);
      xMass += Rect_getMass(oShip->poRectArray[i]) *
               Vector_getX(Rect_getOffset(oShip->poRectArray[i]));
      yMass += Rect_getMass(oShip->poRectArray[i]) *
               Vector_getY(Rect_getOffset(oShip->poRectArray[i]));
   }
   deltX = xMass / totMass;
   deltY = yMass / totMass;
   
   for(i = 0; i < oShip->iNumRects; i++) {
      currVector = Rect_getOffset(oShip->poRectArray[i]);
      Vector_setX(currVector, Vector_getX(currVector) - deltX);
      Vector_setY(currVector, Vector_getY(currVector) - deltY);
   }
}

/* Sets the mass and moment of inertia for oShip.  It is a checked runtime
   error for oShip to be NULL. */
void Ship_setMassMoment(Ship_T oShip) {
   int i;
   double moment = 0, mass = 0;
   assert(oShip != NULL);

   for(i = 0; i < oShip->iNumRects; i++) {
      moment += Rect_getMoment(oShip->poRectArray[i]);
      mass += Rect_getMass(oShip->poRectArray[i]);
   }
   oShip->dMass = mass;
   oShip->dMoment = moment;
}

/* Applies force fX, fY at offset dX, dY to oShip.  It is a
   checked runtime error for oShip to be NULL. */
void Ship_applyForce(Ship_T oShip, double fX, double fY, double dX,
                       double dY) {
   assert(oShip != NULL);
   oShip->dXAc += fX / oShip->dMass;
   oShip->dYAc += fY / oShip->dMass;
   
   oShip->dAlpha += (dX * fY - dY * fX) / oShip->dMoment;
}

/* Applies the ship's current velocities to change its position and
   rotation.  Operates for duration t. It is a checked runtime error for oShip
   to be NULL. */
void Ship_applyVelocities(Ship_T oShip) {
   double deltX, deltY, deltTheta, t, speedsqr;
   int i;
   
   assert(oShip != NULL);
   
   oShip->dLastTLength = t = Timer_getMultiplier(oShip->iLastUpdate);
   oShip->iLastUpdate = Timer_getTime();
   oShip->dXVel += oShip->dXAc * t;
   oShip->dYVel += oShip->dYAc * t;
   oShip->dOmega += oShip->dAlpha * t;

   speedsqr = oShip->dXVel * oShip->dXVel + oShip->dYVel * oShip->dYVel;
   if(speedsqr > oShip->dMaxSpeed * oShip->dMaxSpeed) {
      oShip->dXVel *= oShip->dMaxSpeed / sqrt(speedsqr);
      oShip->dYVel *= oShip->dMaxSpeed / sqrt(speedsqr);
   }
   
   if (oShip->dOmega > oShip->dMaxOmega)
      oShip->dOmega = oShip->dMaxOmega;
   if (oShip->dOmega < -oShip->dMaxOmega)
      oShip->dOmega = -oShip->dMaxOmega;

   deltX = oShip->dXVel * t;
   deltY = oShip->dYVel * t;
   deltTheta = oShip->dOmega * t;
   
   oShip->dXAc = oShip->dYAc = oShip->dAlpha = 0;
   
   Vector_rotate(oShip->oFoward, deltTheta);
   for(i = 0; i < oShip->iNumRects; i++) {
      Rect_translateObj(oShip->poRectArray[i], deltX, deltY);
      Rect_rotateObj(oShip->poRectArray[i], deltTheta);
   }
}

/* Sets all velocities of oShip equal to a fraction of their value.  It is a
   checked runtime error for oShip to be NULL. */
void Ship_stopSlow(Ship_T oShip) {
   double t;
   assert(oShip != NULL);
   t = oShip->dLastTLength;
   oShip->dXVel -= oShip->dXVel*SLOW_FACTOR*t;
   oShip->dYVel -= oShip->dYVel*SLOW_FACTOR*t;
   /*Ship_applyForce(oShip, -30*oShip->dXVel / t, -30*oShip->dYVel / t, 0, 0);
   */
   oShip->dOmega -= oShip->dOmega*t*SLOW_FACTOR;
}

/* Apply force from all engines foward at full throttle.  It is a checked
   runtime error for oShip to be NULL. */
void Ship_fullThrottle(Ship_T oShip) {
   int i;
   assert(oShip != NULL);
   
   oShip->dOmega -= oShip->dOmega*oShip->dLastTLength*SLOW_FACTOR;
   /*oShip->dOmega = 0;*/
   for(i = 0; i < oShip->iNumEngines; i++) {
      Engine_thrust(oShip->poEngines[i], oShip, oShip->dLastTLength);
   }
}

/* Apply full force from all right side engines.  It is a checked runtime
   error for oShip to be NULL. */
void Ship_fullRightThrottle(Ship_T oShip) {
   int i;
   assert(oShip != NULL);
   for(i = 0; i < oShip->iNumEngines; i++) {
      Engine_thrustRight(oShip->poEngines[i], oShip, oShip->dLastTLength);
   }
}

/* Apply full force from all left side engines.  It is a checked runtime
   error for oShip to be NULL. */
void Ship_fullLeftThrottle(Ship_T oShip) {
   int i;
   assert(oShip != NULL);
   for(i = 0; i < oShip->iNumEngines; i++) {
      Engine_thrustLeft(oShip->poEngines[i], oShip, oShip->dLastTLength);
   }
}

/* Draws oShip to the screen.  It is a checked runtime error for oShip to be
   NULL. */
void Ship_draw(Ship_T oShip) {
   int i;
   assert(oShip != NULL);
   
   for(i = 0; i < oShip->iNumRects; i++) {
      Rect_draw(oShip->poRectArray[i], oShip->iColor);
   }
   for(i = 0; i < oShip->iNumEngines; i++) {
      Engine_drawSmoke(oShip->poEngines[i], oShip->dLastTLength);
   }
}

/* Centers the drawing window around oShip.  It is a checked runtime error
   for oShip to be NULL. */
void Ship_centerWindow(Ship_T oShip) {
   double speedsquared = oShip->dXVel * oShip->dXVel + oShip->dYVel * oShip->dYVel + 1;
   Draw_setScale(500 / (2 * sqrt(speedsquared) + 1000));
   Draw_shiftToPoint(Vector_getX(Rect_getCenter(oShip->poRectArray[0])), Vector_getY(Rect_getCenter(oShip->poRectArray[0])));
}

/* Returns the center point of oShip.  It is a checked runtime error for oShip
   to be NULL. */
Vector_T Ship_getCenter(Ship_T oShip) {
   assert(oShip != NULL);
   return(Rect_getCenter(oShip->poRectArray[0]));
}

/* Returns the speed of oShip.  It is a checked runtime error for oShip
   to be NULL. */
double Ship_getSpeed(Ship_T oShip) {
   assert(oShip != NULL);
   return sqrt(oShip->dXVel * oShip->dXVel + oShip->dYVel * oShip->dYVel);
}

/* Returns the x-velocity of oShip.  It is a checked runtime error for oShip
   to be NULL. */
double Ship_getVX(Ship_T oShip) {
   assert(oShip != NULL);
   return oShip->dXVel;
}

/* Returns the y-velocity of oShip.  It is a checked runtime error for oShip
   to be NULL. */
double Ship_getVY(Ship_T oShip) {
   assert(oShip != NULL);
   return oShip->dYVel;
}

/* Returns the radial velocity of oShip.  It is a checked runtime error for
   oShip to be NULL. */
double Ship_getOmega(Ship_T oShip) {
   assert(oShip != NULL);
   return oShip->dOmega;
}

/* Do the ships collide? */
int Ship_doIntersect(Ship_T oShip1, Ship_T oShip2) {
   int i, j;
   for(i = 0; i < oShip1->iNumRects; i++) {
      for(j = 0; j < oShip2->iNumRects; j++) {
         if(Rect_doIntersect(oShip1->poRectArray[i], oShip2->poRectArray[j]))
            return 1;
      }
   }
   return 0;
}

/* These two follow functions use PID controller logic, as found in
   thermostats and cruise control. */
/* Simple test AI function. */
void Ship_followRotation(Ship_T oShip, Ship_T oShip1) {
   double lastE = oShip->lastErrorRot;
   double lastLastE = oShip->lastLastErrorRot;
   double C = 0.0;
   double error;
   double time = oShip->dLastTLength;
   /* Constants for each section. */
   double KP = 300, KI = 0.01, KD = 0.00001;
   /* Proportional, integral, derivative, correction */
   double P, I, D;

   double xDir = Vector_getX(oShip->oFoward);
   double yDir = Vector_getY(oShip->oFoward);
   
   double xDiff = Vector_getX(Ship_getCenter(oShip1)) -
                  Vector_getX(Ship_getCenter(oShip));
   double yDiff = Vector_getY(Ship_getCenter(oShip1)) -
                  Vector_getY(Ship_getCenter(oShip));

   /* The error in angle is the cross product between this ships direction
      and the vector between the ships */
   error = xDir * yDiff - yDir * xDiff;

   P = (error - lastE);
   I = KI * time * error;
   D = KD / time * (error - 2 * lastE + lastLastE);
   
   C = KP * (P + I + D);
   
   if(C < -50) {
      Ship_fullLeftThrottle(oShip);
   }
   else if(C > 50) {
      Ship_fullRightThrottle(oShip);
   }
   
   oShip->lastLastErrorRot = lastE;
   oShip->lastErrorRot = error;
   
}

/* Simple test AI function. */
void Ship_followPosition(Ship_T oShip, Ship_T oShip1) {
   double lastError = oShip->lastErrorPos;
   double lastLastError = oShip->lastLastErrorPos;

   double error;
   double time = oShip->dLastTLength;
   /* Constants for each section. */
   double KP = 800, KI = 2, KD = 0.05;
   /* Proportional, integral, derivative, correction */
   double P, I, D, C;

   double xDiff = Vector_getX(Ship_getCenter(oShip1)) -
                  Vector_getX(Ship_getCenter(oShip)) -
                  750 * oShip1->dXVel / Ship_getSpeed(oShip1);

   double yDiff = Vector_getY(Ship_getCenter(oShip1)) -
                  Vector_getY(Ship_getCenter(oShip)) -
                  750 * oShip1->dYVel / Ship_getSpeed(oShip1);
   
   double vx = Ship_getVX(oShip);
   double vy = Ship_getVY(oShip);

   /* The error in position is the distance between the ships */
   error = sqrt(xDiff * xDiff + yDiff * yDiff);

   P = (error - lastError);
   I = KI * time * error;
   D = KD / time * (error - 2 * lastError + lastLastError);

   C = KP * (P + I + D);

   if(C < 0 || vx * xDiff + vy * yDiff < 0) {
      Ship_stopSlow(oShip);
   }
   else if(C > 0) {
      Ship_fullThrottle(oShip);
   }

   oShip->lastLastErrorPos = lastError;
   oShip->lastErrorPos = error;
}
