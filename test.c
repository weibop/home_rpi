#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include "font.h"
#include "hal_spi.h"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


#define GPIO26 RPI_BPLUS_GPIO_J8_37
#define PIN_LCD_LIGHT GPIO26
#define PIN_LCD_RESET RPI_BPLUS_GPIO_J8_33

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bit = 8;
static uint32_t speed = 500000;
static uint16_t delay;

int main(int argc, char*argv[]){
	int fd;
	int ret = 0;
	printf("Hello World!\n");

	//pabort("bye!\n");
	fd = open(device, O_RDWR);
	if(fd < 0)
		pabort("cannot open device");

	//spi mode
	ret = ioctl(fd,SPI_IOC_WR_MODE,&mode);
	if (ret == -1)
		pabort("can't set spi mode");

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
