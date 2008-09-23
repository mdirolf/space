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
 * date: 12/05/2005
 * description: engine.c defines the implementation of the Engine ADT.
 ___________________________________________________________________________*/

#include "engine.h"
#include "rect.h"
#include "ship.h"
#include "draw.h"
#include "particlesys.h"
#include "math.h"
#include <assert.h>
#include <stdlib.h>

#define CCW_TOLERANCE 0.1
#define DECAY_THRESHOLD 0.05
#define PARTS_PER_LOG_THRUST_SMOKE 8
#define PART_PER_LENGTH_SMOKE 0.5
#define PARTS_PER_LOG_THRUST_ORANGE 3
#define PART_PER_LENGTH_ORANGE 6
#define PARTS_PER_LOG_THRUST_RED 3
#define PART_PER_LENGTH_RED 2

/* Structure to store the engines datatype. */
struct Engine {
       Rect_T oRect;
       double dThrust;
       ParticleSys_T oSmoke;
       ParticleSys_T oRedFlame;
       ParticleSys_T oOrangeFlame;
       int isThrusting;
       };

/* Returns a new engine with shape rect and thrust thrust. */
Engine_T Engine_new(Rect_T rect, double thrust) {
   Engine_T oNewEngine = (Engine_T)malloc(sizeof(struct Engine));
   oNewEngine->oRect = rect;
   oNewEngine->dThrust = thrust;
   oNewEngine->oSmoke = ParticleSys_new((int)(log(thrust) * PARTS_PER_LOG_THRUST_SMOKE), Draw_getColor((char)230, (char)230, (char)230));
   oNewEngine->oRedFlame = ParticleSys_new((int)(log(thrust) * PARTS_PER_LOG_THRUST_RED), Draw_getColor((char)255, 20, 20));
   oNewEngine->oOrangeFlame = ParticleSys_new((int)(log(thrust) * PARTS_PER_LOG_THRUST_ORANGE), Draw_getColor((char)180, (char)140, 60));

   return oNewEngine;
}

/* Frees all memory occupied by oEngine.  Does nothing if oEngine is NULL. */
void Engine_free(Engine_T oEngine) {
   if(oEngine == NULL)
      return;
   ParticleSys_free(oEngine->oSmoke);
   ParticleSys_free(oEngine->oRedFlame);
   ParticleSys_free(oEngine->oOrangeFlame);
   free(oEngine);
}

/* Fires oEngine on oShip for time t.  It is a checked runtime error for
   oEngine or oShip to be NULL. */
void Engine_thrust(Engine_T oEngine, Ship_T oShip, double t) {
   double xcen, ycen, xoff, yoff, xdir, ydir, xsmokeX, ysmokeX, xsmokeY, ysmokeY, vx, vy, mag;
   int i;
   assert(oShip != NULL && oEngine != NULL);

   oEngine->isThrusting = 1;

   /* Center of engine's ship. */
   xcen = Vector_getX(Rect_getCenter(oEngine->oRect));
   ycen = Vector_getY(Rect_getCenter(oEngine->oRect));
   /* Offset of engine itself. */
   xoff = Vector_getX(Rect_getOffset(oEngine->oRect));
   yoff = Vector_getY(Rect_getOffset(oEngine->oRect));
   /* Direction of engine thrust vector. */
   xdir = Vector_getX(Rect_getYAxis(oEngine->oRect));
   ydir = Vector_getY(Rect_getYAxis(oEngine->oRect));
   /* Ship's velocity vector. */
   vx = Ship_getVX(oShip);
   vy = Ship_getVY(oShip);

   mag = oEngine->dThrust;

   /* These determine the random placement of smoke dots. */
   xsmokeX = Vector_getX(Rect_getXAxis(oEngine->oRect)) * Rect_getXExt(oEngine->oRect);
   ysmokeX = Vector_getY(Rect_getXAxis(oEngine->oRect)) * Rect_getXExt(oEngine->oRect);
   xsmokeY = Vector_getX(Rect_getYAxis(oEngine->oRect)) * Rect_getYExt(oEngine->oRect);
   ysmokeY = Vector_getY(Rect_getYAxis(oEngine->oRect)) * Rect_getYExt(oEngine->oRect);

   /* Apply the force. */
   Ship_applyForce(oShip, xdir*mag, ydir*mag, xoff, yoff);

   /* Add some randomized smoke. */
   for(i = 0; i < PART_PER_LENGTH_SMOKE * Rect_getXExt(oEngine->oRect); i++) {
      double drand1 = (RAND_MAX / 2.0 - rand()) / (double)RAND_MAX;
      double drand2 = rand() / (double)RAND_MAX;
      ParticleSys_add(oEngine->oSmoke, xoff + xcen + drand1 * xsmokeX - xsmokeY + drand2*vx*t,
                      yoff + ycen + drand1 * ysmokeX - ysmokeY + drand2*vy*t);
   }
   for(i = 0; i < PART_PER_LENGTH_RED * Rect_getXExt(oEngine->oRect); i++) {
      double drand1 = (RAND_MAX / 2.0 - rand()) / (double)RAND_MAX;
      double drand2 = rand() / (double)RAND_MAX;
      ParticleSys_add(oEngine->oRedFlame, xoff + xcen + drand1 * xsmokeX - xsmokeY + drand2*vx*t,
                      yoff + ycen + drand1 * ysmokeX - ysmokeY + drand2*vy*t);
   }
   for(i = 0; i < PART_PER_LENGTH_ORANGE * Rect_getXExt(oEngine->oRect); i++) {
      double drand1 = (RAND_MAX / 2.0 - rand()) / (double)RAND_MAX;
      double drand2 = rand() / (double)RAND_MAX;
      ParticleSys_add(oEngine->oOrangeFlame, xoff + xcen + drand1 * xsmokeX - xsmokeY + drand2*vx*t,
                      yoff + ycen + drand1 * ysmokeX - ysmokeY + drand2*vy*t);
   }
}


/* Fires oEngine on oShip if it is a right side engine.  It is a checked
   runtime error for oEngine or oShip to be NULL. */
void Engine_thrustRight(Engine_T oEngine, Ship_T oShip, double t) {
   double xpos, ypos, xdir, ydir;
   assert(oEngine != NULL && oShip != NULL);

   xpos = Vector_getX(Rect_getOffset(oEngine->oRect));
   ypos = Vector_getY(Rect_getOffset(oEngine->oRect));
   xdir = Vector_getX(Rect_getYAxis(oEngine->oRect));
   ydir = Vector_getY(Rect_getYAxis(oEngine->oRect));
   if((ydir * xpos - xdir * ypos) > CCW_TOLERANCE)
      Engine_thrust(oEngine, oShip, t);
}

/* Fires oEngine on oShip if it is a left side engine.  It is a checked
   runtime error for oEngine or oShip to be NULL. */
void Engine_thrustLeft(Engine_T oEngine, Ship_T oShip, double t) {
   double xpos, ypos, xdir, ydir;
   assert(oEngine != NULL && oShip != NULL);

   xpos = Vector_getX(Rect_getOffset(oEngine->oRect));
   ypos = Vector_getY(Rect_getOffset(oEngine->oRect));
   xdir = Vector_getX(Rect_getYAxis(oEngine->oRect));
   ydir = Vector_getY(Rect_getYAxis(oEngine->oRect));
   if((ydir * xpos - xdir * ypos) < -CCW_TOLERANCE)
      Engine_thrust(oEngine, oShip, t);
}

/* Draws the smoke of oEngine.  It is a checked runtime error
   for oEngine to be NULL. */
void Engine_drawSmoke(Engine_T oEngine, double t) {
   assert(oEngine != NULL);
   Engine_decaySmoke(oEngine, t);
   ParticleSys_draw(oEngine->oSmoke);
   if(oEngine->isThrusting) {
      ParticleSys_draw(oEngine->oRedFlame);
      ParticleSys_draw(oEngine->oOrangeFlame);
   }
   oEngine->isThrusting = 0;
}

/* Decays the smoke of oEngine.  It is a checked runtime error
   for oEngine to be NULL. */
void Engine_decaySmoke(Engine_T oEngine, double t) {
   static double time = 0;
   assert(oEngine != NULL);
   time += t;
   if (time > DECAY_THRESHOLD) {
      ParticleSys_decay(oEngine->oSmoke);
      ParticleSys_decay(oEngine->oRedFlame);
      ParticleSys_decay(oEngine->oOrangeFlame);
      time -= DECAY_THRESHOLD;
   }
}
