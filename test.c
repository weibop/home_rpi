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

unsigned char text[22] = "www.olimex.com/dev    ";

int main(int argc, char*argv[]){
    unsigned int i;
    unsigned char c;
	printf("Hello World!\n");

	printf("Initializing bcm2835...\n");
	if (!bcm2835_init()){
		printf(" *** Error: *** bcm2835 initialization failed ***");
	}

	{
		//bcm2835_delay(500);
		delay(1000);
		LCD_Light(1);
		delay(1000);
		LCD_Light(0);
	}
    SPI2_Init(0, 1000000, 8);
    LCD_Init();



    while (1)
    {
	    c = text[0];
	    for (i=0; i<22; i++) text[i] = text[i+1];
    	text[21] = c;
	    LCD_Clear_Memory();
  	    LCD_Write(text, 14);
  	    LCD_Update(1, 0);
	    for (i=30000; i; i--);
   }
}
