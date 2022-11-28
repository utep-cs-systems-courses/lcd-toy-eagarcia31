#include <msp430.h>
#include "libTimer.h"
#include "stateMachine.h"
#include "buzzer.h"
#include "switches.h"
#include "led.h"
#include "lcdutils.h"
#include "lcddraw.h"
//#include "shape.h"
//#include "abCircle.h"

unsigned int bgColor = COLOR_BLUE;
int redrawScreen = 1;

int main(void){
  configureClocks();
  switch_init();
  led_init();
  enableWDTInterrupts();
  buzzer_init();

  lcd_init();
  drawStrings5x7(10,10,"Bounce Sim", COLOR_YELLOW, COLOR_BLACK);
  drawStrings5x7(10,150, "You are the Bar", COLOR_YELLOW, COLOR_BLACK);

  shapeInit();
  layerGetBounds(&arenaLayer, &fieldfence);
  
  or_sr(0x18);

  for(;;){
    redrawScreen=0;
    movLayerDraw(&m10,&layer0);
  }
}
