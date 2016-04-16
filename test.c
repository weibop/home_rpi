#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include "font.h"
#include "hal_spi.h"
#include "lcd.h"
#include <unistd.h>


static uint8_t mode;
static uint8_t bit = 8;
static uint32_t speed = 500000;
static uint16_t delay;

unsigned char text[15] = "Hello World!  ";

int main(int argc, char*argv[]){
	unsigned int i;
	unsigned char c;

	printf("\nInitializing bcm2835...");
	if (!bcm2835_init()){
		printf(" *** Error: *** bcm2835 initialization failed ***");
	}

	SPI_Init(8, 1000000, 0);
	LCD_Init();
	LCD_Light(1);
	bcm2835_delay(500);
	LCD_Light(0);
	bcm2835_delay(500);
	LCD_Light(1);

	while (1)
	{
		c = text[0];
		for (i=0; i<13; i++) text[i] = text[i+1];
		text[13] = c;
		printf("\n%s", text);
		bcm2835_delay(1000);
		LCD_Clear_Memory();
  		LCD_Write(text, 14);
  		LCD_Update(1, 0);
	}
}
