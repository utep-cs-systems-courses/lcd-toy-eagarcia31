#include <msp430.h>
#include "libTimer.h"
#include "stateMachine.h"

void __interrupt_vec(WDT_VECTOR) WDT(){
  const int secondLimit = 250;
  static int variableLimit = 5;
  static int blinkCount = 0;
  static int secondCount = 0;

  blinkCount++;
  if(blinkCount >= variableLimit) {
    toggle_green();
    blinkCount=0;
  }
}
