// Multi layer Chua chaotic attractor //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define WIDTH   LTDC_F746_ROKOTECH.width
#define HEIGHT  LTDC_F746_ROKOTECH.height
#define SCR     WIDTH*HEIGHT

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

#define ITER 16384

  float x = 0.1f;
  float y = -0.2f;
  float z = 0.3f;
  float fx = 0.0f;
  float dt = 0.005f;
  float c[6];
  float m[6];
  float alpha = 9.0f;
  float beta = 14.286f;
  

void setup(){

  __HAL_RCC_RNG_CLK_ENABLE();
  RNG_HandleTypeDef handle = {0};
  handle.Instance = RNG;
  HAL_RNG_Init(&handle);

  uint32_t trng = HAL_RNG_GetRandomNumber(&handle);

  srand(trng);

  tft.begin((uint16_t *)buffer);

  tft.fillScreen(LTDC_BLACK);

  m[0] = -0.14285714285f;
  m[1] = 0.28571428571f; 
  m[2] = -0.57142857142f;
  m[3] = 0.28571428571f;
  m[4] = -0.57142857142f;
  m[5] = 0.28571428571f;

  c[0] = 0.0f;
  c[1] = 1.0f;
  c[2] = 2.15f;
  c[3] = 3.6f;
  c[4] = 8.2f;
  c[5] = 13.0f;
  
}


void loop(){
  
  memset(buffer, 0, 2*SCR);

  for (int i=0; i<ITER; i++){

    float nx = x;
    float ny = y;
    float nz = z;
      
    float sum = 0.0f;
        
    for (int k=1; k<6; k++) sum = sum + ((m[k-1] - m[k]) * (fabs(nx + c[k])-fabs(nx - c[k])));

    fx = m[5] * nx + 0.5f * sum;
        
    x = nx + dt * (alpha * (ny - fx));
    y = ny + dt * (nx - ny + nz);
    z = nz + dt * (-beta * ny);

    int ax = (WIDTH/2) + (7*x);
    int ay = (HEIGHT/2) + (21*y);

    uint8_t coll = i<<4;
    
    if (ax>0 && ax<WIDTH && ay>0 && ay <HEIGHT) tft.drawRect(ax, ay, 2, 2, tft.color565(coll,coll,coll));

  }
  
}
