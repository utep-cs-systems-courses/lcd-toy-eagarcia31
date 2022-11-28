#include <msp430.h>
#include "libTimer.h"
#include "stateMachine.h"
#include "buzzer.h"
#include "switches.h"
#include "led.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"
#include "abCircle.h"

AbRectOutline fieldOutline = {abRectOutlineGetBounds, abRectOutlineCheck, {screenWidth/2-10,screenHeight/2-10}};

AbRect bar105 = {abRectGetBounds, abRectCheck,{10,5}};

Layer arenaLayer = {(AbShape*)&fieldOutline,{screenWidth/2,screenHeight/2},{0,0},{0,0},COLOR_BLACK,0};
Layer layer1 = {(AbShape*)&circle8,{screenWidth/2,screenHeight/2},{0,0},{0,0},COLOR_GREEN,&arenaLayer,};
Layer layer0 = {(AbShape*)&bar105,{screenWidth/2,screenHeight/2+25},{0,0},{0,0},COLOR_RED,&layer1,};

unsigned int bgColor = COLOR_BLUE;
int redrawScreen = 1;
Region fieldfence;

typedef struct MovLayer_s{
  Layer*layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

MovLayer m11 = {&layer1, {2,1}, 0};
MovLayer m10 = {&layer0, {0,2}, &m11};

void movLayerDraw(MovLayer *movLayers, Layer *layers){
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);
  for(movLayer = movLayers; movLayer; movLayer = movLayer->next){
    Layer *l = movLayer->layer;
    l->posLast=l->pos;
    l->pos=l->posNext;
  }
  or_sr(8);

  for(movLayer = movLayers; movLayer; movLayer=movLayer->next){
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for(row = bounds.topLeft.axes[1]; row<=bounds.botRight.axes[1]; row++){
      for(col = bounds.topLeft.axes[0]; col<=bounds.botRight.axes[0]; col++){
	Vec2 pixelPos = {col, row};
	unsigned int color = bgColor;
	Layer *probeLayer;
	for(probeLayer = layers; probeLayer; probeLayer = probeLayer->next){
	  if(abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)){
	    color = probeLayer->color;
	    break;
	  }
	}
	lcd_writeColor(color);
      }
    }
  }
}

int main(void){
  configureClocks();
  switch_init();
  led_init();
  enableWDTInterrupts();
  buzzer_init();

  lcd_init();
  layerInit(&layer0);
  layerDraw(&layer0);
  
  drawString5x7(10,10,"Bounce Sim", COLOR_YELLOW, COLOR_BLACK);
  drawString5x7(10,150, "You are the Bar", COLOR_YELLOW, COLOR_BLACK);

  shapeInit();
  layerGetBounds(&arenaLayer, &fieldfence);
  
  or_sr(0x18);

  for(;;){
    redrawScreen=0;
    movLayerDraw(&m10,&layer0);
  }
}
