test: test.c hal_spi.c lcd.c
	gcc -o test test.c hal_spi.c lcd.c -l bcm2835
