#include <stdio.h>
#include <stdint.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "lcd.h"
#include "hal_spi.h"
#include "font.h"
#include <string.h>
//#include "m24sr.h"
//#include <linux/module.h>
//#include <linux/init.h>
//#include <linux/iqr.h>
//#include <linux/interrupt.h>
//#include <linux/gpio.h>

void LcdUpdate(void);
void ResetDoctor(void);

#define PIN_GPIO1 	RPI_BPLUS_GPIO_J8_11
#define PIN_GPIO2 	RPI_BPLUS_GPIO_J8_13
#define PIN_GPIO3 	RPI_BPLUS_GPIO_J8_15
#define PIN_GPIO4 	RPI_BPLUS_GPIO_J8_16
#define PIN_GEL 	PIN_GPIO1
#define PIN_ARR 	PIN_GPIO2
#define PIN_BUZZ 	PIN_GPIO3 
#define PIN_RSTN 	PIN_GPIO4

#define STAT_WAIT4DOCTOR 0
#define STAT_WAIT4WASH   1
#define STAT_WAIT4WASHED 2

static uint8_t mode;
static uint8_t bit = 8;
static uint32_t speed = 1000000;
static uint16_t delay;

unsigned char text1[15], text2[15],  text3[15],  text4[15], text5[15], text6[15];
unsigned char text_c[15] = "SafeHands.com ";//company
unsigned char text_b[15] = "              ";//blank

unsigned char fileName[] = "example_logfile";

FILE *logFile;
uint8_t currDocId, lastDocId, stat;
uint8_t doctorIsHere = 0, handsWashed = 0, handsWashedInTime=1;
long ts_dr_curr, ts_dr_last = 0;
uint8_t washing_cnt = 0, notwashing_cnt = 0;
uint16_t cnt1=0;
uint64_t cnt2=0;


int main(int argc, char*argv[]){
 	//unsigned int i;
	//unsigned char c;
	uint8_t ts_text[50], ts_print[15];
	//long ts_wash_curr, ts_wash_last = 0;

	printf("Initializing the board...\n");
	if (!bcm2835_init()){
		printf(" *** Error: *** bcm2835 initialization failed ***\n");
	}
	ResetDoctor();

	memset(ts_print,0,15);
	bcm2835_gpio_fsel(PIN_RSTN, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(PIN_ARR, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(PIN_GEL, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_fsel(PIN_BUZZ, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_BUZZ, LOW);
	while (1)
	{
		ts_dr_curr = (long)time(NULL);
		memcpy(ts_text, ctime(&ts_dr_curr), 50);
		memcpy(ts_print, ts_text+11, 8);
		//bcm2835_delay(1);
		//printf("d=%d, h=%d, tc=%d, tl=%d, cnt1=%d, ,cnt2=%d\n", doctorIsHere, handsWashed, ts_dr_curr, ts_dr_last, cnt1,cnt2);
		if(!doctorIsHere){
			if(bcm2835_gpio_lev(PIN_ARR)==HIGH){
				doctorIsHere = 1;
				//fopen(fileName, "a+");
				printf("%s Doctor comes.\n", ts_print);
				logFile = fopen(fileName, "a+");
				//fprintf(logFile, "Doctor tags in.\n");
				fclose(logFile);
				ts_dr_last = ts_dr_curr;
				memcpy(text3, "doctor arrives", 14);
				//memset(text3+12, ' ', 2);
				memcpy(text4, text_b, 14);

			}

			if(cnt1==100){
				//printf("%s", ctime(&ts_dr_curr));
				printf("%s Waiting for doctor...\n", ts_print);
			}
		}
		else{//doctorIsHere
			if(!handsWashed){
				//if(ts_dr_curr - ts_dr_last > 5){
				handsWashedInTime = ts_dr_curr - ts_dr_last == 5?0:1;
				//}
				if(bcm2835_gpio_lev(PIN_GEL)==HIGH){
					handsWashed = 1;
					handsWashedInTime = 1;
					washing_cnt = 1;
					//fopen(fileName, "a+");
					printf("%s Doctor washing hands.\n", ts_print);
					//logFile = fopen(fileName, "a+");
					//fprintf(logFile, "Doctor washes the hands.........\n");
					//fclose(logFile);
					memcpy(text3, "Doctor is", 9);
					memset(text3+9, ' ', 5);
					memcpy(text4, "washing hands", 13);
					memset(text4+13, ' ', 1);
				}
			}

			if(cnt1==100){
				printf("%s Doctor is here...\n", ts_print);
				//printf(" h=%d w=%d\n", handsWashed,washing_cnt);
			}

			/*if(ts_curr - ts_last > 600){
				logFile = fopen(fileName, "a+");
				printf("\n*** warning ***  some doctor didn't wash the hands in 5s ***");
				fprintf(logFile, "Doctor times out in 60s.\n");
				fclose(logFile);
				ResetDoctor();
			}*/
		}
		if(!handsWashedInTime){
			if((cnt1 < 10)||(cnt1>20 && cnt1<30)) {
				bcm2835_gpio_write(PIN_BUZZ, HIGH);
			}else{
				bcm2835_gpio_write(PIN_BUZZ, LOW);
			}
			if(cnt1 == 0){
				memcpy(text3, "Please wash", 11);
				memset(text3+11, ' ', 3);
				memcpy(text4, "your hands!", 11);
				memset(text4+11, ' ', 3);
				printf("%s *** warning ***  Please wash your hands ***\n", ts_print);
			}
		}else{
			bcm2835_gpio_write(PIN_BUZZ, LOW);
		}

		if(washing_cnt == 15){
			//if((cnt1 < 10) || (cnt1>20 && cnt1<30) ||(cnt1>40 && cnt1<50)) {
			//	bcm2835_gpio_write(PIN_BUZZ, HIGH);
			//}
			if(cnt1 == 0){
				printf("%s *** Well Done! ***\n", ts_print);
				memcpy(text3, "Thanks for", 10);
				memset(text3+10, ' ', 4);
				memcpy(text4, "washing hands", 13);
				memset(text4+13, ' ', 1);
			}
		}else if(washing_cnt>=16){
			washing_cnt = 0;
		}

		bcm2835_delay(10);
		if(cnt1++>=100){
			cnt1 = 0;
			cnt2++;
			if(handsWashed && washing_cnt>0){
				washing_cnt++;
			}
			//memcpy(ts_text,ctime(&ts_dr_curr), 50);
			memcpy(text1, ts_print, 8);
			LcdUpdate();
		}

		if(bcm2835_gpio_lev(PIN_RSTN)==LOW){
			ResetDoctor();
		}
	}
}

void LcdUpdate(void){
	LCD_Clear_Memory();
	LCD_Write(text1,14);
	LCD_Write(text2,14);
	LCD_Write(text3,14);
	LCD_Write(text4,14);
	LCD_Write(text5,14);
	LCD_Write(text6,14);
	LCD_Update(1,0);
}

void ResetDoctor(void){
	doctorIsHere = 0;
	handsWashed = 0;
	handsWashedInTime = 1;
	washing_cnt = 0;
	notwashing_cnt = 0;
	cnt1=0;
	cnt2=0;

	SPI_Init(bit, speed, 0);
	LCD_Init();
	LCD_Light(1);
	bcm2835_delay(50);
	LCD_Light(0);
	bcm2835_delay(50);
	LCD_Light(1);
	memcpy(text1,text_b,15);
	memcpy(text2,text_b,15);
	memcpy(text3,text_b,15);
	memcpy(text4,text_b,15);
	memcpy(text5,text_b,15);
	memcpy(text6,text_c,15);
	memcpy(text3, "no doctor", 9);
	memset(text3+9, ' ', 5);
	LcdUpdate();

}
