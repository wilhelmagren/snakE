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

#define HEIGHT 4
#define WIDTH 32
#define SIZE_OF_PAGE HEIGHT*WIDTH
#define PIXEL_UNIT 8
#define NUMBER_OF_PAGES 2

int difficulty = 500;
int snakeX = 15;
int snakeY = 15;
//int foodX = 15;
//int foodY = 15;
int velocity;
int posArray;
int posPage;
int score = 0;
/* Interrupt Service Routine */
void user_isr( void ){
  return;
}

/* Lab-specific initialization goes here */
void labinit( void ){

  volatile int *trise = (volatile int *) 0xbf886100;
  *trise = *trise & 0x00;
  TRISE = TRISE & 0x0fe0;
  TRISF = TRISF & 0x2;

  return;
}

int get_posSnake(){
  int i;
  for(i = 0; i < SIZE_OF_PAGE; i++){
    if(display[i] != 0)
      posArray = i;
    }
  return posArray;
}

void set_pixel(int snakeX, int snakeY){
  int i = snakeY / PIXEL_UNIT;
  display[snakeX + i*32] =  1 << (snakeY - i * PIXEL_UNIT);
}

void move_constant(){
  if(velocity = -1)
    snakeX--;

  if(velocity = 1)
    snakeX++;

  if(velocity = -32)
    snakeY--;

  if(velocity = 32)
    snakeY++;
}

void snake_clear(){
  int j;
  for(j = 0; j < SIZE_OF_PAGE; j++){
    if(j != posArray)
      display[j] = 0;
  }
}

void move_Left(){
  velocity = -1;
  snakeX--;
  posArray += velocity;
}

void move_Right(){
  velocity = 1;
  snakeX++;
  posArray += velocity;
}

void move_Up(){
  velocity = -32;
  snakeY--;
  posArray += velocity;
}

void move_Down(){
  velocity = 32;
  snakeY++;
  posArray += velocity;
}

void set_difficulty(){
  int sw;
  if(sw = getsw()){
    if(getsw() == 0x3)
      difficulty = 50;

    if(getsw() == 0x2)
      difficulty = 100;

    if(getsw() == 0x1)
      difficulty = 500;
  }
}

void buttons(){
  int but;
  if(but = getbtns()){
    if(but & 4){
      move_Left();
    }
    if(but & 2){
      move_Right();
    }
    if(but & 1){
      move_Down();
    }
  }

  if(but = btn1()){
    move_Up();


  }
}

void check_page(){
  switch (snakeX/WIDTH){
    case 0:
      posPage = 0;
      break;

    case 1:
      posPage = WIDTH;
      snakeY -= WIDTH;
      break;

    case 2:
      posPage = WIDTH*2;
      snakeY -= WIDTH;
      break;

    case 3:
      posPage = WIDTH*3;
      snakeY -= WIDTH;
      break;
  }
}

void display_clear(int amount){
  int i;
  for(i = 0; i < amount*WIDTH; i += 32){
    display_image(i, death);
  }
}

void game_over(){
  if(snakeX < 0 || snakeX > 31 || snakeY < 0 || snakeY > 31){
    int i;
    int but;
    for(i = 0; i < WIDTH*NUMBER_OF_PAGES; i += WIDTH){
      display_image(i, death);
    }
    while(1){
      display_update();
      display_string(0, "GAME OVER!");
      display_string(2, "TO RESTART:");
      display_string(3, "PRESS BTN1");

      if(but = btn1()){
        snakeX = 15;
        snakeY = 15;
        display_clear(NUMBER_OF_PAGES*2);
        break;
      }
    }
  }
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int *porte = (volatile int *) 0xbf886110;
  *porte += 0x01;

  set_difficulty();
  delay(difficulty);
  game_over();
  check_page();
  buttons();
  display_clear(NUMBER_OF_PAGES);
  snake_clear();
  move_constant();
  get_posSnake();
  set_pixel(snakeX, snakeY);
  //food();
  display_image(posPage,display);
  //move_snakeConstant();
  //display_image(posPage,display);
  //move_nextPage();
  //display_string(0,score);
}
