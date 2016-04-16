test: test.c hal_spi.c nazsys.h lcd.c
	gcc -o test test.c hal_spi.c nazsys.h lcd.c -l bcm2835
