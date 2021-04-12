// Web map - four fold symmetry //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define USER_BTN  PI11 
#define WIDTH LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height

#define ITER 1500

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);}
uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

  float x = 0.1f;
  float y = 0.1f;
  float k = 4.6692f;

void rndrule(){

  tft.fillScreen(LTDC_BLACK);

  x = 0.1f;
  y = 0.1f;
  k = randomf(2.999f, 4.999f);

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

  uint16_t coll = rand();

  for (int i=0; i<ITER; i++) {

    float nx = x;
    float ny = y;

    x = ny;
    y = - nx - k * sinf(ny);

    uint16_t xx = 240+(x/20);
    uint16_t yy = 136+(y/20);

    if(xx > 0 && xx < WIDTH && yy > 0 && yy < HEIGHT) buffer[xx+yy*WIDTH] = coll;

 }

}
