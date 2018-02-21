/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int posPage = 0;
int snakeSize = 8;
int count = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{

    volatile int *trise = (volatile int *) 0xbf886100;
    *trise = *trise & 0x00;
    TRISE = TRISE & 0x0fe0;

  return;
}

void move_snakeRight(void){
  int temp = display[sizeof(display)/sizeof(uint8_t) - 1];
  int i;
  for(i = sizeof(display)/sizeof(uint8_t) - 1; i > 0; i--){
    display[i] = display[i - 1];
  }
  display[0] = temp;
  count ++;
}
/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int *porte = (volatile int *) 0xbf886110;

  *porte += 0x01;
  int but;
  /*if(but = getbtns()){
    if(but & 4)


    if(but & 2)


    if(but & 1)

    }
    */


    delay( 100 );
    //time2string( textstring, mytime );
    //display_string( 3, textstring );
    //display_update();
    //tick( &mytime );
    display_update();
    display_image(0,clear);
    display_image(32,clear);
    display_image(64,clear);
    display_image(96,clear);
    move_snakeRight();
    if(count >= 32 - snakeSize){
      posPage += 32;
      count = 0;
    }
    display_image(posPage,display);

}
