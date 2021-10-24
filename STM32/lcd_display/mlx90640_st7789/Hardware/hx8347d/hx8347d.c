/* Includes ------------------------------------------------------------------*/
#include "hx8347d.h"
#include "stm32_hx8347d_lcd.h"
/** @addtogroup BSP
* @{
*/ 

/** @addtogroup Components
* @{
*/ 

/** @addtogroup HX8347D
* @brief     This file provides a set of functions needed to drive the
*            HX8347D LCD.
* @{
*/

/** @defgroup HX8347D_Private_TypesDefinitions
* @{
*/ 

/**
* @}
*/ 

/** @defgroup HX8347D_Private_Defines
* @{
*/
//#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
//#define MIN(a,b) ( ((a)>(b)) ? (b):(a) )
/**
* @}
*/ 

/** @defgroup HX8347D_Private_Macros
* @{
*/

/**
* @}
*/  

/** @defgroup HX8347D_Private_Variables
* @{
*/ 
LCD_DrvTypeDef   hx8347d_drv = 
{
  hx8347d_Init,
  hx8347d_ReadID,
  hx8347d_DisplayOn,
  hx8347d_DisplayOff,
  hx8347d_SetCursor,
	hx8347d_SetRotation,
  hx8347d_WritePixel,
  hx8347d_ReadPixel,
  hx8347d_SetDisplayWindow,
	hx8347d_FillRect,
  hx8347d_DrawHLine,
  hx8347d_DrawVLine,
  hx8347d_GetLcdPixelWidth,
  hx8347d_GetLcdPixelHeight,
  hx8347d_DrawBitmap,  
};

static uint8_t Is_hx8347d_Initialized = 0;
//static uint16_t ArrayRGB[320] = {0};

uint16_t _width = 240;
uint16_t _height = 320;
uint8_t _rotation = 0;

/**
* @}
*/ 

/** @defgroup HX8347D_Private_FunctionPrototypes
* @{
*/
void hx8347d_WriteColor(uint16_t color, uint32_t len);
/**
* @}
*/ 

/** @defgroup HX8347D_Private_Functions
* @{
*/   

/**
* @brief  Initialise the HX8347D LCD Component.
* @param  None
* @retval None
*/
void hx8347d_Init(void)
{  
	  if(Is_hx8347d_Initialized == 0)
  {
    Is_hx8347d_Initialized = 1;
    /* Initialise HX8347D low level bus layer --------------------------------*/
    
    /* Driving ability setting */
    hx8347d_WriteReg(LCD_REG_234, 0x00);
    hx8347d_WriteReg(LCD_REG_235, 0x20);
    hx8347d_WriteReg(LCD_REG_236, 0x0C);
    hx8347d_WriteReg(LCD_REG_237, 0xC4);
    hx8347d_WriteReg(LCD_REG_232, 0x40);
    hx8347d_WriteReg(LCD_REG_233, 0x38);
    hx8347d_WriteReg(LCD_REG_241, 0x01);
    hx8347d_WriteReg(LCD_REG_242, 0x10);
    hx8347d_WriteReg(LCD_REG_39,  0xA3);
    
    /* Adjust the Gamma Curve */
    hx8347d_WriteReg(LCD_REG_64, 0x01);
    hx8347d_WriteReg(LCD_REG_65, 0x00);
    hx8347d_WriteReg(LCD_REG_66, 0x00);
    hx8347d_WriteReg(LCD_REG_67, 0x10);
    hx8347d_WriteReg(LCD_REG_68, 0x0E);
    hx8347d_WriteReg(LCD_REG_69, 0x24);
    hx8347d_WriteReg(LCD_REG_70, 0x04);
    hx8347d_WriteReg(LCD_REG_71, 0x50);
    hx8347d_WriteReg(LCD_REG_72, 0x02);
    hx8347d_WriteReg(LCD_REG_73, 0x13);
    hx8347d_WriteReg(LCD_REG_74, 0x19);
    hx8347d_WriteReg(LCD_REG_75, 0x19);
    hx8347d_WriteReg(LCD_REG_76, 0x16);
    hx8347d_WriteReg(LCD_REG_80, 0x1B);
    hx8347d_WriteReg(LCD_REG_81, 0x31);
    hx8347d_WriteReg(LCD_REG_82, 0x2F);
    hx8347d_WriteReg(LCD_REG_83, 0x3F);
    hx8347d_WriteReg(LCD_REG_84, 0x3F);
    hx8347d_WriteReg(LCD_REG_85, 0x3E);
    hx8347d_WriteReg(LCD_REG_86, 0x2F);
    hx8347d_WriteReg(LCD_REG_87, 0x7B);
    hx8347d_WriteReg(LCD_REG_88, 0x09);
    hx8347d_WriteReg(LCD_REG_89, 0x06);
    hx8347d_WriteReg(LCD_REG_90, 0x06);
    hx8347d_WriteReg(LCD_REG_91, 0x0C);
    hx8347d_WriteReg(LCD_REG_92, 0x1D);
    hx8347d_WriteReg(LCD_REG_93, 0xCC);
    
    /* Power voltage setting */
    hx8347d_WriteReg(LCD_REG_27, 0x1B);
    hx8347d_WriteReg(LCD_REG_26, 0x01);
    hx8347d_WriteReg(LCD_REG_36, 0x2F);
    hx8347d_WriteReg(LCD_REG_37, 0x57);
    /*****VCOM offset ****/
    hx8347d_WriteReg(LCD_REG_35, 0x86);
    
    /* Power on setting up flow */
    hx8347d_WriteReg(LCD_REG_24, 0x36); /* Display frame rate = 70Hz RADJ = '0110' */
    hx8347d_WriteReg(LCD_REG_25, 0x01); /* OSC_EN = 1 */
    hx8347d_WriteReg(LCD_REG_28, 0x06); /* AP[2:0] = 111 */
    hx8347d_WriteReg(LCD_REG_29, 0x06); /* AP[2:0] = 111 */
    hx8347d_WriteReg(LCD_REG_31, 0x90); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
    hx8347d_WriteReg(LCD_REG_39, 1); /* REF = 1 */
    
    LCD_Delay(10);
    /* 262k/65k color selection */
    hx8347d_WriteReg(LCD_REG_23, 0x05); /* default 0x06 262k color,  0x05 65k color */
    /* SET PANEL */
    hx8347d_WriteReg(LCD_REG_54, 0x0A); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 1, BGR_PANEL = 0 */ //color
    
    /* Display ON flow */
    hx8347d_WriteReg(LCD_REG_40, 0x38); /* GON=1, DTE=1, D=10 */
    LCD_Delay(60);
    hx8347d_WriteReg(LCD_REG_40, 0x3C); /* GON=1, DTE=1, D=11 */
    
    /* Set GRAM Area - Partial Display Control */
    hx8347d_WriteReg(LCD_REG_1, 0x00); /* DP_STB = 0, DP_STB_S = 0, SCROLL = 0, */
    hx8347d_WriteReg(LCD_REG_2, 0x00); /* Column address start 2 */
    hx8347d_WriteReg(LCD_REG_3, 0x00); /* Column address start 1 */
    hx8347d_WriteReg(LCD_REG_6, 0x00); /* Row address start 2 */
    hx8347d_WriteReg(LCD_REG_7, 0x00); /* Row address start 2 */
//    hx8347d_WriteReg(LCD_REG_22, 0x60); /* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
  }
  /* Set the Cursor */ 
  hx8347d_SetCursor(0, 0);
	
//	hx8347d_SetRotation(0);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
}

/**
* @brief  Enables the Display.
* @param  None
* @retval None
*/
void hx8347d_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  hx8347d_WriteReg(LCD_REG_24, 0x36); /* Display frame rate = 70Hz RADJ = '0110' */
  hx8347d_WriteReg(LCD_REG_25, 0x01); /* OSC_EN = 1 */
  hx8347d_WriteReg(LCD_REG_28, 0x06); /* AP[2:0] = 111 */
  hx8347d_WriteReg(LCD_REG_31, 0x90); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
  LCD_Delay(10);
  /* 262k/65k color selection */
  hx8347d_WriteReg(LCD_REG_23, 0x05); /* default 0x06 262k color,  0x05 65k color */
  /* SET PANEL */
  hx8347d_WriteReg(LCD_REG_54, 0x0A); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 1, BGR_PANEL = 0 */
  
  /* Display On */
  hx8347d_WriteReg(LCD_REG_40, 0x38);
  LCD_Delay(60);
  hx8347d_WriteReg(LCD_REG_40, 0x3C);
	
	LCD_BL_ON();
}

/**
* @brief  Disables the Display.
* @param  None
* @retval None
*/
void hx8347d_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  hx8347d_WriteReg(LCD_REG_23, 0x0000); /* default 0x06 262k color,  0x05 65k color */
  hx8347d_WriteReg(LCD_REG_24, 0x0000); /* Display frame rate = 70Hz RADJ = '0110' */
  hx8347d_WriteReg(LCD_REG_25, 0x0000); /* OSC_EN = 1 */
  hx8347d_WriteReg(LCD_REG_28, 0x0000); /* AP[2:0] = 111 */
  hx8347d_WriteReg(LCD_REG_31, 0x0000); /* GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0*/
  hx8347d_WriteReg(LCD_REG_54, 0x0000); /* SS_PANEL = 1, GS_PANEL = 0,REV_PANEL = 0, BGR_PANEL = 1 */
  
  /* Display Off */
  hx8347d_WriteReg(LCD_REG_40, 0x38);
  LCD_Delay(60);
  hx8347d_WriteReg(LCD_REG_40, 0x04);
	
	LCD_BL_OFF();
}

/**
* @brief  Get the LCD pixel Width.
* @param  None
* @retval The Lcd Pixel Width
*/
uint16_t hx8347d_GetLcdPixelWidth(void)
{
  return (uint16_t)_width;
}

/**
* @brief  Get the LCD pixel Height.
* @param  None
* @retval The Lcd Pixel Height
*/
uint16_t hx8347d_GetLcdPixelHeight(void)
{
  return (uint16_t)_height;
}

/**
* @brief  Get the HX8347D ID.
* @param  None
* @retval The HX8347D ID 
*/
uint16_t hx8347d_ReadID(void)
{
  if(Is_hx8347d_Initialized == 0)
  {
    hx8347d_Init();
    Is_hx8347d_Initialized = 1;    
  }
  return (hx8347d_ReadReg(0x00));
}

/**
* @brief  Set Cursor position.
* @param  Xpos: specifies the X position.
* @param  Ypos: specifies the Y position.
* @retval None
*/
void hx8347d_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	switch(_rotation){
		case 0:
			hx8347d_WriteReg(LCD_REG_6, Ypos >> 8);
			hx8347d_WriteReg(LCD_REG_7, Ypos & 0xFF);
			hx8347d_WriteReg(LCD_REG_2, Xpos >> 8);
			hx8347d_WriteReg(LCD_REG_3, Xpos & 0xFF);
			break;
		case 1:
			hx8347d_WriteReg(LCD_REG_6, Xpos >> 8);
			hx8347d_WriteReg(LCD_REG_7, Xpos & 0xFF);
			hx8347d_WriteReg(LCD_REG_2, Ypos >> 8);
			hx8347d_WriteReg(LCD_REG_3, Ypos & 0xFF);
			break;
		case 2:
			hx8347d_WriteReg(LCD_REG_6, Ypos >> 8);
			hx8347d_WriteReg(LCD_REG_7, Ypos & 0xFF);
			hx8347d_WriteReg(LCD_REG_2, Xpos >> 8);
			hx8347d_WriteReg(LCD_REG_3, Xpos & 0xFF);
			break;
		case 3:
			hx8347d_WriteReg(LCD_REG_6, Xpos >> 8);
			hx8347d_WriteReg(LCD_REG_7, Xpos & 0xFF);
			hx8347d_WriteReg(LCD_REG_2, Ypos >> 8);
			hx8347d_WriteReg(LCD_REG_3, Ypos & 0xFF);
			break;
	}
}

/**
* @brief  Write pixel.   
* @param  Xpos: specifies the X position.
* @param  Ypos: specifies the Y position.
* @param  RGBCode: the RGB pixel color
* @retval None
*/
void hx8347d_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&RGBCode, 2);
	
}

/**
* @brief  Read pixel.
* @param  None
* @retval the RGB pixel color
*/
uint16_t hx8347d_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);
  
  /* Dummy read */
  LCD_IO_ReadData(LCD_REG_34);
  
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCD_REG_34));
}

/**
* @brief  Writes to the selected LCD register.
* @param  LCDReg:      address of the selected register.
* @param  LCDRegValue: value to write to the selected register.
* @retval None
*/
void hx8347d_WriteReg(uint8_t LCDReg, uint16_t LCDRegValue)
{
  LCD_IO_WriteReg(LCDReg);
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&LCDRegValue, 2);
}

/**
* @brief  Reads the selected LCD Register.
* @param  LCDReg: address of the selected register.
* @retval LCD Register Value.
*/
uint16_t hx8347d_ReadReg(uint8_t LCDReg)
{
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCDReg));
}

/**
* @brief  Sets a display window
* @param  Xpos:   specifies the X bottom left position.
* @param  Ypos:   specifies the Y bottom left position.
* @param  Height: display window height.
* @param  Width:  display window width.
* @retval None
*/
void hx8347d_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Horizontal GRAM Start Address */
  hx8347d_WriteReg(LCD_REG_6, (Xpos) >> 8); /* SP */
  hx8347d_WriteReg(LCD_REG_7, (Xpos) & 0xFF); /* SP */
  
  /* Horizontal GRAM End Address */
  hx8347d_WriteReg(LCD_REG_8, (Xpos + Height - 1) >> 8); /* EP */
  hx8347d_WriteReg(LCD_REG_9, (Xpos + Height - 1) & 0xFF); /* EP */
  
  /* Vertical GRAM Start Address */
  hx8347d_WriteReg(LCD_REG_2, (Ypos) >> 8); /* SC */
  hx8347d_WriteReg(LCD_REG_3, (Ypos) & 0xFF); /* SC */
  
  /* Vertical GRAM End Address */
  hx8347d_WriteReg(LCD_REG_4, (Ypos + Width - 1) >> 8); /* EC */
  hx8347d_WriteReg(LCD_REG_5, (Ypos + Width - 1) & 0xFF); /* EC */
}


void hx8347d_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode){
	if ((Xpos >= _width) || (Ypos >= _height)) return;

  int16_t x2 = Xpos + Width - 1, y2 = Ypos + Height - 1;
  if ((x2 < 0) || (y2 < 0)) return;

//  // Clip left/top
//  if (Xpos < 0) {
//    Xpos = 0;
//    Width = x2 + 1;
//  }
//  if (Ypos < 0) {
//    Ypos = 0;
//    Height = y2 + 1;
//  }

  // Clip right/bottom
  if (x2 >= _width)  Width = _width  - Xpos;
  if (y2 >= _height) Height = _height - Ypos;

  int32_t len = (int32_t)Width * Height;
  hx8347d_SetDisplayWindow(Xpos, Ypos, Width, Height);
	
	/* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
	hx8347d_WriteColor(RGBCode,len);
	
}


void hx8347d_WriteColor(uint16_t color, uint32_t len){
	uint16_t temp[TFT_MAX_PIXELS_AT_ONCE] ;
	size_t blen = (len > TFT_MAX_PIXELS_AT_ONCE) ? TFT_MAX_PIXELS_AT_ONCE : len;
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
void hx8347d_SetRotation(uint8_t rotation)
{
	_rotation = rotation%4;
	switch(_rotation){
		case 0:
			/* Memory access control: MY = 0, MX = 0, MV = 0, ML = 0 */
			hx8347d_WriteReg(LCD_REG_22, 0x08);
			hx8347d_WriteReg(LCD_REG_4,  0x00);
			hx8347d_WriteReg(LCD_REG_5,  0xEF);
			hx8347d_WriteReg(LCD_REG_8,  0x01);
			hx8347d_WriteReg(LCD_REG_9,  0x3F);
			_height = HX8347D_LCD_PIXEL_WIDTH + 2;
			_width = HX8347D_LCD_PIXEL_HEIGHT + 2;
			break;
		case 1:
			/* Memory access control: MY = 1, MX = 1, MV = 0, ML = 1 */
			hx8347d_WriteReg(LCD_REG_22, 0x68);
			hx8347d_WriteReg(LCD_REG_4,  0x01);
			hx8347d_WriteReg(LCD_REG_5,  0x3F);
			hx8347d_WriteReg(LCD_REG_8,  0x00);
			hx8347d_WriteReg(LCD_REG_9,  0xEF);
			_height = HX8347D_LCD_PIXEL_HEIGHT;
			_width = HX8347D_LCD_PIXEL_WIDTH;
			break;
		case 2:
			/* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
			hx8347d_WriteReg(LCD_REG_22, 0xC8);
			hx8347d_WriteReg(LCD_REG_4,  0x00);
			hx8347d_WriteReg(LCD_REG_5,  0xEF);
			hx8347d_WriteReg(LCD_REG_8,  0x01);
			hx8347d_WriteReg(LCD_REG_9,  0x3F);
			_height = HX8347D_LCD_PIXEL_WIDTH;
			_width = HX8347D_LCD_PIXEL_HEIGHT;
			break;
		case 3:
			/* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
			hx8347d_WriteReg(LCD_REG_22, 0xA8);
			hx8347d_WriteReg(LCD_REG_4,  0x01);
			hx8347d_WriteReg(LCD_REG_5,  0x3F);
			hx8347d_WriteReg(LCD_REG_8,  0x00);
			hx8347d_WriteReg(LCD_REG_9,  0xEF);
			_height = HX8347D_LCD_PIXEL_HEIGHT;
			_width = HX8347D_LCD_PIXEL_WIDTH;
			break;
	}
		
}

/**
* @brief  Draw vertical line.
* @param  RGBCode: Specifies the RGB color   
* @param  Xpos:     specifies the X position.
* @param  Ypos:     specifies the Y position.
* @param  Length:   specifies the Line length.  
* @retval None
*/
void hx8347d_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
	uint16_t i,x1 = MIN((Xpos + Length),(_width - 1));
	if(Xpos >= _width || Ypos >= _height){
		return;
	}

	if((_rotation == 0) || (_rotation == 2)){
		for(i = Xpos; i <= x1; i++){
			hx8347d_WritePixel(Ypos,i,RGBCode);
		}
	}else{
		for(i = Xpos; i <= x1; i++){
			hx8347d_WritePixel(i,Ypos,RGBCode);
		}
	}
}

/**
* @brief  Draw vertical line.
* @param  RGBCode: Specifies the RGB color    
* @param  Xpos:     specifies the X position.
* @param  Ypos:     specifies the Y position.
* @param  Length:   specifies the Line length.  
* @retval None
*/
void hx8347d_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{ 
	uint16_t i,y1 = MIN((Ypos + Length),(_height - 1));
	
	if(Xpos >= _width || Ypos >= _height){
		return;
	}
	if((_rotation == 0) || (_rotation == 2)){
		for(i = Ypos; i <= y1; i++){
			hx8347d_WritePixel(i,Xpos,RGBCode);
		}
	}else{
		for(i = Ypos; i <= y1; i++){
			hx8347d_WritePixel(Xpos,i,RGBCode);
		}
	}

}

/**
* @brief  Displays a bitmap picture loaded in the internal Flash.
* @param  BmpAddress: Bmp picture address in the internal Flash.
* @retval None
*/
void hx8347d_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
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
  /* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 */
  hx8347d_WriteReg(LCD_REG_22, 0xC0);
	
  /* Set Cursor */
  hx8347d_SetCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  LCD_IO_WriteMultipleData((uint8_t*)pbmp, size*2);
  
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 1, MX = 1, MV = 1, ML = 0 */
	switch(_rotation){
		case 0:
			hx8347d_WriteReg(LCD_REG_22, 0x08);
			break;
		case 1:
			hx8347d_WriteReg(LCD_REG_22, 0x68);
			break;
		case 2:
			hx8347d_WriteReg(LCD_REG_22, 0xC8);
			break;
		case 3:
			hx8347d_WriteReg(LCD_REG_22, 0xA8);
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

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
