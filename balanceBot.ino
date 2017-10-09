// TODO:
// Detect if RX disconnected. IE no serial activity for a while.
//

#define USE_SBUS 0

#include "bbDrive.h"
#include "SBUS.h"
#include "MPU6050_DMP.h"


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
  float * ypr = GyroGet();
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
    float P = -30.0;
    float pitch = (ypr[1] * 180/M_PI) + 13; /* 10 - 14 */
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
      drive.set((int)(pitch * P),0);
    }
  }
#endif
}
