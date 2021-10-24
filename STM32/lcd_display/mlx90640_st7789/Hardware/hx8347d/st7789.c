/* Includes ------------------------------------------------------------------*/
#include "st7789.h"
#include "stm32_hx8347d_lcd.h"
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 

/** @addtogroup st7789v
  * @brief      This file provides a set of functions needed to drive the
  *             st7789v LCD.
  * @{
  */

/** @defgroup st7789v_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup st7789v_Private_Defines
  * @{
  */

/**
  * @}
  */ 

/** @defgroup st7789v_Private_Macros
  * @{
  */

/**
  * @}
  */  

/** @defgroup st7789v_Private_Variables
  * @{
  */ 

LCD_DrvTypeDef   st7789v_drv = 
{
  st7789v_Init,
  0,
  st7789v_DisplayOn,
  st7789v_DisplayOff,
  st7789v_SetCursor,
	st7789_SetRotation,
  st7789v_WritePixel,
  0,
  st7789v_SetDisplayWindow,
	st7789v_FillRect,
  st7789v_DrawHLine,
  st7789v_DrawVLine,
  st7789v_GetLcdPixelWidth,
  st7789v_GetLcdPixelHeight,
  st7789v_DrawBitmap,
};

//static uint8_t Is_st7789v_Initialized = 0;
static uint16_t ArrayRGB[320] = {0};

uint16_t 	st7789v_width = 240;
uint16_t 	st7789v_height = 320;
uint8_t 	st7789v_rotation = 0;
/**
* @}
*/ 

/** @defgroup st7789v_Private_FunctionPrototypes
  * @{
  */
void st7789v_WriteColor(uint16_t color, uint32_t len);
/**
* @}
*/ 

/** @defgroup st7789v_Private_Functions
  * @{
  */
  
/**
  * @brief  Writes to the selected LCD register.
  * @param  LCDReg: Address of the selected register.
  * @param  LCDRegValue: value to write to the selected register.
  * @retval None
  */
void st7789v_WriteReg(uint8_t LCDReg, uint8_t LCDRegValue)
{
  LCD_IO_WriteReg(LCDReg);
  LCD_IO_WriteMultipleData(&LCDRegValue, 1);
}

/**
  * @brief  Initialize the st7789v LCD Component.
  * @param  None
  * @retval None
  */
void st7789v_Init(void)
{    

  /* Initialize st7789v low level bus layer -----------------------------------*/
		LCD_IO_Init();
  /* Out of sleep mode, 0 args, no delay */
		st7789v_WriteReg(0x11, 0x00); 
  /**/    
		LCD_Delay(10);
		st7789v_WriteReg(0x36, 0x00);

    st7789v_WriteReg(0x3A, 0x55);


    st7789v_WriteReg(0xB2, 0x0C);
    st7789v_WriteReg(0xB2, 0x0C); 
    st7789v_WriteReg(0xB2, 0x00);   
    st7789v_WriteReg(0xB2, 0x33);   
    st7789v_WriteReg(0xB2, 0x33);   

    st7789v_WriteReg(0xB7, 0x35);   //VGH=13.26V, VGL=-10.43V

    st7789v_WriteReg(0xBB, 0x28);   //VCOM

    st7789v_WriteReg(0xC0, 0x3C);   

    st7789v_WriteReg(0xC2, 0x01);   

    st7789v_WriteReg(0xC3, 0x0B); 	//VAP  //5V

    st7789v_WriteReg(0xC4, 0x20);   

    st7789v_WriteReg(0xC6, 0x0F);   

    st7789v_WriteReg(0xD0, 0xA4);   
    st7789v_WriteReg(0xD0, 0xA1);   


    st7789v_WriteReg(0xE0, 0xD0);   
    st7789v_WriteReg(0xE0, 0x01);   
    st7789v_WriteReg(0xE0, 0x08);   
    st7789v_WriteReg(0xE0, 0x0f);   
    st7789v_WriteReg(0xE0, 0x11);   
    st7789v_WriteReg(0xE0, 0x2a);   
    st7789v_WriteReg(0xE0, 0x36);   
    st7789v_WriteReg(0xE0, 0x55);   
    st7789v_WriteReg(0xE0, 0x44);   
    st7789v_WriteReg(0xE0, 0x3a);   
    st7789v_WriteReg(0xE0, 0x0b);   
    st7789v_WriteReg(0xE0, 0x06);   
    st7789v_WriteReg(0xE0, 0x11);   
    st7789v_WriteReg(0xE0, 0x20);   


    st7789v_WriteReg(0xE1, 0xD0);   
    st7789v_WriteReg(0xE1, 0x02);   
    st7789v_WriteReg(0xE1, 0x07);   
    st7789v_WriteReg(0xE1, 0x0A);   
    st7789v_WriteReg(0xE1, 0x0b);   
    st7789v_WriteReg(0xE1, 0x18);   
    st7789v_WriteReg(0xE1, 0x34);   
    st7789v_WriteReg(0xE1, 0x43);   
    st7789v_WriteReg(0xE1, 0x4a);   
    st7789v_WriteReg(0xE1, 0x2b);   
    st7789v_WriteReg(0xE1, 0x1b);   
    st7789v_WriteReg(0xE1, 0x1c);   
    st7789v_WriteReg(0xE1, 0x22);   
    st7789v_WriteReg(0xE1, 0x1f);   

		st7789v_WriteReg(0x55,0xb0);
    st7789v_WriteReg(0x29,0x00);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void st7789v_DisplayOn(void)
{
//  uint8_t data = 0;
  LCD_IO_WriteReg(0x13);/* Partial off (Normal): NORON */
  LCD_Delay(10);
  LCD_IO_WriteReg(0x29);/* Display on: DISPON */
  LCD_Delay(10);
//  LCD_IO_WriteReg(0x36);/* Memory data access control: MADCTL */ 
//  data = 0xC0;
//  LCD_IO_WriteMultipleData(&data, 1);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void st7789v_DisplayOff(void)
{
//  uint8_t data = 0;
  LCD_IO_WriteReg(0x13);/* Partial off (Normal): NORON */
  LCD_Delay(10);
  LCD_IO_WriteReg(0x28);/* Display off: DISPOFF */
  LCD_Delay(10);
//  LCD_IO_WriteReg(0x36);/* Memory data access control: MADCTL */ 
//  data = 0xC0;
//  LCD_IO_WriteMultipleData(&data, 1);
}

/**
  * @brief  Sets Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void st7789v_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  uint8_t data = 0;
  LCD_IO_WriteReg(0x2A);/* Column address set: CASET */
  data = (Xpos) >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Xpos) & 0xFF;
  LCD_IO_WriteMultipleData(&data, 1);
  LCD_IO_WriteReg(0x2B);/* Row address set: RASET */
  data = (Ypos) >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Ypos) & 0xFF;
  LCD_IO_WriteMultipleData(&data, 1);
  LCD_IO_WriteReg(0x2C);/* Memory write: RAMWR */
}

/**
  * @brief  Writes pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  * @retval None
  */
void st7789v_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  uint8_t data = 0;
//  if((Xpos > ST7789V_LCD_PIXEL_WIDTH) || (Ypos > ST7789V_LCD_PIXEL_HEIGHT)) 
//  {
//    return;
//  }
  
  /* Set Cursor */
  st7789v_SetCursor(Xpos, Ypos);
  
  data = RGBCode >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = RGBCode;
  LCD_IO_WriteMultipleData(&data, 1);
}  




/**
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void st7789v_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint8_t data = 0;
  /* Column addr set, 4 args, no delay: XSTART = Xpos, XEND = (Xpos + Width - 1) */
  LCD_IO_WriteReg(0x2A);/* Column address set: CASET */ 
  data = (Xpos) >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Xpos) & 0xFF;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Xpos + Width - 1) >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Xpos + Width - 1) & 0xFF;
  LCD_IO_WriteMultipleData(&data, 1);
  /* Row addr set, 4 args, no delay: YSTART = Ypos, YEND = (Ypos + Height - 1) */
  LCD_IO_WriteReg(0x2B);/* Row address set: RASET */
  data = (Ypos) >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Ypos) & 0xFF;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Ypos +  Height - 1) >> 8;
  LCD_IO_WriteMultipleData(&data, 1);
  data = (Ypos + Height - 1) & 0xFF;
  LCD_IO_WriteMultipleData(&data, 1);
}

/**
  * @brief  Draws horizontal line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void st7789v_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint8_t counter = 0;
  
	if(0 == st7789v_rotation || 2 == st7789v_rotation){
		if(Xpos + Length > ST7789V_LCD_PIXEL_WIDTH) return;
  
		/* Set Cursor */
		st7789v_SetCursor(Xpos, Ypos);
		
		for(counter = 0; counter < Length; counter++)
		{
			ArrayRGB[counter] = RGBCode;
		}
		LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
	}
	else{
		if(Xpos + Length > ST7789V_LCD_PIXEL_HEIGHT) return;
  
		/* Set Cursor */
		st7789v_SetCursor(Xpos, Ypos);
		
		for(counter = 0; counter < Length; counter++)
		{
			ArrayRGB[counter] = RGBCode;
		}
		LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
	}
}

/**
  * @brief  Draws vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void st7789v_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint8_t counter = 0;
  
  if(Ypos + Length > ST7789V_LCD_PIXEL_HEIGHT) return;
  for(counter = 0; counter < Length; counter++)
  {
    st7789v_WritePixel(Xpos, Ypos + counter, RGBCode);
  }   
}

/**
  * @brief  Gets the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t st7789v_GetLcdPixelWidth(void)
{
  return ST7789V_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Gets the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t st7789v_GetLcdPixelHeight(void)
{                          
  return ST7789V_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void st7789v_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  
  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;
  
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 0, MX = 1, MV = 0, ML = 0 */
	st7789v_WriteReg(0x36, 0x40);// LCD_REG_54              0x36 /* Memory data access control: MADCTL */ 
  /* Set Cursor */
  st7789v_SetCursor(Xpos, Ypos);  
 
  LCD_IO_WriteMultipleData((uint8_t*)pbmp, size*2);
 
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
	st7789v_WriteReg(0x36, 0xC0);/* Memory data access control: MADCTL */ 
}

/**
* @}
*/ 

void st7789v_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode)
{
	if ((Xpos >= st7789v_width) || (Ypos >= st7789v_height)) return;

  int16_t x2 = Xpos + Width - 1, y2 = Ypos + Height - 1;
  if ((x2 < 0) || (y2 < 0)) return;


  // Clip right/bottom
  if (x2 >= st7789v_width)  Width = st7789v_width  - Xpos;
  if (y2 >= st7789v_height) Height = st7789v_height - Ypos;

  int32_t len = (int32_t)Width * Height;
  st7789v_SetDisplayWindow(Xpos, Ypos, Width, Height);
	
	/* Prepare to write GRAM */
  LCD_IO_WriteReg(0x2C);
	st7789v_WriteColor(RGBCode,len);
	
}

void st7789v_WriteColor(uint16_t color, uint32_t len){
//#define TFT_MAX_PIXELS_AT_ONCE      32	
	uint16_t temp[32] ;
	size_t blen = (len > 32) ? 32 : len;
	uint16_t tlen = 0;
	
	for(uint32_t t = 0; t < blen; t ++){
		temp[t] = color;
	}
  
	while (len) {
    tlen = (len > blen) ? blen : len;
    LCD_IO_WriteMultipleData((uint8_t*)temp, tlen*2);
    len -= tlen;
  }
  
}

/**
* @brief  Set the Rotation.
* @param  rotation: 0(0бу), 1(90бу), 2(180бу), 3(270бу)
* @retval None
*/
void st7789_SetRotation(uint8_t rotation)
{
	st7789v_rotation = rotation%4;
	switch(st7789v_rotation){
		case 0:
			/* Memory access control: MY = 0, MX = 0, MV = 0, ML = 0 */
			/*  */
			st7789v_WriteReg(0X36, 0x08);
			st7789v_WriteReg(0x2A, 0x00);
			st7789v_WriteReg(0x2A, 0x00);
			st7789v_WriteReg(0x2A, 0x00);
			st7789v_WriteReg(0x2A, 0xEF);
			st7789v_WriteReg(0x2B, 0x00);
			st7789v_WriteReg(0x2B, 0x00);
			st7789v_WriteReg(0x2B, 0x01);
			st7789v_WriteReg(0x2B, 0x3F);
			st7789v_height = ST7789V_LCD_PIXEL_HEIGHT;
			st7789v_width = ST7789V_LCD_PIXEL_WIDTH;
			break;
		case 1:
			/* Memory access control: MY = 0, MX = 1, MV = 1, ML = 0 */
			st7789v_WriteReg(0X36, 0x68);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0x01);
			st7789v_WriteReg(0x2A,  0x3F);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0xEF);
			st7789v_height = ST7789V_LCD_PIXEL_WIDTH;
			st7789v_width = ST7789V_LCD_PIXEL_HEIGHT;
			break;
		case 2:
			/* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
			st7789v_WriteReg(0X36, 0xC8);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0xEF);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0x01);
			st7789v_WriteReg(0x2B,  0x3F);
			st7789v_height = ST7789V_LCD_PIXEL_HEIGHT;
			st7789v_width = ST7789V_LCD_PIXEL_WIDTH;
			break;
		case 3:
			/* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
			st7789v_WriteReg(0X36, 0xA8);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0x00);
			st7789v_WriteReg(0x2A,  0x01);
			st7789v_WriteReg(0x2A,  0x3F);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0x00);
			st7789v_WriteReg(0x2B,  0xEF);
			st7789v_height = ST7789V_LCD_PIXEL_WIDTH;
			st7789v_width = ST7789V_LCD_PIXEL_HEIGHT;
			break;
	}
		
}

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/
