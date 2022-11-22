#include <msp430.h>
#include "led.h"
#include "switches.h"
#include "stateMachine.h"

unsigned char red_on=0, green_on=0;
static char redVal[]={0,LED_RED}, greenVal[]={0,LED_GREEN);

void led_init(){
  P1DIR |= LEDS;
  switch_state_changed = 1;
  led_update();
}

void led_update(){
  if(switch_state_changed){
    char ledFlags = redVal[red_on] | greenVal[green_on];

    ledFlags |= switch_state_down ? LED_GREEN : 0;

    P1OUT &= (0xff - LEDS) | ledFlags;
    P1OUT |= ledFlags;
  }
  switch_state_changed = 0;
}
