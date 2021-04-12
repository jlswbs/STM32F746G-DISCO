// Structure cellular automata //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define USER_BTN  PI11 
#define WIDTH LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height

#define ITER 16

  uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

  uint8_t state[WIDTH];
  uint16_t coll[ITER];
  uint8_t k;


void rndrule(){

  tft.fillScreen(LTDC_BLACK);

  for (int i=0; i<WIDTH; i++) state[i] = rand()%ITER;
  for (int i=0; i<ITER; i++) coll[i] = rand();
  k = rand()%ITER;

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

  for (int y = 0; y < HEIGHT; y++) {
  
    for (int x = 0; x < WIDTH; x++) {
      
      k = k ^ state[x] ^ state[k];
      state[x] = k;
      buffer[x+y*WIDTH] = coll[k];
      
    }
    
  }
    
}
