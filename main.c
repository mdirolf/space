#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "draw.h"
#include "rect.h"
#include "vector.h"
#include "ship.h"
#include "particlesys.h"
#include "timer.h"

#include <SDL/SDL.h>

#define FPS 100

Color_T black;
Color_T white;

void DrawScene(Ship_T oShip, Ship_T oShip2, Ship_T oShip3)
{
   Draw_lockScreen();
   
   Draw_clearScreen();
   /*  for(x = -50; x < 50; x++)
       for(y = -50; y < 50; y++)
       Draw_drawScaledPixel(x*150, y*150, white);
   */
   Ship_applyVelocities(oShip);
   Ship_applyVelocities(oShip2);
   Ship_applyVelocities(oShip3);
   Ship_centerWindow(oShip);
   Ship_draw(oShip);
   Ship_draw(oShip2);
   Ship_draw(oShip3);/*
                       Vector_drawBetween(Ship_getCenter(oShip), Ship_getCenter(oShip2), white);
                       Vector_drawBetween(Ship_getCenter(oShip), Ship_getCenter(oShip3), white);*/
   Draw_flipScreen();
   Draw_unlockScreen();
}

int main(int argc, char *argv[])
{
   int done=0;
   Ship_T oShip, oShip2, oShip3;
   
   Draw_init();
   Timer_init(FPS);
   
   black = Draw_getColor(0,0,0);
   white = Draw_getColor((unsigned char)255,
                         (unsigned char)255,
                         (unsigned char)255);
   
   oShip = Ship_new("ship_xwingjr");
   oShip2 = Ship_new("ship_xwing");
   oShip3 = Ship_new("ship_xwing");
   
   Ship_translate(oShip2, 1000, 1000);
   Ship_translate(oShip3, -100, -100);
   
   while(done == 0)
   {
      SDL_Event event;
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (keystate[SDLK_ESCAPE]) done = 1;
      if (keystate[SDLK_UP])
         Ship_fullThrottle(oShip);
      else {
         if (keystate[SDLK_RIGHT])
            Ship_fullLeftThrottle(oShip);
         if (keystate[SDLK_LEFT])
            Ship_fullRightThrottle(oShip);
      }
      Ship_followRotation(oShip3, oShip);
      Ship_followPosition(oShip3, oShip);
      Ship_followRotation(oShip2, oShip);
      Ship_followPosition(oShip2, oShip);
      
/*   if(Ship_doIntersect(oShip2, oShip3)) {
     Ship_T temp = oShip;
     oShip = oShip2;
     oShip2 = temp;
     }
*/   
      if (keystate[SDLK_DOWN])
         Ship_stopSlow(oShip);
      while ( SDL_PollEvent(&event) )
      {
         if ( event.type == SDL_QUIT )  done = 1;
         if ( event.type == SDL_KEYDOWN )
            if ( event.key.keysym.sym == SDLK_SPACE ) {
               Ship_T temp = oShip;
               oShip = oShip2;
               oShip2 = temp;
            }
         
      }
      
      DrawScene(oShip, oShip2, oShip3);
   }
   
   Ship_free(oShip);
   Ship_free(oShip2);
   return 0;
}

