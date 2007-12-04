/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/05/2005
 * description: engine.h defines the interface for the Engine ADT.
 ___________________________________________________________________________*/

#ifndef ENGINE_H
#define ENGINE_H

#include "rect.h"
#include "ship.h"

typedef struct Engine* Engine_T;

/* Returns a new engine with shape oRect and thrust dThrust. */
Engine_T Engine_new(Rect_T oRect, double dThrust);

/* Frees all memory occupied by oEngine.  Does nothing if oEngine is NULL. */
void Engine_free(Engine_T oEngine);

/* Fires oEngine on oShip for time t.  It is a checked runtime error for
   oEngine or oShip to be NULL. */
void Engine_thrust(Engine_T oEngine, Ship_T oShip, double t);

/* Fires oEngine on oShip if it is a right side engine.  It is a checked
   runtime error for oEngine or oShip to be NULL. */
void Engine_thrustRight(Engine_T oEngine, Ship_T oShip, double t);

/* Fires oEngine on oShip if it is a left side engine.  It is a checked
   runtime error for oEngine or oShip to be NULL. */
void Engine_thrustLeft(Engine_T oEngine, Ship_T oShip, double t);

/* Draws the smoke of oEngine.  It is a checked runtime error
   for oEngine to be NULL. */
void Engine_drawSmoke(Engine_T oEngine, double t);

/* Decays the smoke of oEngine.  It is a checked runtime error
   for oEngine to be NULL. */
void Engine_decaySmoke(Engine_T oEngine, double t);

#endif
