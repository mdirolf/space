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
