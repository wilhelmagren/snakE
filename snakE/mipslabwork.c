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
#include <stdlib.h>   /*includes standardlibrary to use function rand()*/
#include "standard.h" /*includes the use of functions rand()*/

#define HEIGHT 4
#define WIDTH 32
#define SIZE_OF_PAGE HEIGHT*WIDTH
#define PIXEL_UNIT 8
#define NUMBER_OF_PAGES 2

//srand();
volatile int *porte = (volatile int *) 0xbf886110;
char scoreBuffer[] = "YOUR SCORE: ";
int difficulty = 500;
int snakeX = 15;
int snakeY = 15;
int foodX = 20;
int foodY = 20;
int posFood;
int velocity = 0;
int posArray;
int posPage;
int score = 254;
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
  int i = snakeY / PIXEL_UNIT;
  return posArray = snakeX + i*32;
}

int get_posFood(){
  int i = foodY / PIXEL_UNIT;
  return posFood = foodX + i*32;
}

void set_pixel(int snakeX, int snakeY){
  int i = snakeY / PIXEL_UNIT;
  display[snakeX + i*32] =  1 << (snakeY - i * PIXEL_UNIT);
}

void update(){
  int j;
  for(j = 0; j < SIZE_OF_PAGE; j++){
    if(j != posArray && j != posFood)
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
      difficulty = 200;
  }
}

void buttons(){
  int but;
  if(but = getbtns()){
    if(but & 4){
      velocity = -1;
      move_Left();
    }
    if(but & 2){
      velocity = 32;
      move_Down();
    }
    if(but & 1){
      velocity = -32;
      move_Up();
    }
  }

  if(but = btn1()){
    velocity = 1;
    move_Right();
  }
}

void food(){
  set_pixel(foodX,foodY);
  *porte += 0x01;
  score++;
}

void check_collide(){
  if(snakeX == foodX && snakeY == foodY){
    foodX = rand() % 31;
    foodY = rand() % 31;
    food();
  }
}

void move_Constant(){
  if(velocity == 1)
    snakeX++;

  if(velocity == -1)
    snakeX--;

  if(velocity == 32)
    snakeY++;

  if(velocity == -32)
    snakeY--;
}

void display_clear(int amount, uint8_t screen[]){
  int i;
  for(i = 0; i < amount*WIDTH; i += 32){
    display_image(i, screen);
  }
}

void reset(){
  snakeX = 15;
  snakeY = 15;
  foodX = rand() % 31;
  foodY = rand() % 31;
  velocity = 0;
  *porte = 0x0;
  score = 0;
  display_clear(NUMBER_OF_PAGES*2,death);
}

void game_win(){
  if(score > 255){
    *porte = 0xff;
    int but;
    while(1){
      display_update();
      display_string(0, "YOU WON!");
      display_string(1, "SCORE ON LIGHTS");
      display_string(2, "TO RESTART");
      display_string(3, "PRESS BUTTON 1");
      *porte += 0xA;

      if(but = btn1()){
        reset();
        break;
      }
    }
  }
}

void game_over(){
  if(snakeX < 0 || snakeX > 31 || snakeY < 0 || snakeY > 31){
    int i;
    int but;
    for(i = 0; i < WIDTH*NUMBER_OF_PAGES; i += WIDTH){
      display_image(i, clear);
    }
    while(1){
      display_update();
      display_string(0, "GAME OVER!");
      display_string(1, scoreBuffer);
      display_string(2, "TO RESTART");
      display_string(3, "PRESS BUTTON 1");

      if(but = btn1()){
        reset();
        break;
      }
    }
  }
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  set_difficulty();
  delay(difficulty);
  game_over();
  game_win();
  buttons();
  display_clear(NUMBER_OF_PAGES/2,clear);
  move_Constant();
  get_posSnake();
  get_posFood();
  check_collide();
  set_pixel(foodX, foodY);
  set_pixel(snakeX, snakeY);
  update();
  display_image(posPage,display);
}
