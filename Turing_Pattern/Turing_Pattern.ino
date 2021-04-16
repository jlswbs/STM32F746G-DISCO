// Turing patterns //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));
uint32_t size = 1024 * 1024 - 8;
float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);} 

#define USER_BTN  PI11 
#define WIDTH 160
#define HEIGHT 90
#define SCR HEIGHT*WIDTH
#define SCL 4

  int lim = 128;
  int dirs = 9;
  int patt = 0;

  float *pat = (float*)malloc(size);
  float *pnew = (float*)malloc(size);
  float *pmedian = (float*)malloc(size);
  float *prange = (float*)malloc(size);
  float *pvar = (float*)malloc(size);


void rndrule(){

  tft.fillScreen(LTDC_BLACK);
  
  for(int i=0; i<SCR; i++) pat[i] = randomf(0.0f ,255.0f);

  lim = 96 + rand()%96;
  dirs = 6 + rand()%7;
  patt = rand()%3;
  
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

  float R = randomf(0,TWO_PI);

  memcpy(pnew, pat, 4*SCR);
  memset(pmedian, 0, 4*(SCR*SCL));
  memset(prange, 0, 4*(SCR*SCL));
  memset(pvar, 0, 4*(SCR*SCL));
 
  for(int i=0; i<SCL; i++) {
      
    float d = (2<<i);

    for(int j=0; j<dirs; j++) {
        
      float dir = j*TWO_PI/dirs + R;
      int dx = (d * cosf(dir));
      int dy = (d * sinf(dir));
      
          for(int l=0; l<SCR; l++) { 
        
            int x1 = l + dx, y1 = l/WIDTH + dy;

            if(x1<0) x1 = WIDTH-1-(-x1-1); if(x1>=WIDTH) x1 = x1%WIDTH;
            if(y1<0) y1 = HEIGHT-1-(-y1-1); if(y1>=HEIGHT) y1 = y1%HEIGHT;

            pmedian[l+SCR*i] += pat[x1+y1*WIDTH] / dirs;
         
          }
        }

      for(int j=0; j<dirs; j++) {
      
        float dir = j*TWO_PI/dirs + R;
        int dx = (d * cosf(dir));
        int dy = (d * sinf(dir));
        
          for(int l=0; l<SCR; l++) { 

            int x1 = l + dx, y1 = l/WIDTH + dy;

            if(x1<0) x1 = WIDTH-1-(-x1-1); if(x1>=WIDTH) x1 = x1%WIDTH;
            if(y1<0) y1 = HEIGHT-1-(-y1-1); if(y1>=HEIGHT) y1 = y1%HEIGHT;

            pvar[l+SCR*i] += fabs( pat[x1+y1*WIDTH] - pmedian[l+SCR*i] ) / dirs;
            prange[l+SCR*i] += pat[x1+y1*WIDTH] > (lim + i*10) ? +1.0f : -1.0f;   
    
          }
        }    
      }
 
      for(int l=0; l<SCR; l++) { 
     
        int imin=0, imax=SCL;
        float vmin = MAXFLOAT;
        float vmax = -MAXFLOAT;
        
        for(int i=0; i<SCL; i++) {
          if (pvar[l+SCR*i] <= vmin) { vmin = pvar[l+SCR*i]; imin = i; }
          if (pvar[l+SCR*i] >= vmax) { vmax = pvar[l+SCR*i]; imax = i; }
        }
        
        switch(patt){
          case 0: for(int i=0; i<=imin; i++)    pnew[l] += prange[l+SCR*i]; break;
          case 1: for(int i=imin; i<=imax; i++) pnew[l] += prange[l+SCR*i]; break;
          case 2: for(int i=imin; i<=imax; i++) pnew[l] += prange[l+SCR*i] + pvar[l+SCR*i] / 2.0f; break;
        } 
      }
 
      float vmin = MAXFLOAT;
      float vmax = -MAXFLOAT;
  
      for(int i=0; i<SCR; i++) {
    
        vmin = min(vmin, pnew[i]);
        vmax = max(vmax, pnew[i]);
        
      }      
  
      float dv = vmax - vmin;
   
      for(int x=0; x<WIDTH; x++)
    
        for(int y=0; y<HEIGHT; y++) {

          pat[x+y*WIDTH] = (pnew[x+y*WIDTH] - vmin) * 255.0f/dv;
          uint8_t col = pat[x+y*WIDTH];
          buffer[(3*x)+(3*y)*LTDC_F746_ROKOTECH.width] = tft.color565(col, col, col);

      }

}
