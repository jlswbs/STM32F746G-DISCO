// Lorenz chaotic attractor //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define WIDTH   LTDC_F746_ROKOTECH.width
#define HEIGHT  LTDC_F746_ROKOTECH.height
#define SCR     WIDTH*HEIGHT

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

#define ITER 16384

  float x = 0.1f;
  float y = 0.1f;
  float z = 0.1f;
  float dt = 0.001f;
  float a = 10.0f;
  float b = 28.0f;
  float c = 8.0f/3.0f;
  

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
  
  memset(buffer, 0, 2*SCR);

  for (int i=0; i<ITER; i++){

    float nx = x;
    float ny = y;
    float nz = z;
        
    x = nx + dt * (a * (ny - nx));
    y = ny + dt * (nx * (b - nz) - ny);
    z = nz + dt * (nx * ny - c * nz);

    int ax = (WIDTH/2) + (11*x);
    int ay = (HEIGHT) - (5*z);

    uint8_t coll = i<<4;
    
    if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) tft.drawRect(ax, ay, 2, 2, tft.color565(coll,coll,coll));

  }
  
}
