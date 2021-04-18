// StarWars cellular automata //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

uint16_t *buffer = (uint16_t *)malloc(LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height * sizeof(uint16_t));

#define USER_BTN  PI11
#define WIDTH  240
#define HEIGHT 136
#define SCR 480*272

  uint16_t coll;

  uint8_t current [WIDTH][HEIGHT];
  uint8_t next [WIDTH][HEIGHT];
  uint8_t alive_counts [WIDTH][HEIGHT];
  uint8_t tmp[WIDTH][HEIGHT];
  
  int ALIVE = 3;
  int DEATH_1 = 2;
  int DEATH_2 = 1;
  int DEAD = 0;


void rndrule(){

  tft.fillScreen(LTDC_BLACK);
  
  for (int y = 0; y < HEIGHT; y++) { for (int x = 0; x < WIDTH; x++) current[x][y] = random(0, 100) < 20 ? ALIVE : DEAD; }
  
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

  step();

  memset(buffer, 0, 2*SCR);

  draw_type(50, 100, LTDC_BLUE);
  draw_type(2, 49, LTDC_YELLOW);
  draw_type(0, 1, LTDC_RED);
 
}

void step(){
 
  for (int x = 0; x < WIDTH; x++) {  
    for (int y = 0; y < HEIGHT; y++) {
    
      int count = 0;
      int next_val;
    
      int mx = WIDTH-1;
      int my = HEIGHT-1;
    
      int self = current[x][y];
    
      for (int nx = x-1; nx <= x+1; nx++) {
  
        for (int ny = y-1; ny <= y+1; ny++) {
    
          if (nx == x && ny == y) continue;     
          if (current[wrap(nx, mx)][wrap(ny, my)] == ALIVE) count++;
      
        }   
      }  

    int neighbors = count;
    
    if (self == ALIVE) next_val = ((neighbors == 3) || (neighbors == 4) || (neighbors == 5)) ? ALIVE : DEATH_1;
  
    else if (self == DEAD) next_val = (neighbors == 2) ? ALIVE : DEAD;
  
    else next_val = self-1;
   
    next[x][y] = next_val;
  
    if (next_val == ALIVE) alive_counts[x][y] = min(alive_counts[x][y]+1, 100);
    else if (next_val == DEAD) alive_counts[x][y] = 0;
    
    }
  }
 
    for (int x = 0; x < WIDTH; x++) { 
      for (int y = 0; y < HEIGHT; y++) {
         
        tmp[x][y] = current[x][y];
        current[x][y] = next[x][y];
        next[x][y] = tmp[x][y];
    
    }
  } 
}
  
int wrap(int v, int m){

    if (v < 0) return v + m;
    else if (v >= m) return v - m;
    else return v;
}
  
void draw_type(int min_alive, int max_alive, uint16_t s) {  
    
  for (int x = 0; x < WIDTH; x++) {
      
    for (int y = 0; y < HEIGHT; y++) {
    
      int self = current[x][y];
      if (self == DEAD) continue;
   
      int alive = alive_counts[x][y];
      if (alive < min_alive || alive > max_alive) continue;
        
      if (self == ALIVE) coll = s;
      else if (self == DEATH_1) coll = s>>2;
      else if (self == DEATH_2) coll = s>>5;
    
      buffer[(2*x)+(2*y)*LTDC_F746_ROKOTECH.width] = coll;
        
    }
  }
 
}
