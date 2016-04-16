
#include "hal_spi.h"
#include "font.h"
#include <bcm2835.h>
#include "lcd.h"

//LCD Functions +


static unsigned char LCDMemory[504], inverse=1;
static int index=0;


void LCD_Init()
{
	
	bcm2835_gpio_fsel(PIN_LCD_RESET, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LCD_RESET, LOW);	
	bcm2835_gpio_fsel(PIN_LCD_CS, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LCD_CS, HIGH);
	bcm2835_gpio_fsel(PIN_LCD_DC, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LCD_DC, HIGH);
	bcm2835_gpio_fsel(PIN_LCD_LIGHT, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LCD_LIGHT, LOW);
	
	bcm2835_delay(1);
	bcm2835_gpio_write(PIN_LCD_RESET, HIGH);	
	
  //P9OUT = P9OUT & 0xBF;         //LCD_RES = 0   //~(1<<6)
  //Delay(1000);
  //P9OUT = P9OUT | 0x40;         //LCD_RES = 1   //1<<6
  //Delay(1000);

  //from Dimitar's PIC source +
	//Send sequence of command +
	  LCD_Send(0x21, SEND_CMD);  // LCD Extended Commands.
	  //Extended commands +
		LCD_Send(0xC8, SEND_CMD);  // Set Contrast.
		LCD_Send(0x04 | !!(LCD_START_LINE_ADDR&(0x40)), SEND_CMD);      // Set Temp S6 for start line
		LCD_Send(0x40 | (LCD_START_LINE_ADDR & ((0x40)-1)), SEND_CMD);  // Set Temp S[5:0] for start line
		LCD_Send(0x12, SEND_CMD);  // LCD bias mode 1:68.
	  //Extended commands -
	  LCD_Send(0x20, SEND_CMD);  // LCD Standard Commands, Horizontal addressing mode.
	  LCD_Send(0x08, SEND_CMD);  // LCD blank
	  LCD_Send(0x0C, SEND_CMD);  // LCD in normal mode.
	//Send sequence of command -
  //from Dimitar's PIC source -

  //Optional commands +
	LCD_Contrast(0xFF);       //Increase contrast
	LCD_Clear();              //Clear Screen
	LCD_Set_Position(0, 0);   //0, 0 coordinates of the display
  //Optional Commands -
}

void LCD_Send(unsigned char data, unsigned char cd)
{
  bcm2835_gpio_write(PIN_LCD_CS, LOW);
	bcm2835_gpio_write(PIN_LCD_DC, HIGH);
  //P9OUT = P9OUT & ~(0x01);

  if (cd == SEND_CHR)         //check for sending char/command
	bcm2835_gpio_write(PIN_LCD_DC, HIGH);
	//P9OUT = P9OUT | 0x80;     //sending char                    //1<<7
  else
	bcm2835_gpio_write(PIN_LCD_DC, LOW);
	//P9OUT = P9OUT & 0x7F;     //sending command                 //~(1<<7)

  SPI2_Transmit(data);        //transmitting char/command

  bcm2835_gpio_write(PIN_LCD_CS, HIGH);
  //P9OUT = P9OUT | 0x01;
}

void LCD_Clear_Memory()
{
  for (index=0; index<504; index++)
	LCDMemory[index] = 0;
  index = 0;
}

/*
  mode indicates inverse display:
  (shows black pixels as white and white pixels as black)

  the 1 in binary num indicates which of the lines
  must/must not be inverted
  ex. num = 13 --> 0b001101 --> lines 0, 2, 3 will be inverted
*/
void LCD_Update(unsigned char mode, unsigned char num)
{
  char x, y;
  for(y=0; y<48/8; y++)
  {
	LCD_Send(0x80, SEND_CMD);     //x - home
	LCD_Send(0x40|y, SEND_CMD);   //y - next line

	if (((mode==inverse) && !(num & 1<<y)) || ((mode != inverse) && (num & 1<<y)))
	  for(x=0; x<84; x++)
		LCD_Send(LCDMemory[y*84+x], SEND_CHR);
	else
	  for(x=0; x<84; x++)
		LCD_Send(~LCDMemory[y*84+x], SEND_CHR);
  }
}

void LCD_Clear()
{
  LCD_Clear_Memory();
  LCD_Update(1, 0x00);
}

void LCD_Contrast(unsigned char contrast)
{
	LCD_Send(0x21, SEND_CMD);
	//LCD Extended Commands +
	  LCD_Send(0x80 | contrast, SEND_CMD);  // Set LCD Vop (Contrast).
	//LCD Extended Commands -
	LCD_Send(0x20, SEND_CMD);         //LCD Standard Commands, horizontal addressing mode
}

void LCD_Set_Position(unsigned char x, unsigned int y)
{
  //Optional calculations +
	//in case the position is out of the screen:
	//if X > 83 it goes to the next (or more) line.
	//if Y > 5 it return to the 0, 1, 2... line
	  y = y + x/84;
	  y = y % 6;
	  x = x % 84;
  //Optional calculations -
  index = 84*y + x;

  //send cursor to x, y coordinates +
	LCD_Send(0x80 | x, SEND_CMD);
	LCD_Send(0x40 | y, SEND_CMD);
  //send cursor to x, y coordinates -
}

void LCD_Write(unsigned char *a, int l)
{
  unsigned char i;
  while (*a && l)
  {
   l--;
	for (i=0; i<5; i++)
	  LCDMemory[index++] = FontLookup[*a-32][i];
	LCDMemory[index++] = 0x00;    //space between signs
	a++;
  }
}
void LCD_Light(int enable)
{
	if(enable)
		bcm2835_gpio_write(PIN_LCD_LIGHT, HIGH);
	else
		bcm2835_gpio_write(PIN_LCD_LIGHT, LOW);	
}