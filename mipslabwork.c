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

#define SIZE_OF_DISPLAY 128

int score = 0;
int difficulty = 0;
char textstring[] = "";
int posArray = 0;
int yPos = 0;
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

    PR2 = (80000000/256)/difficulty;
  	// interupt flag can be found in IFS0 BIT 8
  	// 1:256
  	T2CON = 0x70;
  	TMR2 = 0;
  	T2CONSET = 0x8000; // Start the timer

  return;
}

void move_snakeConstant(void){
  int temp = display[sizeof(display)/sizeof(uint8_t) - 1];
  int i;
  for(i = sizeof(display)/sizeof(uint8_t) - 1; i > 0; i--){
    display[i] = display[i - 1];
  }
  display[0] = temp;
}

void display_clear(void){
  int clr;
  for(clr = 0; clr < 128; clr += 32){
    display_image(clr, clear);
  }
}

void update_score(void){
  score += 10;
}

int get_position(void){
  int i;
  int positionX;
  for(i = 0; i < SIZE_OF_DISPLAY; i++){
    if(display[i] != 255)
      positionX = i;
  }
  return positionX;
}

void move_Right(void){
  posArray++;
}

void move_Left(void){
  posArray--;
}

void move_Down(void){

}

int xCoordinate(int posArray){
  if(posArray <= 31)
    return posArray;

  if(posArray > 31 && posArray <= 63)
    return posArray - 32;

  if(posArray > 63 && posArray <= 95)
    return posArray - 64;

  if(posArray > 95 && posArray <= 127)
    return posArray - 96;
}

int yCoordinate(int posArray){
  if(posArray <= 31)
    return 0;

  if(posArray > 31 && posArray <= 63)
    return 1;

  if(posArray > 63 && posArray <= 95)
    return 2;

  if(posArray > 95 && posArray <= 127)
    return 3;
}

void set_bar(int posArray, int yPos, int amount){
  int i;
  for(i = 0; i < SIZE_OF_DISPLAY; i++){
    if(i != posArray && )
      display[i] = 255;
    else
      display[i] = amount;
  }
  //posArray ++;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int *porte = (volatile int *) 0xbf886110;
  *porte += 0x01;
  int but;

  posArray = get_position();
  yPos = yCoordinate(posArray);

  if(getsw() == 0x3)
    difficulty = 100;

  if(getsw() == 0x2)
    difficulty = 500;

  if(getsw() == 0x1)
    difficulty = 1000;

  if(but = getbtns()){
    if(but & 4){
      move_Down();
    }
    if(but & 2){
      move_Left();
    }
    if(but & 1){
      move_Right();
    }
  }


  delay(difficulty);
  display_update();
  display_clear();
  set_bar(xCoordinate(posArray),254);
  display_image(0,display);
  //move_snakeConstant();
  //display_image(posPage,display);
  //move_nextPage();
  //display_string(0,score);
}
