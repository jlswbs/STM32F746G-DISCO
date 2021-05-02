// Mandelbrot fractal zoomer //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define WIDTH   LTDC_F746_ROKOTECH.width
#define HEIGHT  LTDC_F746_ROKOTECH.height
#define SCR     WIDTH*HEIGHT

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

  float x = -0.085f;
  float y = 0.85f;
  float zoom = 1.0f;
  

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
  
  mandelbrot(-3.5f*zoom+x, -2.0f*zoom+y, 3.5f*zoom+x, 2.0f*zoom+y);
  
  zoom *= 0.7f; 
  if(zoom <= 0.00001f) zoom = 1.0f;
  
}


void mandelbrot(float x1, float y1, float x2, float y2) {

  float sy = y2 - y1;
  float sx = x2 - x1;
  const int MAX = 256;
  int iter;

  for(int i = 0; i < WIDTH; i++) {
    for(int j = 0; j < HEIGHT; j++) {
      float cy = j * sy / HEIGHT + y1;
      float cx = i * sx / WIDTH + x1;
      float x=0.0f, y=0.0f, xx=0.0f, yy=0.0f;
      for(iter=0; iter <= MAX && xx+yy<4.0f; iter++) {
        xx = x*x;
        yy = y*y;
        y = 2.0f*x*y + cy;
        x = xx - yy + cx;
      }
      uint16_t coll = ((iter << 8) | (iter&0xFF));
      buffer[i+j*WIDTH] = coll;
    }
  }
}
