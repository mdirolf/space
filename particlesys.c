/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/05/2005
 * description: particlesys.c defines the implementation for the ParticleSys
   ADT.
 ___________________________________________________________________________*/

#include "draw.h"
#include "particlesys.h"
#include <assert.h>
#include <stdlib.h>

/* Structure to store the systems datatype. */
struct ParticleSys {
       /* Array of points. [iCount][2], where the 2 is x and y */
       double* pdPointArray;
       
       /* Number of points. */
       int iNum;
       
       /* Number of used slots. */
       int iNumUsed;

       /* Color. */
       Color_T iColor;
       };


/* Creates a new system of iCount particles, with color iCol. */
ParticleSys_T ParticleSys_new(int iCount, Color_T iCol) {
   ParticleSys_T oNewSys;
   
   oNewSys = (ParticleSys_T)malloc(sizeof(struct ParticleSys));
   assert(oNewSys != NULL);
   
   oNewSys->iColor = iCol;
   oNewSys->iNum = iCount;
   oNewSys->iNumUsed = 0;

   /* Note the constant 2 here is legit because that is the number of
      dimensions needed to represent a point. */
   oNewSys->pdPointArray = (double*)malloc(iCount * 2 * sizeof(double));
   assert(oNewSys->pdPointArray != NULL);
   
   return oNewSys;
}

/* Frees all memory occupied by oPSys.  Does nothing if oPSys is NULL. */
void ParticleSys_free(ParticleSys_T oPSys) {
   if (oPSys == NULL)
      return;
   free(oPSys->pdPointArray);
   free(oPSys);
}

/* Adds a particle at point (dX, dY) to oPSys.  It is a checked runtime error
   for oPSys to be NULL. */
void ParticleSys_add(ParticleSys_T oPSys, double dX, double dY) {
   int pos;
   assert(oPSys != NULL);
   
   if(oPSys->iNumUsed < oPSys->iNum) {
      oPSys->pdPointArray[2 * oPSys->iNumUsed] = dX;
      oPSys->pdPointArray[2 * oPSys->iNumUsed + 1] = dY;
      oPSys->iNumUsed++;
      return;
   }
   pos = (int)((double)rand() / (RAND_MAX) * oPSys->iNum);
   oPSys->pdPointArray[2 * pos] = dX;
   oPSys->pdPointArray[2 * pos + 1] = dY;
}

/* Removes a random particle from oPSys.  It is a checked runtime error for
   oPSys to be NULL. */
void ParticleSys_decay(ParticleSys_T oPSys) {
   int pos, num;
   assert(oPSys != NULL);
   
   num = oPSys->iNumUsed - 1;
   if (num < 0) return;
   pos = (int)(rand() * num / RAND_MAX );

   oPSys->pdPointArray[2 * pos] = oPSys->pdPointArray[2 * num];
   oPSys->pdPointArray[2 * pos + 1] = oPSys->pdPointArray[2 * num + 1];
   oPSys->iNumUsed--;

}

/* Draws oPSys to the screen.  It is a checked runtime error for oPSys to
   be NULL. */
void ParticleSys_draw(ParticleSys_T oPSys) {
   int i;
   assert(oPSys != NULL);
   for(i = 0; i < oPSys->iNumUsed; i++) {
      Draw_drawScaledPixel(oPSys->pdPointArray[2 * i],
                           oPSys->pdPointArray[2 * i + 1], oPSys->iColor);
   }
}
   
