// Super Langtons Ant //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define USER_BTN  PI11 
#define WIDTH LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height

  uint32_t size = 1024 * 1024 - 8;
  uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

#define ITER 25
#define LENGHT 32

  uint16_t *state = (uint16_t*)malloc(size);
  uint16_t antX = WIDTH/2;
  uint16_t antY = HEIGHT/2;
  uint16_t direction;
  uint16_t stateCount;
  bool type[LENGHT];
  uint16_t stateCols[LENGHT];


void rndrule(){

  tft.fillScreen(LTDC_BLACK);

  antX = WIDTH/2;
  antY = HEIGHT/2;

  stateCount = 2 + rand()%(LENGHT-2);

  direction = rand()%4;

  for(int i = 0; i < stateCount; i++) type[i] = rand()%2;
  for(int i = 0; i < stateCount; i++) stateCols[i] = rand();

  for (int j=0; j < HEIGHT; j++){ for (int i = 0; i < WIDTH; i++) state[i+j*WIDTH] = 0; }

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

  move();
  turn(type[(state[antX+antY*WIDTH]%stateCount)]);
  state[antX+antY*WIDTH]++;
  buffer[antX+antY*WIDTH] = stateCols[(state[antX+antY*WIDTH]%stateCount)];

  delayMicroseconds(ITER);
    
}

void turn(int angle){
  
  if(angle == 0){
    if(direction == 0){
      direction = 3;
    } else {
      direction--;
    }
  } else {
    if(direction == 3){
      direction = 0;
    } else {
      direction++;
    }
  }
}

void move(){
  
  if(antY == 0 && direction == 0){
    antY = HEIGHT-1;
  } else {
    if(direction == 0 ){
      antY--;
    }
  }
  if(antX == WIDTH-1 && direction == 1){
    antX = 0;
  } else {
    if(direction == 1){
      antX++;
    }
  }
  if(antY == HEIGHT-1 && direction == 2){
   antY = 0; 
  } else {
    if(direction == 2){
      antY++;
    }
  }
  if(antX == 0 && direction == 3){
    antX = WIDTH-1;
  } else {
    if(direction == 3){
      antX--;
    }
  }
}
