// TODO:
// Detect if RX disconnected. IE no serial activity for a while.
//


#include "bbDrive.h"
#include "SBUS.h"

bbDrive drive;
SBUS sbus(Serial);

void setup() {
 sbus.begin(false);
}

int forwardx4_max = 0x600;
int forwardx4_min = 0x200;

void loop() {
  sbus.process();
  
  int forwardx4 = sbus.getChannel(2) ;
  int forward = (forwardx4 - forwardx4_min)/4;
  long scalex256  = ((long)(0x800) * (long)(0x100)) / (long)(forwardx4_max - forwardx4_min);
  forward = (int)((forward * scalex256)/0x100);
  forward = forward - 0x100;
  bool gear = sbus.getChannel(5) > 0x400;
  
  if((sbus.getFailsafeStatus() == SBUS_FAILSAFE_INACTIVE) && (sbus.getGoodFrames() != 0))
  {
    if(gear)
    {
      if(forwardx4 > forwardx4_max)
      {
        forwardx4_max = (forwardx4_max + forwardx4)/2;
      }
      else if(forwardx4 < forwardx4_min)
      {
        forwardx4 = (forwardx4_min + forwardx4)/2;
      }
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
}
