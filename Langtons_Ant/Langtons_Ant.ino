// Langtons Ant //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define USER_BTN  PI11 
#define WIDTH  LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height
  
  #define NUMANTS 6
  #define ITER 25
  
  uint16_t coll[NUMANTS];
  
  int x[NUMANTS];
  int y[NUMANTS];
  int antsdir[NUMANTS];
  
  uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

 void rndrule(){
  
  tft.fillScreen(LTDC_BLACK);

  for(int i = 0; i < NUMANTS; i++){
  
    x[i] = rand()%WIDTH;
    y[i] = rand()%HEIGHT;
    antsdir[i] = rand()%4;
    coll[i] = rand();
    
  }

}
 
void setup(){

  __HAL_RCC_RNG_CLK_ENABLE();
  RNG_HandleTypeDef handle = {0};
  handle.Instance = RNG;
  HAL_RNG_Init(&handle);

  uint32_t trng = HAL_RNG_GetRandomNumber(&handle);

  srand(trng);

  pinMode(USER_BTN, INPUT);

  tft.begin((uint16_t *)buffer);
  
  rndrule();
  
}


void loop(){

  if (digitalRead(USER_BTN)) rndrule();
  
  for(int i = 0; i < NUMANTS; i++){
    
    if (buffer[x[i]+WIDTH*y[i]] > LTDC_BLACK){ antsdir[i] = antsdir[i] - 1; buffer[x[i]+WIDTH*y[i]] = LTDC_BLACK; }
    else { antsdir[i] = antsdir[i] + 1; buffer[x[i]+WIDTH*y[i]] = coll[i]; }

    if (antsdir[i] > 3) antsdir[i] = 0;   
    if (antsdir[i] < 0) antsdir[i] = 3;   
    
    if (antsdir[i] == 0) x[i] = x[i] - 1;
    if (antsdir[i] == 1) y[i] = y[i] + 1;
    if (antsdir[i] == 2) x[i] = x[i] + 1;
    if (antsdir[i] == 3) y[i] = y[i] - 1;
    
    if (x[i] > WIDTH-1) x[i] = 0;
    if (x[i] < 0) x[i] = WIDTH-1;
    if (y[i] > HEIGHT-1) y[i] = 0;
    if (y[i] < 0) y[i] = HEIGHT-1;

    delayMicroseconds(ITER);
    
  }
  
}
