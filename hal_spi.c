#include "hal_spi.h"
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include "nazsys.h"
#include <fcntl.h>
#include <stdio.h>

static const char *device = "/dev/spidev0.0";
static struct spi_ioc_transfer tr_data;
static unsigned char mode;
//static uint8_t bits = 8;
//static uint32_t speed = 500000;
//static uint16_t delay;
unsigned char spi_tx[1]; 
unsigned char spi_rx[1];
int fd;

void SPI_Init(int bits, int speed, int delay){	
	int ret = 0;
	
	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");
 
	/*
	 * spi mode
	 */
	mode = SPI_CPOL;
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");
 
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");
 
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");
 
	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");
 
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
 
	tr_data.tx_buf = (unsigned long)spi_tx;
	tr_data.rx_buf = (unsigned long)spi_rx;
	tr_data.len = 1;
	tr_data.delay_usecs = delay;
	tr_data.speed_hz = speed;
	tr_data.bits_per_word = bits;

	printf("\nspi mode: %d", mode);
	printf("\nbits per word: %d", bits);
	printf("\nmax speed: %d Hz (%d KHz)", speed, speed/1000);
}

unsigned char SPI_Receive()
{
	int ret = 0;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr_data);
	if (ret == -1)
		pabort("Error in SPI_Transmit");
	return spi_rx[0];
}

void SPI_Transmit(unsigned char data)
{
	int ret = 0;
	spi_tx[0] = data;
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr_data);
	if (ret == -1)
		pabort("Error in SPI_Transmit");
}




