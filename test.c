#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>

#define GPIO26 RPI_BPLUS_GPIO_J8_37
#define PIN_LCD_LIGHT GPIO26
#define PIN_LCD_RESET RPI_BPLUS_GPIO_J8_33

int main(int argc, char*argv[]){
	printf("Hello World!\n");

	printf("Initializing bcm2835...\n");
	if (!bcm2835_init()){
		printf(" *** Error: *** bcm2835 initialization failed ***");
	}

	bcm2835_gpio_fsel(PIN_LCD_RESET, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LCD_RESET, HIGH);


	bcm2835_gpio_fsel(PIN_LCD_LIGHT, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LCD_LIGHT, LOW);

	while(1){
		//bcm2835_delay(500);
		delay(1000);
		bcm2835_gpio_write(PIN_LCD_LIGHT, HIGH);
		delay(1000);
		bcm2835_gpio_write(PIN_LCD_LIGHT, LOW);
	}
}
