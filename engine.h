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
