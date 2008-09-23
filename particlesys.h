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
 * description: particlesys.h defines the interface for the particle
 *    system ADT.
 ___________________________________________________________________________*/

#ifndef PARTICLESYS_H
#define PARTICLESYS_H

#include "draw.h"

typedef struct ParticleSys* ParticleSys_T;

/* Creates a new system of iCount particles, with color iCol. */
ParticleSys_T ParticleSys_new(int iCount, Color_T iCol);

/* Frees all memory occupied by oPSys.  Does nothing if oPSys is NULL. */
void ParticleSys_free(ParticleSys_T oPSys);

/* Adds a particle at point (dX, dY) to oPSys.  It is a checked runtime error
   for oPSys to be NULL. */
void ParticleSys_add(ParticleSys_T oPSys, double dX, double dY);

/* Removes a random particle from oPSys.  It is a checked runtime error for
   oPSys to be NULL. */
void ParticleSys_decay(ParticleSys_T oPSys);

/* Draws oPSys to the screen.  It is a checked runtime error for oPSys to
   be NULL. */
void ParticleSys_draw(ParticleSys_T oPSys);

#endif
