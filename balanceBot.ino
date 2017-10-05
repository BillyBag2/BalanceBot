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

void loop() {
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
}
