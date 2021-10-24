/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_HX8347D_LCD_H
#define __STM32_HX8347D_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "hx8347d.h"
#include "Fonts/fonts.h"
#include "main.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32_HX8347D
  * @{
  */
 
/** @addtogroup STM32_HX8347D_LCD
  * @{
  */ 


/** @defgroup STM32_HX8347D_LCD_Exported_Types
  * @{
  */
   
/** 
  * @brief  Draw Properties structures definition
  */ 
typedef struct 
{ 
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT    *pFont; 

}LCD_DrawPropTypeDef;

/** 
  * @brief  Point structures definition
  */ 
typedef struct 
{
  int16_t X;
  int16_t Y;

}Point, * pPoint;

/** 
  * @brief  Line mode structures definition
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /*!< Center mode */
  RIGHT_MODE              = 0x02,    /*!< Right mode  */
  LEFT_MODE               = 0x03     /*!< Left mode   */

}Line_ModeTypdef;

/**
  * @}
  */ 

/** @defgroup STM32_HX8347D_LCD_Exported_Constants
  * @{
  */
  
#define __IO    volatile  

/** 
  * @brief  LCD status structure definition  
  */     
#define LCD_OK         0x00
#define LCD_ERROR      0x01
#define LCD_TIMEOUT    0x02

/** 
  * @brief  LCD color  
  */
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_GREY          0xF7DE          
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_WHITE         0xFFFF

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT         Font24
//======================================================================================================================////========================================================================================================================//
/**
  * @}
  */
	
/*###################### SPI1 ###################################*/
#define LCD_SPIx                                 SPI1
#define LCD_SPIx_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define LCD_SPIx_SCK_GPIO_PORT                   GPIOB
#define LCD_SPIx_SCK_PIN                         GPIO_PIN_3
#define LCD_SPIx_SCK_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_SPIx_SCK_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

#define LCD_SPIx_MISO_MOSI_GPIO_PORT             GPIOA
#define LCD_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOA_CLK_DISABLE()
#define LCD_SPIx_MISO_PIN                        GPIO_PIN_6
#define LCD_SPIx_MOSI_PIN                        GPIO_PIN_7
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000

/**
  * @brief  LCD Control Lines management
  */
#define LCD_CS_LOW()      HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()     HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()      HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()     HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)
#define LCD_BL_ON()				HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)
#define LCD_BL_OFF()			HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)
/**
  * @brief  LCD Control Interface pins (shield D10)
  */
#define LCD_CS_PIN                                 LCD_CS_Pin
#define LCD_CS_GPIO_PORT                           LCD_CS_GPIO_Port
#define LCD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

/**
  * @brief  LCD Data/Command Interface pins
  */
#define LCD_DC_PIN                                 LCD_DC_Pin
#define LCD_DC_GPIO_PORT                           LCD_DC_GPIO_Port
#define LCD_DC_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

//=========================================================2=============================================================//
/** @defgroup STM32_HX8347D_LCD_Exported_Functions
  * @{
  */   
uint8_t  BSP_LCD_Init(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);
 
uint16_t BSP_LCD_GetTextColor(void);
uint16_t BSP_LCD_GetBackColor(void);
void     BSP_LCD_SetTextColor(__IO uint16_t Color);
void     BSP_LCD_SetBackColor(__IO uint16_t Color);
void		 BSP_LCD_SetRotation(uint8_t rotation);
void     BSP_LCD_SetFont(sFONT *fonts);
sFONT    *BSP_LCD_GetFont(void);

void     BSP_LCD_Clear(uint16_t Color);
void     BSP_LCD_ClearStringLine(uint16_t Line);
void     BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr, uint16_t RGBCode);
void     BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Line_ModeTypdef Mode, uint16_t RGBCode);
void     BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii, uint16_t RGBCode);

void     BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void     BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t RGBCode);
void     BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t RGBCode);
void     BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t RGBCode);
void     BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode);
void     BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t RGBCode);
void     BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount, uint16_t RGBCode);
void     BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp);
void     BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode);
void     BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t RGBCode);
void     BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount, uint16_t RGBCode);
void     BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint16_t RGBCode);
void     BSP_LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3, uint16_t RGBCode);

void     BSP_LCD_DisplayOff(void);
void     BSP_LCD_DisplayOn(void);


uint16_t BSP_LCD_GetColor565(uint8_t red, uint8_t green, uint8_t blue);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32_HX8347D_LCD_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
