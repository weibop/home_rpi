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

unsigned char text1[15] = "Hello World!  ";
unsigned char text2[15] = "30            ";
unsigned char text3[15] = "25            ";

int main(int argc, char*argv[]){
	unsigned int i;
	unsigned char c;
	float tempC;

	printf("\nInitializing bcm2835...");
	if (!bcm2835_init()){
		printf(" *** Error: *** bcm2835 initialization failed ***");
	}

	if(Ds18b20_Init())
		printf(" *** Error: *** ds18b20 initialization failed ***");

	SPI_Init(8, 1000000, 0);
	LCD_Init();
	LCD_Light(1);
	bcm2835_delay(500);
	LCD_Light(0);
	bcm2835_delay(500);
	LCD_Light(1);


	while (1)
	{
		c = text1[0];
		for (i=0; i<13; i++) text1[i] = text1[i+1];
		text1[13] = c;
		printf("\n%s", text1);
		bcm2835_delay(1000);

		Ds18b20_Read(&tempC);
		//sprintf(text3,"%.3f C", tempC/1000);
		//printf("\n%s", text3);
		LCD_Clear_Memory();
  		LCD_Write(text1, 14);
  		//LCD_Write(text2, 14);
  		//LCD_Write(text3, 14);
  		LCD_Update(1, 0);
	}
}
