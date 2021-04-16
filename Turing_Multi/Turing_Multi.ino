// Multi-Scale Turing patterns //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

float randomf(float minf, float maxf) {return minf + (rand()%(1UL << 31))*(maxf - minf) / (1UL << 31);} 

#define USER_BTN  PI11
#define WIDTH 240
#define HEIGHT 136
#define SCR (WIDTH * HEIGHT)

uint32_t size = 4*SCR;

  float sinus[7] = { 0, sinf(TWO_PI/1.0f), sinf(TWO_PI/2.0f),  sinf(TWO_PI/3.0f),  sinf(TWO_PI/4.0f),  sinf(TWO_PI/5.0f),  sinf(TWO_PI/6.0f) };
  float cosinus[7] = { 0, cosf(TWO_PI/1.0f), cosf(TWO_PI/2.0f),  cosf(TWO_PI/3.0f),  cosf(TWO_PI/4.0f),  cosf(TWO_PI/5.0f),  cosf(TWO_PI/6.0f) };

  int radius;
  int blurlevels, symmetry;
  float base;
  int levels;
  float stepScale;
  float stepOffset;
  float blurFactor;

  int8_t *radii = (int8_t*)malloc(size);
  float *stepSizes = (float*)malloc(size); 
  float *grid = (float*)malloc(size); 
  float *blurBuffer = (float*)malloc(size); 
  float *variation = (float*)malloc(size); 
  float *bestVariation = (float*)malloc(size);
  int8_t *bestLevel = (int8_t*)malloc(size);
  bool *direction = (bool*)malloc(size);
  float *activator = (float*)malloc(size);
  float *inhibitor = (float*)malloc(size);
  float *swap = (float*)malloc(size);

int getSymmetry(int i, int w, int h) {
  if(symmetry <= 1) return i;
  if(symmetry == 2) return SCR - 1 - i;
  int x1 = i % w;
  int y1 = i / w;
  float dx = x1 - w/2.0f;
  float dy = y1 - h/2.0f;
  int x2 = w/2 + (int)(dx * cosinus[symmetry] + dy * sinus[symmetry]);
  int y2 = h/2 + (int)(dx * -sinus[symmetry] + dy * cosinus[symmetry]);
  int j = x2 + y2 * w;
  return j<0 ? j+SCR : j>=SCR ? j-SCR : j;
}

void rndrule() {

  tft.fillScreen(LTDC_BLACK);

  symmetry = rand()%7;
  base = randomf(1.5f, 2.4f);
  stepScale = randomf(0.01f, 0.2f);
  stepOffset = randomf(0.01f, 0.4f);
  blurFactor = randomf(0.5f, 1.0f);

  levels = (int) (log(max(WIDTH,HEIGHT)) / logf(base)) - 1.0f;
  blurlevels = (int) max(0, (levels+1) * blurFactor - 0.5f);

  for (int i = 0; i < levels; i++) {
    int radius = (int)powf(base, i);
    radii[i] = radius;
    stepSizes[i] = logf(radius) * stepScale + stepOffset;
  }

  for (int i = 0; i < SCR; i++) grid[i] = randomf(-1.0f, 1.0f);
 
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

  if(symmetry >= 1) for(int i = 0; i < SCR; i++) grid[i] = grid[i] * 0.9f + grid[getSymmetry(i, WIDTH, HEIGHT)] * 0.1f;
   
  memcpy(activator, grid, 4*SCR);

  for (int level = 0; level < levels - 1; level++) {

    int radius = radii[level];

    if(level <= blurlevels){
        
      for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
          int t = y * WIDTH + x;
          if (y == 0 && x == 0) blurBuffer[t] = activator[t];
          else if (y == 0) blurBuffer[t] = blurBuffer[t - 1] + activator[t];
          else if (x == 0) blurBuffer[t] = blurBuffer[t - WIDTH] + activator[t];
          else blurBuffer[t] = blurBuffer[t - 1] + blurBuffer[t - WIDTH] - blurBuffer[t - WIDTH - 1] + activator[t];
        }
      }
    }
    
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        int minx = max(0, x - radius);
        int maxx = min(x + radius, WIDTH - 1);
        int miny = max(0, y - radius);
        int maxy = min(y + radius, HEIGHT - 1);
        int area = (maxx - minx) * (maxy - miny);

        int nw = miny * WIDTH + minx;
        int ne = miny * WIDTH + maxx;
        int sw = maxy * WIDTH + minx;
        int se = maxy * WIDTH + maxx;

        int t = y * WIDTH + x;
        inhibitor[t] = (blurBuffer[se] - blurBuffer[sw] - blurBuffer[ne] + blurBuffer[nw]) / area;
      }
    }
        
    for (int i = 0; i < SCR; i++) {
      float variation = fabs(activator[i] - inhibitor[i]);
      if (level == 0 || variation < bestVariation[i]) {
        bestVariation[i] = variation;
        bestLevel[i] = level;
        direction[i] = activator[i] > inhibitor[i];
      }
    }

    if(level==0) {
      memcpy(activator, inhibitor, 4*SCR);
    } else {
      memcpy(swap, activator, 4*SCR);
      memcpy(activator, inhibitor, 4*SCR);
      memcpy(inhibitor, swap, 4*SCR);
    }
            
  }

  float smallest = MAXFLOAT;
  float largest = -MAXFLOAT;

  for (int i = 0; i < SCR; i++) {
    float curStep = stepSizes[bestLevel[i]];
    if (direction[i])grid[i] += curStep;
    else grid[i] -= curStep;
    smallest = min(smallest, grid[i]);
    largest = max(largest, grid[i]);
  }

  float range = (largest - smallest) / 2.0f;
  
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        int t = y * WIDTH + x;
        grid[t] = ((grid[t] - smallest) / range) - 1.0f;
        uint8_t col = 128.0f + (128.0f * grid[t]);
        buffer[(2*x)+(2*y)*LTDC_F746_ROKOTECH.width] =  tft.color565(col, col, col);
      }   
    }
}
