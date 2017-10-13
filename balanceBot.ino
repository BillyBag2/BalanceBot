// TODO:
// Detect if RX disconnected. IE no serial activity for a while.
//

#define USE_SBUS 0
#define BALANCE_POINT -7.0  /* -5 */
#define MAX_ANGLE 45.0
#define CUT_ANGLE 45.0

#define PID_P -2.9        /* -3*/
#define PID_D -0.000012    /* -0.00002*/
#define PID_I -0.00001    /* -0.00001*/

#include "bbDrive.h"
#include "SBUS.h"
#include "MPU6050_DMP.h"

float gBalancePoint = BALANCE_POINT;
float gSpeed = 0.0;
float gTargetSpeed = 0.0;

bbDrive drive;
#if USE_SBUS
SBUS sbus(Serial);
#endif

void setup() {
#if USE_SBUS
  sbus.begin(false);
#else
  Serial.begin(115200);
#endif
  GyroSetup();
}

float saturate(float a,float range)
{
  if(a > range)
  {
    a = range;
  }
  if(a < -range)
  {
    a = -range;
  }
  return a;
}

void loop() {
  int32_t gyroXyz[3];
  float * ypr = GyroGet(gyroXyz);

  
#if USE_SBUS
  sbus.process();
  
  int forwardx4 = sbus.getChannel(2) ;
  int forward = forwardx4/4;  
  forward = forward - 0x100;
  bool gear = sbus.getChannel(5) > 0x400;
  
  if((sbus.getFailsafeStatus() == SBUS_FAILSAFE_INACTIVE) && (sbus.getGoodFrames() != 0))
  {
    if(gear)
    {
      drive.set(0,0);
    }
    else
    {
      drive.set(forward,0);
    }
  }
  else
  {
    drive.set(0,0);
  }
#else
  if(ypr != NULL)
  {
    float pitch = (ypr[1] * 180/M_PI) + gBalancePoint; /* 10 - 14 */
    if((pitch > CUT_ANGLE) || (pitch < -CUT_ANGLE))
    {
      drive.set(0,0);
      gSpeed = 0;
      gBalancePoint = ((gBalancePoint - BALANCE_POINT) * 0.1) + BALANCE_POINT;
    }
    else
    {
      float correction = (pitch * PID_P) /* - (gyroXyz[1] * PID_D) */; 
      gSpeed = gSpeed + correction - (gyroXyz[1] * PID_D);
      gSpeed = saturate(gSpeed, 255.0);
      drive.set((int)gSpeed,0);

gTargetSpeed = 0.0;
      
      gBalancePoint += (gSpeed - gTargetSpeed) * PID_I;
      gBalancePoint = saturate(gBalancePoint, MAX_ANGLE);
    }
  }
#endif
}
