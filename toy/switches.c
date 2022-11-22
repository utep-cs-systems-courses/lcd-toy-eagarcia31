#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "stateMachine.h"

char S1_down, S2_down, S3_down, S4_down;
char switch_state_down, switch_state_changed;

static char switch_update_interrupt_sense(){
  char p1val = P2IN;
  P2IES |= (p1val & SWITCHES);
  P2IES &= (p1val | ~SWITCHES);
  return p1val;
}

void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_update_interrupt_sense();
  led_update();
}

void switch_interrupt_handler(){
  char p1val = switch_update_interrupt_sense();
  S1_down = (p1val & S1) ? 0 : 1;
  S2_down = (p1val & S2) ? 0 : 1;
  S3_down = (p1val & S3) ? 0 : 1;
  S4_down = (p1val & S4) ? 0 : 1;
  switch_state_changed = 1;
  led_update();

  if(S1_down){
    buzzer_set_period(1000);
  }
  if(S2_down){
    buzzer_set_period(500);
  }
  if(S3_down){
    buzzer_set_period(800);
  }
  if(S4_down){
    toggle_green();
  }
}
