test: test.c hal_spi.c
	gcc -o test test.c hal_spi.c -l bcm2835
