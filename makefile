test: test.c hal_spi.c lcd.c ds18b20.c
	gcc -o test test.c hal_spi.c lcd.c ds18b20.c -l bcm2835
