// Hopalong orbit fractal //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);} 

  #define USER_BTN  PI11
  #define WIDTH     480
  #define HEIGHT    272
  #define SCR       WIDTH*HEIGHT
  #define ITER      10000

  float a, b, c, x, y, t;

void rndrule() {

  tft.fillScreen(LTDC_BLACK);

  x = 0.1f;
  y = 0.1f;
  
  float logpmax = logf(WIDTH/4);
  
  a = expf(randomf(0.0f, 1.0f)*logpmax);
  b = expf(randomf(0.0f, 1.0f)*logpmax);
  c = expf(randomf(0.0f, 1.0f)*logpmax);
 
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

    t = sqrtf(fabs(b*nx-c));
    x = ny - ((nx<0) ? t : -t);
    y = a - nx;

    int ax = (WIDTH/2) + (x/3);
    int ay = (HEIGHT/2) + (y/3);
      
    if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) tft.drawPixel(ax,ay, coll);

  }

}
