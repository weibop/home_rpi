
//LCD Functions +

#define PIN_LCD_DC RPI_BPLUS_GPIO_J8_35
//#define PIN_LCD_CS RPI_BPLUS_GPIO_J8_24
#define PIN_LCD_LIGHT RPI_BPLUS_GPIO_J8_37
#define PIN_LCD_RESET RPI_BPLUS_GPIO_J8_33

#define SEND_CMD 0
#define SEND_CHR 1
#define LCD_X_RES                  84
#define LCD_Y_RES                  48
#define LCD_START_LINE_ADDR	(66-2)

void LCD_Init();

void LCD_Send(unsigned char data, unsigned char cd);

void LCD_Clear_Memory();
void LCD_Update(unsigned char mode, unsigned char num);

void LCD_Clear();

void LCD_Contrast(unsigned char contrast);

void LCD_Set_Position(unsigned char x, unsigned int y);
void LCD_Write(unsigned char *a, int l);
void LCD_Light(int enable);
