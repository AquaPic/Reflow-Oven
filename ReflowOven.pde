#include <UTFT.h>
#include <UTouch.h>
#include <BUI.h>
#include <StringUtils.h>
#include <ReflowOven.h>
#include <drv_pid.h>
#include <MAX31855.h>

/*** setup ***/
void setup () {
  //Serial.begin (9600);
  oven.init ();
  attachCoreTimerService (TimerServiceCallback);
  //Serial.println ("Finished Setup");
}

/*** main ***/
void loop () {
  oven.loop ();
}

uint32_t TimerServiceCallback(uint32_t currentTime) {
  return oven.timerService (currentTime);
}
