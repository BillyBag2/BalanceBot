// TODO:
// Detect if RX disconnected. IE no serial activity for a while.
//

#define USE_SBUS 0
#define BALANCE_POINT -5.0  /* -10 -5 */

#if 1 /*coorection ^ 1 */
#define PID_P -45.0 /* -45.0 */
#define PID_D -0.00001 /* -0.000015 ? */
#define PID_I -0.0001 /* Sudo I -0.0003 */
#endif

#if 0 /*coorection ^ 3 */
#define PID_P -2 /* -45.0 */
#define PID_D -0.000001 /* -0.000015 ? */
#define PID_I -0.0001 /* Sudo I -0.0003 */
#endif


#include "bbDrive.h"
#include "SBUS.h"
#include "MPU6050_DMP.h"

float balancePoint = BALANCE_POINT;

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
  //drive.set(0,0);
  if(ypr != NULL)
  {
    float pitch = (ypr[1] * 180/M_PI) + balancePoint; /* 10 - 14 */
    //Serial.print("YPR ");
    //Serial.print(ypr[0] * 180/M_PI);Serial.print("\t");
    //Serial.print(ypr[1] * 180/M_PI);Serial.print("\t");
    //Serial.println(ypr[2] * 180/M_PI);
    if((pitch > 45) || (pitch < -45))
    {
      drive.set(0,0);
    }
    else
    {
      float correction = (pitch * PID_P) - (gyroXyz[1] * PID_D);
      drive.set((int)correction,0);
      if(correction > 10.0)
      {
        correction = 10.0;
      }
      if(correction < -10.0)
      {
        correction = -10.0;
      }
      balancePoint += correction * PID_I;
      if(balancePoint > 20.0)
      {
        balancePoint = 20.0;
      }
      if(balancePoint < -20.0)
      {
        balancePoint = -20.0;
      }
    }
  }
#endif
}
