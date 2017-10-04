
#include "bbDrive.h"
#include "arduino.h"

#define RIGHT_ANALOG 3
#define RIGHT_DIRECTION 2
#define RIGHT_ANALOG_INV true
#define RIGHT_DIRECTION_INV false


#define LEFT_ANALOG 5
#define LEFT_DIRECTION 4
#define LEFT_ANALOG_INV true
#define LEFT_DIRECTION_INV true

bbDrive::bbDrive()
{
  pinMode(LEFT_DIRECTION,OUTPUT);
  pinMode(LEFT_ANALOG,OUTPUT);
  pinMode(RIGHT_DIRECTION,OUTPUT);
  pinMode(RIGHT_ANALOG,OUTPUT);
}

void bbDrive::set(int forward,int turn)
{
  int left = forward + turn;
  int right = forward - turn;

  if(left > 255)
  {
    left = 255;
  }
  else if (left < -255)
  {
    left = -255;
  }

  if(right > 255)
  {
    right = 255;
  }
  else if(right < -255)
  {
    right = -255;
  }

  if(LEFT_DIRECTION_INV)
  {
    left = -left;
  }
  if(RIGHT_DIRECTION_INV)
  {
    left = -left;
  }

  digitalWrite(LEFT_DIRECTION, left > 0);
  digitalWrite(RIGHT_DIRECTION, right>0);

  if(LEFT_ANALOG_INV)
  {
    left = -left;
  }
  if(RIGHT_ANALOG_INV)
  {
    right = -right;
  }

  if(left < 0)
  {
    left = 255 + left;
  }

  if(right < 0)
  {
    right = 255 + right;
  }

  analogWrite(LEFT_ANALOG,left);
  analogWrite(RIGHT_ANALOG,right);
  
  
}

