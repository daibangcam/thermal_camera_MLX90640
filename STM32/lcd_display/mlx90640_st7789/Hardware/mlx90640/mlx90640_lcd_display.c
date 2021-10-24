#include <string.h>
#include "mlx90640_lcd_display.h"
#include "stm32_hx8347d_lcd.h"
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06

#define  MLX90640_ADDR 0x33
#define	 RefreshRate FPS8HZ 
#define EMMISIVITY 0.95f
#define TA_SHIFT 8 //Default shift for MLX90640 in open air

paramsMLX90640 mlx90640;
static uint16_t eeMLX90640[832];  
int status;

// start with some initial colors
float minTemp = 20.0f;
float maxTemp = 40.0f;
float centerTemp;

char tempBuffer[6];
// variables for interpolated colors
uint8_t red, green, blue;

// variables for row/column interpolation
float intPoint, val, a, b, c, d, ii;
int x, y, i, j;

// array for the 32 x 24 measured tempValues
static float tempValues[32*24];


static uint16_t TempToColor(float val);
static void setTempScale(void);
static void setAbcd(void);
static void drawLegend(void);
static void drawMeasurement(void);
static void drawPicture(void);
static void readTempValues(void);

static uint16_t TempToColor(float val){
	  /*
    pass in value and figure out R G B
    several published ways to do this I basically graphed R G B and developed simple linear equations
    again a 5-6-5 color display will not need accurate temp to R G B color calculation

    equations based on
    http://web-tech.ga-usa.com/2012/05/creating-a-custom-hot-to-cold-temperature-color-gradient-for-use-with-rrdtool/index.html

  */

  red = constrain(255.0f / (c - b) * val - ((b * 255.0f) / (c - b)), 0, 255);

  if ((val > minTemp) & (val < a)) {
    green = constrain(255.0f / (a - minTemp) * val - (255.0f * minTemp) / (a - minTemp), 0, 255);
  }
  else if ((val >= a) & (val <= c)) {
    green = 255;
  }
  else if (val > c) {
    green = constrain(255.0f / (c - d) * val - (d * 255.0f) / (c - d), 0, 255);
  }
  else if ((val > d) | (val < a)) {
    green = 0;
  }

  if (val <= b) {
    blue = constrain(255.0f / (a - b) * val - (255.0f * b) / (a - b), 0, 255);
  }
  else if ((val > b) & (val <= d)) {
    blue = 0;
  }
  else if (val > d) {
    blue = constrain(240.0f / (maxTemp - d) * val - (d * 240.0f) / (maxTemp - d), 0, 240);
  }

  // use the displays color mapping function to get 5-6-5 color palet (R=5 bits, G=6 bits, B-5 bits)
  return BSP_LCD_GetColor565(red, green, blue);
}

static void setTempScale(void) {
  minTemp = 255;
  maxTemp = 0;

  for (i = 0; i < 768; i++) {
    minTemp = min(minTemp, tempValues[i]);
    maxTemp = max(maxTemp, tempValues[i]);
  }

  setAbcd();
  drawLegend();
}


// Function to get the cutoff points in the temp vs RGB graph.
static void setAbcd(void) {
  a = minTemp + (maxTemp - minTemp) * 0.2121f;
  b = minTemp + (maxTemp - minTemp) * 0.3182f;
  c = minTemp + (maxTemp - minTemp) * 0.4242f;
  d = minTemp + (maxTemp - minTemp) * 0.8182f;
}


// Draw a legend.
static void drawLegend(void) {
  float inc = (maxTemp - minTemp) / 224.0f;
  j = 0;
  for (ii = minTemp; ii < maxTemp; ii += inc) {
    BSP_LCD_DrawVLine(8+ + j++, 292, 20, TempToColor(ii));
  }
	
	BSP_LCD_SetFont(&Font16);
	memset(tempBuffer,0,sizeof(tempBuffer));
	sprintf(tempBuffer,(char *)"%2.1f",minTemp);
	BSP_LCD_DisplayStringAt(8,290,(uint8_t *)tempBuffer,LEFT_MODE,LCD_COLOR_BLUE);

	memset(tempBuffer,0,sizeof(tempBuffer));
	sprintf(tempBuffer,(char *)"%2.1f",maxTemp);
	BSP_LCD_DisplayStringAt(190,290,(uint8_t *)tempBuffer,LEFT_MODE,LCD_COLOR_RED);

}

// Draw a circle + measured value.
static void drawMeasurement(void ) {

  // Mark center measurement
  BSP_LCD_DrawCircle(120, 8+84, 3, LCD_COLOR_WHITE);

  // Measure and print center temperature
  centerTemp = (tempValues[383 - 16] + tempValues[383 - 15] + tempValues[384 + 15] + tempValues[384 + 16]) / 4;
	
	BSP_LCD_SetFont(&Font20);
	memset(tempBuffer,0,sizeof(tempBuffer));
	sprintf(tempBuffer,(char *)"%2.2f ",centerTemp);
	BSP_LCD_DisplayStringAt(88,220,(uint8_t *)tempBuffer,LEFT_MODE,LCD_COLOR_RED);
}

static void drawPicture(void) {
	for (y=0; y<24; y++) {
		for (x=0; x<32; x++) {
			BSP_LCD_FillRect(8 + x*7, 8 + y*7, 7, 7, TempToColor(tempValues[(31-x) + (y*32)]));
		}
	}
}

// Read pixel data from MLX90640.
static void readTempValues(void) {

  for (uint8_t x = 0 ; x < 2 ; x++) // Read both subpages
  {
    uint16_t mlx90640Frame[834];
    status = MLX90640_GetFrameData(MLX90640_ADDR, mlx90640Frame);
    if (status < 0)
    {
      printf("GetFrame Error: %d\r\n",status);
    }

    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature

    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, EMMISIVITY, tr, tempValues);
  }
}

void mlx90640_display_process(void){
	readTempValues();
  setTempScale();
  drawPicture();
	drawMeasurement();
}

void mlx90640_display_init(void){
	MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
	MLX90640_SetChessMode(MLX90640_ADDR);
	
  status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
  if (status != 0) printf("\r\nload system parameters error with code:%d\r\n",status);
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0) printf("\r\nParameter extraction failed with error code:%d\r\n",status);
}

