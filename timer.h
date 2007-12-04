/*____________________________________________________________________________
 * name: Michael Dirolf
 * date: 12/07/2005
 * description: timer.h is an abstract object forming the interface to SDL
 * timing functions.
 ___________________________________________________________________________*/

#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>

/* Define the Time_T type, making it easier to pass times around between
   functions and data types. */
typedef Uint32 Time_T;

/* Initialize the timing module. */
void Timer_init(int iFrameRate);

/* Return the current time. */
Time_T Timer_getTime();

/* Return the time multiplier for updating given oLastUpdate. */
double Timer_getMultiplier(Time_T oLastUpdate);

#endif
