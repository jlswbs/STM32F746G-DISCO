// XOR patterns //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define WIDTH LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height

#define ITER 16

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

  int t;

void setup(){

  __HAL_RCC_RNG_CLK_ENABLE();
  RNG_HandleTypeDef handle = {0};
  handle.Instance = RNG;
  HAL_RNG_Init(&handle);

  uint32_t trng = HAL_RNG_GetRandomNumber(&handle);

  srand(trng);

  tft.begin((uint16_t *)buffer);  
  tft.fillScreen(LTDC_BLACK);
  
}


void loop(){

  for (int y = 0; y < HEIGHT; y++) {
  
    for (int x = 0; x < WIDTH; x++) {

      uint8_t coll = (rand()%32) ^ (x ^ (y+t)) * t;    
      buffer[x+y*WIDTH] = tft.color565(coll, coll, coll);
       
    }
    
    delayMicroseconds(200);
    
  }

  t++;
  
}
