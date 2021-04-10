// Turmites cellular automata //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define USER_BTN  PI11 
#define WIDTH LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height

#define ITER 100

  uint32_t size = 1024 * 1024 - 8;

  uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

  int posx,posy;
  int oldposx, oldposy;
  int state;
  int dir;
  int *world = (int*)malloc(size);
  int last_filled;
  int current_col;
  int next_col[4][4];
  int next_state[4][4];
  int directions[4][4];


void rndrule(){

  tft.fillScreen(LTDC_BLACK);

  state = rand()%4;
  dir = 0;
  posx = WIDTH/2;
  posy = HEIGHT/2;
  
  for(int j=0; j<4; j++){   
    for(int i=0; i<4; i++){         
      next_col[i][j] = rand()%4;
      next_state[i][j] = rand()%4;
      directions[i][j] = rand()%8;
    }   
  }

  for (int j=0; j < HEIGHT; j++){ for (int i = 0; i < WIDTH; i++) world[i+j*WIDTH] = 0; } 

  world[posx+posy*WIDTH] = rand()%4;

}

void move_turmite(){
  
  int cols = world[posx+posy*WIDTH];
  
  oldposx = posx;
  oldposy = posy;
  current_col = next_col[cols][state];
  world[posx+posy*WIDTH] = next_col[cols][state];
  state = next_state[cols][state];    

  dir = (dir + directions[cols][state]) % 8;

  switch(dir){
    case 0: posy--; break;
    case 1: posy--; posx++; break;
    case 2: posx++; break;
    case 3: posx++; posy++; break;
    case 4: posy++; break;
    case 5: posy++; posx--; break;
    case 6: posx--; break;
    case 7: posx--; posy--; break;
  }

  if(posy < 0) posy = HEIGHT-1;
  if(posy >= HEIGHT) posy = 0;
  if(posx < 0) posx = WIDTH-1;
  if(posx >= WIDTH) posx=0;
  
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

  move_turmite();
    
  switch(current_col){
    case 0: buffer[oldposx + oldposy * WIDTH] = LTDC_RED; break;
    case 1: buffer[oldposx + oldposy * WIDTH] = LTDC_GREEN; break;
    case 2: buffer[oldposx + oldposy * WIDTH] = LTDC_BLUE; break;
    case 3: buffer[oldposx + oldposy * WIDTH] = LTDC_WHITE; break;
  }

  delayMicroseconds(ITER);
    
}
