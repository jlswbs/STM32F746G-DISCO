// Conway's Game of Life //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));
uint32_t size = 1024 * 1024 - 8;

#define USER_BTN  PI11 
#define WIDTH  240
#define HEIGHT 136

  bool *grid = (bool*)malloc(size);
  bool *newgrid = (bool*)malloc(size);


void rndrule(){

  tft.fillScreen(LTDC_BLACK);
  
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
        newgrid[x+y*WIDTH] = false;
        if (rand()%4 == 1) grid[x+y*WIDTH] = true;
        else grid[x+y*WIDTH] = false;
      }
    }
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

  uint16_t color;

  computeCA();
  
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      if ((grid[x+y*WIDTH]) != (newgrid[x+y*WIDTH])) {
        if (newgrid[x+y*WIDTH] == 1) color = LTDC_WHITE;
        else color = LTDC_BLACK;
        buffer[(2*x)+(2*y)*LTDC_F746_ROKOTECH.width] = color;
      }
    }
  }

  memcpy(grid, newgrid, WIDTH*HEIGHT);
 
}

void computeCA() {
  
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x+y*WIDTH] == true && (neighbors == 2 || neighbors == 3)) newgrid[x+y*WIDTH] = true;
      else if (grid[x+y*WIDTH] == true)  newgrid[x+y*WIDTH] = false;
      if (grid[x+y*WIDTH] == false && (neighbors == 3)) newgrid[x+y*WIDTH] = true;
      else if (grid[x+y*WIDTH] == false) newgrid[x+y*WIDTH] = false;
    }
  }
}

uint8_t getNumberOfNeighbors(uint8_t x, uint8_t y) { return grid[(x-1)+y*WIDTH] + grid[(x-1)+(y-1)*WIDTH] + grid[x+(y-1)*WIDTH] + grid[(x+1)+(y-1)*WIDTH] + grid[(x+1)+y*WIDTH] + grid[(x+1)+(y+1)*WIDTH] + grid[x+(y+1)*WIDTH] + grid[(x-1)+(y+1)*WIDTH]; } 
