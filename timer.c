/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/07/2005
 * description: timer.c is an abstract object forming the implementation to
 * SDL timing functions.
 ___________________________________________________________________________*/

#include "timer.h"
#include <SDL.h>

/* The inverse of the target framerate. */
static double dInverseRate;

/* Initialize the timing module. */
void Timer_init(int iFrameRate) {
   dInverseRate = (double)iFrameRate;
}

/* Return the current time. */
Time_T Timer_getTime() {
   return SDL_GetTicks();
}

/* Return the time multiplier for updating given oLastUpdate. */
double Timer_getMultiplier(Time_T oLastUpdate) {
   return (double)(SDL_GetTicks() - oLastUpdate) / dInverseRate;
}
