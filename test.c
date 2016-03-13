#include <stdio.h>
#include <bcm2835.h>

#define PIN_LCD_LIGHT RPI_GPIO_P1_26

int main(int argc, char*argv[]){
	printf("Hello World!\n");

	printf("Initializing bcm2835...\n");
	if (!bcm2835_init()){
		printf(" *** Error: *** bcm2835 initialization failed ***");
	}

	bcm2835_gpio_fsel(PIN_PI_RDY, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_PI_RDY, LOW);
	
	while(1){
		bcm2835_delay(1000);	
		bcm2835_gpio_write(PIN_LCD_LIGHT, HIGH);
		bcm2835_delay(1000);
		bcm2835_gpio_write(PIN_LCD_LIGHT, LOW);
	}
}
