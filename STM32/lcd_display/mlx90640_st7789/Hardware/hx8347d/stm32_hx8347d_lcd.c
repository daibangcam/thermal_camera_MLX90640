/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
*/

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - The LCD hx8347d component driver MUST be included with this driver.  

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the LCD using the BSP_LCD_Init() function.
  
  + Display on LCD
     o Clear the whole LCD using the BSP_LCD_Clear() function or only one specified 
       string line using the BSP_LCD_ClearStringLine() function.
     o Display a character on the specified line and column using the BSP_LCD_DisplayChar()
       function or a complete string line using the BSP_LCD_DisplayStringAtLine() function.
     o Display a string line on the specified position (x,y in pixel) and align mode
       using the BSP_LCD_DisplayStringAtLine() function.          
     o Draw and fill a basic shapes (dot, line, rectangle, circle, ellipse, ..) 
       on LCD using a set of functions.    
 
------------------------------------------------------------------------------*/

/* Dependencies
- hx8347d.c
- fonts.h
- font24.c
- font20.c
- font16.c
- font12.c
- font8.c"
EndDependencies */
    
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "main.h"
#include "spi.h"
#include "stm32_hx8347d_lcd.h"
#include "Fonts/fonts.h"
#include "Fonts/font24.c"
#include "Fonts/font20.c"
#include "Fonts/font16.c"
#include "Fonts/font12.c"
#include "Fonts/font8.c"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32_HX8347D
  * @{
  */
    
/** @addtogroup STM32_HX8347D_LCD
  * @{
  */ 

/** @defgroup STM32_HX8347D_LCD_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup STM32_HX8347D_LCD_Private_Defines
  * @{
  */
#define POLY_X(Z)             ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)             ((int32_t)((Points + (Z))->Y))
//#define NULL                  (void *)0

#define MAX_HEIGHT_FONT         24
#define MAX_WIDTH_FONT          17
#define OFFSET_BITMAP           54
/**
  * @}
  */ 

/** @defgroup STM32_HX8347D_LCD_Private_Macros
  * @{
  */
#define ABS(X) ((X) > 0 ? (X) : -(X)) 

/**
  * @}
  */ 
    
/** @defgroup STM32_HX8347D_LCD_Private_Variables
  * @{
  */ 
LCD_DrawPropTypeDef DrawProp;

static LCD_DrvTypeDef  *lcd_drv; 

/* Max size of bitmap will based on a font24 (17x24) */
static uint8_t bitmap[MAX_HEIGHT_FONT*MAX_WIDTH_FONT*2+OFFSET_BITMAP] = {0};

/*  Rotation  */
static uint8_t _rotation = 0;

/**
  * @}
  */ 

/** @defgroup STM32_HX8347D_LCD_Private_FunctionPrototypes
  * @{
  */ 
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c, uint16_t RGBCode);
static void SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**
  * @}
  */ 
//=========================================================1=============================================================//
/** @defgroup STM32F1XX_NUCLEO_Private_Functions STM32F1XX NUCLEO Private Functions
  * @{
  */ 
static void  SPIx_Init(void);
static uint16_t SPIx_Read(uint8_t Value);
static void  SPIx_Write(uint8_t Value);
static void  SPIx_Error (void);
static void  SPIx_MspInit(void);


uint32_t SpixTimeout = NUCLEO_SPIx_TIMEOUT_MAX;  

//=========================================================2=============================================================//
/** @defgroup STM32_HX8347D_LCD_Private_Functions
  * @{
  */
  
/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{ 
  uint8_t ret = LCD_ERROR;
  
  /* Default value for draw propriety */
  DrawProp.BackColor = 0xFFFF;
  DrawProp.pFont     = &Font24;
  DrawProp.TextColor = 0x0000;
  
//  lcd_drv = &hx8347d_drv;
	lcd_drv = &st7789v_drv;
	
	LCD_IO_Init();
  /* LCD Init */   
  lcd_drv->Init();
  
	BSP_LCD_SetRotation(0);
	LCD_BL_ON();
  /* Clear the LCD screen */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Initialize the font */
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  ret = LCD_OK;
  
  return ret;
}

/**
  * @brief  Gets the LCD X size.
  * @param  None    
  * @retval Used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return(lcd_drv->GetLcdPixelWidth());
}

/**
  * @brief  Gets the LCD Y size.
  * @param  None   
  * @retval Used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return(lcd_drv->GetLcdPixelHeight());
}

/**
  * @brief  Gets the LCD text color.
  * @param  None 
  * @retval Used text color.
  */
uint16_t BSP_LCD_GetTextColor(void)
{
  return DrawProp.TextColor;
}

/**
  * @brief  Gets the LCD background color.
  * @param  None
  * @retval Used background color
  */
uint16_t BSP_LCD_GetBackColor(void)
{
  return DrawProp.BackColor;
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code RGB(5-6-5)
  * @retval None
  */
void BSP_LCD_SetTextColor(uint16_t Color)
{
  DrawProp.TextColor = Color;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color: Background color code RGB(5-6-5)
  * @retval None
  */
void BSP_LCD_SetBackColor(uint16_t Color)
{
  DrawProp.BackColor = Color;
}

/**
* @brief  Set the Rotation.
* @param  rotation: 0(0бу), 1(90бу), 2(180бу), 3(270бу)
* @retval None
*/
void BSP_LCD_SetRotation(uint8_t rotation)
{
	if(lcd_drv->SetRotation != NULL)
  {
		_rotation = rotation%4;
    lcd_drv->SetRotation(rotation);
  }
}
/**
  * @brief  Sets the LCD text font.
  * @param  fonts: Font to be used
  * @retval None
  */
void BSP_LCD_SetFont(sFONT *pFonts)
{
  DrawProp.pFont = pFonts;
}

/**
  * @brief  Gets the LCD text font.
  * @param  None
  * @retval Used font
  */
sFONT *BSP_LCD_GetFont(void)
{
  return DrawProp.pFont;
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: Color of the background
  * @retval None
  */
void BSP_LCD_Clear(uint16_t Color)
{ 
  uint32_t counter = 0;
	
	if(_rotation == 0 || _rotation == 2){
		
		for(counter = 0; counter < BSP_LCD_GetYSize(); counter++)
		{
			BSP_LCD_DrawHLine(0, counter, BSP_LCD_GetXSize(),Color);
		}
	}else{
		
		for(counter = 0; counter < BSP_LCD_GetXSize(); counter++)
		{
			BSP_LCD_DrawHLine(0, counter, BSP_LCD_GetYSize(),Color);
		}
	}

}

/**
  * @brief  Clears the selected line.
  * @param  Line: Line to be cleared
  *          This parameter can be one of the following values:
  *            @arg  0..9: if the Current fonts is Font16x24
  *            @arg  0..19: if the Current fonts is Font12x12 or Font8x12
  *            @arg  0..29: if the Current fonts is Font8x8
  * @retval None
  */
void BSP_LCD_ClearStringLine(uint16_t Line)
{ 
  uint32_t color_backup = DrawProp.TextColor; 
  DrawProp.TextColor = DrawProp.BackColor;;
    
  /* Draw a rectangle with background color */
  BSP_LCD_FillRect(0, (Line * DrawProp.pFont->Height), BSP_LCD_GetXSize(), DrawProp.pFont->Height,DrawProp.TextColor);
  
  DrawProp.TextColor = color_backup;
  BSP_LCD_SetTextColor(DrawProp.TextColor);
}

/**
  * @brief  Displays one character.
  * @param  Xpos: Start column address
  * @param  Ypos: Line where to display the character shape.
  * @param  Ascii: Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E 
  * @retval None
  */
void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii, uint16_t RGBCode)
{
  DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') *\
    DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)], RGBCode);
}

/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)   
  * @param  Text: Pointer to string to display on LCD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE   
  * @retval None
  */
void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Line_ModeTypdef Mode, uint16_t RGBCode)
{
  uint16_t refcolumn = 1, i = 0;
  uint32_t size = 0, xsize = 0; 
  uint8_t  *ptr = Text;
  
  /* Get the text size */
  while (*ptr++) size ++ ;
  
  /* Characters number per line */
  xsize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);
  
  switch (Mode)
  {
		case CENTER_MODE:
			{
				refcolumn = Xpos + ((xsize - size)* DrawProp.pFont->Width) / 2;
				break;
			}
		case LEFT_MODE:
			{
				refcolumn = Xpos;
				break;
			}
		case RIGHT_MODE:
			{
				refcolumn =  - Xpos + ((xsize - size)*DrawProp.pFont->Width);
				break;
			}    
		default:
			{
				refcolumn = Xpos;
				break;
			}
  }
  
  /* Send the string character by character on lCD */
  while ((*Text != 0) & (((BSP_LCD_GetXSize() - (i*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
  {
    /* Display one character on LCD */
    BSP_LCD_DisplayChar(refcolumn, (BSP_LCD_GetYSize() - Ypos), *Text, RGBCode);
    /* Decrement the column position by 16 */
    refcolumn += DrawProp.pFont->Width;
    /* Point on the next character */
    Text++;
    i++;
  }
}

/**
  * @brief  Displays a character on the LCD.
  * @param  Line: Line where to display the character shape
  *          This parameter can be one of the following values:
  *            @arg  0..19: if the Current fonts is Font8
  *            @arg  0..12: if the Current fonts is Font12
  *            @arg  0...9: if the Current fonts is Font16
  *            @arg  0...7: if the Current fonts is Font20
  *            @arg  0...5: if the Current fonts is Font24
  * @param  ptr: Pointer to string to display on LCD
  * @retval None
  */
void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr, uint16_t RGBCode)
{
  BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE, RGBCode);
}

/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos: X position 
  * @param  Ypos: Y position
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)  
  * @retval None
  */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code)
{
  if(lcd_drv->WritePixel != NULL)
  {
    lcd_drv->WritePixel(Xpos, Ypos, RGB_Code);
  }
}
  
/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length,uint16_t RGBCode)
{
  lcd_drv->DrawHLine(RGBCode, Xpos, Ypos, Length);
}

/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length,uint16_t RGBCode)
{

  lcd_drv->DrawVLine(RGBCode, Xpos, Ypos, Length);

}

/**
  * @brief  Draws an uni-line (between two points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @retval None
  */
void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t RGBCode)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawPixel(x, y, RGBCode);  /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }
}

/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width  
  * @param  Height: Rectangle height
  * @retval None
  */
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode)
{
  /* Draw horizontal lines */
  BSP_LCD_DrawHLine(Xpos, Ypos, Width, RGBCode);
  BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width, RGBCode);
  
  /* Draw vertical lines */
  BSP_LCD_DrawVLine(Xpos, Ypos, Height, RGBCode);
  BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height, RGBCode);
}
                            
/**
  * @brief  Draws a circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t RGBCode)
{
  int32_t  D;       /* Decision Variable */ 
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos - CurY), RGBCode);

    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos - CurY), RGBCode);

    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos - CurX), RGBCode);

    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos - CurX), RGBCode);

    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos + CurY), RGBCode);

    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos + CurY), RGBCode);

    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos + CurX), RGBCode);

    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos + CurX), RGBCode);   

    /* Initialize the font */
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  } 
}

/**
  * @brief  Draws an poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount, uint16_t RGBCode)
{
  int16_t X = 0, Y = 0;

  if(PointCount < 2)
  {
    return;
  }

  BSP_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y, RGBCode);
  
  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    BSP_LCD_DrawLine(X, Y, Points->X, Points->Y, RGBCode);
  }
}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);
  
  do {      
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);      
    
    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;     
  }
  while (y <= 0);
}

/**
  * @brief  Draws a bitmap picture loaded in the STM32 MCU internal memory.
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pBmp: Pointer to Bmp picture address
  * @retval None
  */
void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp)
{
  uint32_t height = 0;
  uint32_t width  = 0;
  
  /* Read bitmap width */
  width = pBmp[18] + (pBmp[19] << 8) + (pBmp[20] << 16)  + (pBmp[21] << 24);

  /* Read bitmap height */
  height = pBmp[22] + (pBmp[23] << 8) + (pBmp[24] << 16)  + (pBmp[25] << 24);
  
  /* Remap Ypos, st7735 works with inverted X in case of bitmap */
  /* X = 0, cursor is on Top corner */
  
  SetDisplayWindow(Xpos, Ypos, width, height);
  
  if(lcd_drv->DrawBitmap != NULL)
  {
    lcd_drv->DrawBitmap(Xpos, Ypos, pBmp);
  } 
	SetDisplayWindow(0, 0, BSP_LCD_GetYSize(), BSP_LCD_GetXSize());
}

/**
  * @brief  Draws a full rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width  
  * @param  Height: Rectangle height
  * @retval None
  */
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t RGBCode)
{
	if(lcd_drv->FillRect != NULL)
  {
    lcd_drv->FillRect(Xpos, Ypos, Width, Height, RGBCode);
  } 
	
	SetDisplayWindow(0, 0, BSP_LCD_GetYSize(), BSP_LCD_GetXSize());
}

/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t RGBCode)
{
  int32_t  D;        /* Decision Variable */ 
  uint32_t  CurX;    /* Current X Value */
  uint32_t  CurY;    /* Current Y Value */ 
  
  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;
  
  BSP_LCD_SetTextColor(DrawProp.TextColor);

  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      BSP_LCD_DrawHLine(Xpos - CurY, Ypos + CurX, 2*CurY, RGBCode);
      BSP_LCD_DrawHLine(Xpos - CurY, Ypos - CurX, 2*CurY, RGBCode);
    }

    if(CurX > 0) 
    {
      BSP_LCD_DrawHLine(Xpos - CurX, Ypos - CurY, 2*CurX, RGBCode);
      BSP_LCD_DrawHLine(Xpos - CurX, Ypos + CurY, 2*CurX, RGBCode);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  BSP_LCD_SetTextColor(DrawProp.TextColor);
	if((_rotation == 0) || (_rotation == 2)){
		BSP_LCD_DrawCircle(Ypos, Xpos, Radius, RGBCode);
	}else{
		BSP_LCD_DrawCircle(Xpos, Ypos, Radius, RGBCode);
	}
  
}

/**
  * @brief  Draws a full poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount, uint16_t RGBCode)
{
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;  
  
  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;
  
  for(counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }
    
    pixelY = POLY_Y(counter);
    if(pixelY < IMAGE_TOP)
    {
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }  
  
  if(PointCount < 2)
  {
    return;
  }
  
  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;
  
  X_first = Points->X;
  Y_first = Points->Y;
  
  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;    
    
    BSP_LCD_FillTriangle(X, X2, X_center, Y, Y2, Y_center, RGBCode);
    BSP_LCD_FillTriangle(X, X_center, X2, Y, Y_center, Y2, RGBCode);
    BSP_LCD_FillTriangle(X_center, X2, X, Y_center, Y2, Y, RGBCode);   
  }
  
  BSP_LCD_FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center, RGBCode);
  BSP_LCD_FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2, RGBCode);
  BSP_LCD_FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first, RGBCode);   
}

/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius  
  * @retval None
  */
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint16_t RGBCode)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);    
  
  do 
  { 
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), RGBCode);
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), RGBCode);
    
    e2 = err;
    if (e2 <= x) 
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}

/**
  * @brief  Fills a triangle (between 3 points).
  * @param  Points: Pointer to the points array
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @param  x3: Point 3 X position
  * @param  y3: Point 3 Y position
  * @retval None
  */
void BSP_LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3, uint16_t RGBCode)
{ 
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawLine(x, y, x3, y3, RGBCode);
    
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  } 
}

/**
  * @brief  Enables the display.
  * @param  None
  * @retval None
  */
void BSP_LCD_DisplayOn(void)
{
  lcd_drv->DisplayOn();
}

/**
  * @brief  Disables the display.
  * @param  None
  * @retval None
  */
void BSP_LCD_DisplayOff(void)
{
  lcd_drv->DisplayOff();
}


uint16_t BSP_LCD_GetColor565(uint8_t red, uint8_t green, uint8_t blue){
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue) >> 3);
}
/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: Line where to display the character shape
  * @param  Ypos: Start column address
  * @param  pChar: Pointer to the character data
  * @retval None
  */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *pChar, uint16_t RGBCode)
{
  uint32_t counterh = 0, counterw = 0, index = 0;
  uint16_t height = 0, width = 0;
  uint8_t offset = 0;
  uint8_t *pchar = NULL;
  uint32_t line = 0;
  
  height = DrawProp.pFont->Height;
  width  = DrawProp.pFont->Width;
  
  /* Fill bitmap header*/
  *(uint16_t *) (bitmap + 2) = (uint16_t)(height*width*2+OFFSET_BITMAP);
  *(uint16_t *) (bitmap + 4) = (uint16_t)((height*width*2+OFFSET_BITMAP)>>16);
  *(uint16_t *) (bitmap + 10) = OFFSET_BITMAP;
  *(uint16_t *) (bitmap + 18) = (uint16_t)(width);
  *(uint16_t *) (bitmap + 20) = (uint16_t)((width)>>16);
  *(uint16_t *) (bitmap + 22) = (uint16_t)(height);
  *(uint16_t *) (bitmap + 24) = (uint16_t)((height)>>16);
  
  offset =  8 *((width + 7)/8) - width ;
  
  for(counterh = 0; counterh < height; counterh++)
  {
    pchar = ((uint8_t *)pChar + (width + 7)/8 * counterh);
    
    if(((width + 7)/8) == 3)
    {
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
    }
    
    if(((width + 7)/8) == 2)
    {
      line =  (pchar[0]<< 8) | pchar[1];
    }
    
    if(((width + 7)/8) == 1)
    {
      line =  pchar[0];
    }    
    
    for (counterw = 0; counterw < width; counterw++)
    {
      /* Image in the bitmap is written from the bottom to the top */
      /* Need to invert image in the bitmap */
      index = (((height-counterh-1)*width)+(counterw))*2+OFFSET_BITMAP;
      if(line & (1 << (width- counterw + offset- 1))) 
      {
        bitmap[index] = (uint8_t)RGBCode;
        bitmap[index+1] = (uint8_t)(RGBCode >> 8);
      }
      else
      {
        bitmap[index] = (uint8_t)DrawProp.BackColor;
        bitmap[index+1] = (uint8_t)(DrawProp.BackColor >> 8);
      } 
    }
  }
  // ST7789v
  BSP_LCD_DrawBitmap(Xpos, Ypos, bitmap);
}

/**
  * @brief  Sets display window.
  * @param  LayerIndex: layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height  
  * @retval None
  */
static void SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  if(lcd_drv->SetDisplayWindow != NULL)
  {
    lcd_drv->SetDisplayWindow(Xpos, Ypos, Width, Height);
  }  
}

/**
  * @}
  */


//=========================================================1=============================================================//

/**
  * @brief  Initialize SPI MSP.
  */
static void SPIx_MspInit(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  LCD_SPIx_SCK_GPIO_CLK_ENABLE();
  LCD_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI SCK */
  gpioinitstruct.Pin        = LCD_SPIx_SCK_PIN;
  gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
  gpioinitstruct.Speed      = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_SPIx_SCK_GPIO_PORT, &gpioinitstruct);

  /* Configure SPI MISO and MOSI */ 
  gpioinitstruct.Pin        = LCD_SPIx_MOSI_PIN;
  HAL_GPIO_Init(LCD_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);
  
  gpioinitstruct.Pin        = LCD_SPIx_MISO_PIN;
  gpioinitstruct.Mode       = GPIO_MODE_INPUT;
  HAL_GPIO_Init(LCD_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  LCD_SPIx_CLK_ENABLE();
}
/**
  * @brief  Initialize SPI HAL.
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hspi1.Instance = SPI1;
      /* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 64/8 = 8 MHz) 
       to verify these constraints:
          - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint 
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 32 MHz 
       */
    hspi1.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.Direction          = SPI_DIRECTION_2LINES;
    hspi1.Init.CLKPhase           = SPI_PHASE_1EDGE;
    hspi1.Init.CLKPolarity        = SPI_POLARITY_LOW;
    hspi1.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial      = 7;
    hspi1.Init.DataSize           = SPI_DATASIZE_8BIT;
    hspi1.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    hspi1.Init.NSS                = SPI_NSS_SOFT;
    hspi1.Init.TIMode             = SPI_TIMODE_DISABLE;
    hspi1.Init.Mode               = SPI_MODE_MASTER;
    
    SPIx_MspInit();
    HAL_SPI_Init(&hspi1);
  }
}

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t data;

  status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &Value, &data, 1, SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval data
  */
static uint16_t SPIx_Read(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t data;

  status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &Value, &data, 1, SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
		return HAL_ERROR;
  }else{
		return data;
	}
	
}
/**
  * @brief  SPI error treatment function
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hspi1);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

/**
  * @}
  */ 
 
/********************************* LINK LCD ***********************************/
/**
  * @brief  Initialize the LCD
  */
void LCD_IO_Init(void)
{
	
  GPIO_InitTypeDef  gpioinitstruct;

  /* LCD_CS_GPIO and LCD_DC_GPIO Periph clock enable */
  LCD_CS_GPIO_CLK_ENABLE();
  LCD_DC_GPIO_CLK_ENABLE();
  
  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
  gpioinitstruct.Pin    = LCD_CS_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &gpioinitstruct);
      
  /* Configure LCD_DC_PIN pin: LCD Card DC pin */
  gpioinitstruct.Pin    = LCD_DC_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpioinitstruct);

  /* LCD chip select high */
  LCD_CS_HIGH();
  
  /* LCD SPI Config */
  //SPIx_Init();
}

/**
  * @brief  Write command to select the LCD register.
  * @param  LCDReg: Address of the selected register.
  * @retval None
  */
void LCD_IO_WriteReg(uint8_t LCDReg)
{
  /* Reset LCD control line CS */
  LCD_CS_LOW();
  
  /* Set LCD data/command line DC to Low */
  LCD_DC_LOW();
    
  /* Send Command */
  SPIx_Write(LCDReg);
  
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
  * @brief  Read command to select the LCD register.
  * @param  LCDReg: Address of the selected register.
  * @retval data
  */
uint16_t LCD_IO_ReadData(uint16_t LCDReg){
	  /* Reset LCD control line CS */
  LCD_CS_LOW();
  
  /* Set LCD data/command line DC to Low */
  LCD_DC_LOW();
    
  /* Send Command */
  uint16_t data = SPIx_Read(LCDReg);
  
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
	
	return data;
}
/**
* @brief  Write register value.
* @param  pData Pointer on the register value
* @param  Size Size of byte to transmit to the register
*/
void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size)
{
  uint32_t counter = 0;
  
  /* Reset LCD control line CS */
  LCD_CS_LOW();
  
  /* Set LCD data/command line DC to High */
  LCD_DC_HIGH();

  if (Size == 1)
  {
    /* Only 1 byte to be sent to LCD - general interface can be used */
    /* Send Data */
    SPIx_Write(*pData);
  }
  else
	{
    /* Several data should be sent in a raw */
    /* Direct SPI accesses for optimization */
    for (counter = Size; counter != 0; counter--)
		{
      while(((hspi1.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
			{
			}
      /* Need to invert bytes for LCD*/
      *((__IO uint8_t*)&hspi1.Instance->DR) = *(pData+1);

      while(((hspi1.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
			{
			}
      *((__IO uint8_t*)&hspi1.Instance->DR) = *pData;
      counter--;
      pData += 2;
		}  
  
    /* Wait until the bus is ready before releasing Chip select */ 
    while(((hspi1.Instance->SR) & SPI_FLAG_BSY) != RESET)
		{
		} 
  } 
  
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  * @retval None
  */
void LCD_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}
//=========================================================2=============================================================//
/**
  * @}
  */     

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
