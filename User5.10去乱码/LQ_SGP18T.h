/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
???    ???????i.MX RT1052?????-???????
????    д??CHIUSIR
??E-mail  ??chiusir@163.com
???????汾??V1.0
?????????2018??2??1??
?????????ο????е????
????    ???http://www.lqist.cn
??????????http://shop36265907.taobao.com
------------------------------------------------
??dev.env.??IAR8.20.1??????汾
??Target ?? i.MX RT1052
??Crystal?? 24.000Mhz
??ARM PLL?? 1200MHz
??SYS PLL?? 528MHz
??USB PLL?? 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	
#include "include.h" 

/*******************??????******************************/
#define APP_LCD_WIDTH  LCD_WIDTH//?????????
#define APP_LCD_HEIGHT LCD_HEIGHT//????????

#define TFT18W        162
#define TFT18H        132

#define	u16RED		0xf800
#define	u16GREEN	0x07e0
#define	u16BLUE		0x001f
#define	u16PURPLE	0xf81f
#define	u16YELLOW	0xffe0
#define	u16CYAN		0x07ff 		//?????
#define	u16ORANGE	0xfc08
#define	u16BLACK	0x0000
#define	u16WHITE	0xffff


/*****************??к???????*********************************/							
extern void TFTSPI_Init(uint8_t type);									 //LCD?????  0:????  1??????
extern void TFTSPI_Write_Cmd(uint8_t cmd);						         //?????????
extern void TFTSPI_Write_Byte(uint8_t dat);						         //???????????
extern void TFTSPI_Write_Word(uint16_t dat);						         //???????????????
extern void TFTSPI_Addr_Rst(void);							         //DDRAM???????
extern void TFTSPI_Set_Pos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);	                         //??λ???????λ??
extern void TFTSPI_CLS(uint16_t color);					                         //????????????
extern void TFTSPI_Draw_Part(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);     //?????????????????????????????????????????
extern void TFTSPI_Draw_Line(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);     //????????????????????????????????????
extern void TFTSPI_Draw_Rectangle(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);//?????α?????????????????????????????????
extern void TFTSPI_Draw_Circle(uint8_t x,uint8_t y,uint8_t r,uint16_t color_dat);                 //????α???????????????????????
extern void TFTSPI_Draw_Dot(uint8_t x,uint8_t y,uint16_t color_dat);	                         //???????????????????
extern void TFTSPI_P8X16(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X16Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8Str(uint8_t x, uint8_t y, uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8NUM(uint8_t x, uint8_t y, uint16_t num, uint8_t num_bit,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8NUM(uint8_t x, uint8_t y, uint16_t num, uint8_t num_bit,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P16x16Str(uint8_t x,uint8_t y,uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P16x12Str(uint8_t x,uint8_t y,uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_Show_Pic(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint8_t *ppic);
extern void TFTSPI_Show_Pic2(uint8_t xs,uint8_t ys,uint8_t w,uint8_t h,const uint8_t *ppic) ;
extern void TFTSPI_Show_Battery_Icon(void);
extern void TFTSPI_Show_Title(void);
extern void TFTSPI_Show_Logo(uint8_t xs,uint8_t ys);
extern void TFTSPI_Test(void);
extern void ColumnarSetting(uint8_t x,uint8_t y, uint8_t w,uint8_t h,uint16_t fclolr,uint16_t bcolor);
extern void DrawingColumnar(uint8_t t,uint8_t duty,uint8_t cl);
extern void DrawSpectrum(void);
extern void Clear_Columnar(void);
#endif /*SGP18T_ILI9163B.h*/