// Raytracer demo //

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define WIDTH LTDC_F746_ROKOTECH.width
#define HEIGHT LTDC_F746_ROKOTECH.height

#include "raytracer.h"

void setup(){

  uint16_t *buffer = (uint16_t *)malloc(2 * LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height);

  tft.begin((uint16_t *)buffer);
  
  tft.fillScreen(LTDC_BLACK);

  tft.setTextColor(LTDC_WHITE, LTDC_BLACK);
  tft.setTextSize(1);

  tft.setTextColor(LTDC_WHITE, LTDC_BLACK);

  unsigned long t = millis();

  doRaytrace(8);  // very high quality 480x272 antialiased, 8 samples

}

void loop(){ 
 
}
