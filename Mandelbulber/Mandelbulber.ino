// Mandelbulber hypercomplex fractal //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define USER_BTN  PI11
#define WIDTH   LTDC_F746_ROKOTECH.width
#define HEIGHT  LTDC_F746_ROKOTECH.height
#define SCR     WIDTH*HEIGHT

uint16_t RGB565(byte R, byte G, byte B){ return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );}

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);} 

  float power = 5.0f;
  float xmin = -1.25f;
  float ymin = -1.25f;
  float zmin = -1.0f;
  float wh = 2.5f;
  float addx = wh/HEIGHT;
  float addy = wh/HEIGHT;
  float x = xmin, y;
  int i, j;

void rndrule() {

  tft.fillScreen(LTDC_BLACK);

  i = 0;
  j = 0;
  x = xmin;

  power = randomf(1.5f, 8.0f);
 
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

  x = xmin;

  for (i = 0; i < HEIGHT; i++) {

    if (digitalRead(USER_BTN)) rndrule();

    y = ymin;

    for (j = 0; j < HEIGHT; j++) {
      
      float z = zmin;
      
      for (int k = 255; k > 0 && z < -1.0f*zmin; k--) {
        
        float dr = 1;
        float nx = x;
        float ny = y;
        float nz = z;
        float rad = sqrtf(x*x+y*y+z*z);
        int n = 0;
        
        while (n < 3) {
          
          float powRad = powf(rad, power);
          float theta = atan2f(sqrtf(nx*nx+ny*ny), nz)*power;
          float phi = atan2f(ny, nx)*power;
          nx = sinf(theta)*cosf(phi)*powRad+x;
          ny = sinf(theta)*sinf(phi)*powRad+y;
          nz = cosf(theta)*powRad+z;
          dr = dr*power*powf(rad, power - 1.0f);
          rad = sqrtf(nx*nx+ny*ny+nz*nz);
          if (rad > 4.0f) break;
          n++;            
        }
        
        if (0.5f*logf(rad)*rad/dr < 0.00001f) {        
          buffer[(104+i)+j*WIDTH] = RGB565(k<<1,k,k);
          break;       
        } else {
          k--;
          z += 0.5f*logf(rad)*rad/dr;  
        }   
      }   
      y += addy;
    }   
    x += addx;
  }

}
