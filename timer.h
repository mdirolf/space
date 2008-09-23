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
 * date: 12/07/2005
 * description: timer.h is an abstract object forming the interface to SDL
 * timing functions.
 ___________________________________________________________________________*/

#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

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
