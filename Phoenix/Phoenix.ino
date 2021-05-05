// Phoenix fractal //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define WIDTH   LTDC_F746_ROKOTECH.width
#define HEIGHT  LTDC_F746_ROKOTECH.height
#define SCR     WIDTH*HEIGHT

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

float mapf(float val, float in_min, float in_max, float out_min, float out_max) {return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}

#define ITER 100

  float p = 0.5667f;
  float q = -0.5f;

float multa(float a, float b, float c, float d) { return (a*c - b*d); }
float multb(float a, float b, float c, float d) { return (a*d + b*c); }

int henon(float a, float b) {
  
  float pzr = 0;
  float pzi = 0;
  float zr = a;
  float zi = b;
  int i = 0;
  float ph1[2], ph2[2];
  
  while (i < ITER && sqrtf(zr*zr + zi*zi) < 4) {
    ph1[0] = multa(zr, zi, zr, zi);
    ph1[1] = multb(zr, zi, zr, zi);
    ph2[0] = multa(pzr, pzi, q, 0);
    ph2[1] = multb(pzr, pzi, q, 0);
    
    pzr = zr;
    pzi = zi;
    
    zr = ph1[0] + p + ph2[0];
    zi = ph1[1] + ph2[1];
    
    i++;
  }
  
  return map(i, 0, ITER, 65535, 0);

}

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
  
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
     
      uint16_t coll = henon(mapf(x, 0, WIDTH, -2.0f, 2.0f), mapf(y, 0, HEIGHT, -2.0f, 2.0f));   
      buffer[x+y*WIDTH] = coll;
      
    }
  }
  
}
