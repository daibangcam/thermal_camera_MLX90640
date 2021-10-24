/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __st7789v_H
#define __st7789v_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup st7789v
  * @{
  */

/** @defgroup st7789v_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup st7789v_Exported_Constants
  * @{
  */
      
/** 
  * @brief  st7789v Size  
  */  
#define  ST7789V_LCD_PIXEL_WIDTH    ((uint16_t)240)
#define  ST7789V_LCD_PIXEL_HEIGHT   ((uint16_t)320)


/** 
  * @brief  LCD Lines depending on the chosen fonts.  
  */
#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19) 
   
/**
  * @}
  */

/** @defgroup ADAFRUIT_SPI_LCD_Exported_Functions
  * @{
  */ 
void     st7789v_Init(void);
uint16_t st7789v_ReadID(void);

void     st7789v_DisplayOn(void);
void     st7789v_DisplayOff(void);
void     st7789v_SetCursor(uint16_t Xpos, uint16_t Ypos);
void 		 st7789_SetRotation(uint8_t rotation);
void     st7789v_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
void     st7789v_WriteReg(uint8_t LCDReg, uint8_t LCDRegValue);
uint8_t  st7789v_ReadReg(uint8_t LCDReg);
void     st7789v_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void 		 st7789v_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode);
void     st7789v_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     st7789v_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);

uint16_t st7789v_GetLcdPixelWidth(void);
uint16_t st7789v_GetLcdPixelHeight(void);
void     st7789v_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);

/* LCD driver structure */
extern LCD_DrvTypeDef   st7789v_drv;

/* LCD IO functions */
void     LCD_IO_Init(void);
void     LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
void     LCD_IO_WriteReg(uint8_t Reg);
void     LCD_Delay(uint32_t delay);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __st7789v_H */

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */
