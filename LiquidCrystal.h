#ifndef LIQUIDCRYSTAL_H_
#define LIQUIDCRYSTAL_H_

//************************************************
// MSP430 - LCD Test Program
//************************************************

/************************************************

2013.06.22 - MrChips
           - LCD Interface Test
           - Edit: Austin Moore to work with CCS

************************************************/

#include <msp430g2553.h>
#include <string.h>

#define DELAY 20000

//---------------------------------------------------
// LCD Interface
//---------------------------------------------------

void LCD_init(void);
void LCD_ready(void);
void LCD_posn(char n);
void LCD_ir(char ch);
void LCD_dr(char ch);
void LCD_txt(char *s);
void LCD_home(void);
void LCD_clear(void);
void LCD_Line1(void);
void LCD_Line2(void);
void LCD_Countdown_10to0(void);


// define LCD interface on PORT2
#define LCD_OUT  P2OUT
#define LCD_IN   P2IN
#define LCD_DDR  P2DIR
#define LCD_RS0  P2OUT &= ~BIT4
#define LCD_RS1  P2OUT |= BIT4
#define LCD_RW0  P2OUT &= ~BIT5
#define LCD_RW1  P2OUT |= BIT5
#define LCD_E0   P2OUT &= ~BIT6
#define LCD_E1   P2OUT |= BIT6

void LCD_init(void)
{
  LCD_ir(0x28);         // dual line, 4 bits
  LCD_ir(0x06);         // increment mode
  LCD_ir(0x0C);         // cursor turned off
  LCD_ir(0x10);         // cursor right
  LCD_ir(0x01);         // clear display
}

void LCD_clear(void)
{
  LCD_ir(0x01);
}

void LCD_home(void)
{
  LCD_ir(0x02);
}

void LCD_Line1(void)
{
  unsigned char i;
  LCD_ir(0x80);
  for (i = 0; i < 16; i++)
  {
    LCD_dr(0x20);
  }
  LCD_ir(0x80);
}

void LCD_Line2(void)
{
  unsigned char i;
  LCD_ir(0xC0);
  for (i = 0; i < 16; i++)
  {
    LCD_dr(0x20);
  }
  LCD_ir(0xC0);
}

void LCD_posn(char n)
// direct positioning where n = 0-79
{
  char ch;
  ch = n;
  if (ch  >= 40) ch += 24;
  LCD_ir(ch | 0x80);
}

void LCD_ready(void)
{
  char busy;
  LCD_DDR = 0xF0;
  LCD_RW1;
  LCD_RS0;
  do
  {
    LCD_E1;
    // high 4 bits read first
    // busy flag is D7 of LCD = bit 3 of P2IN
    busy = (LCD_IN & 0x08);
    LCD_E0;
    LCD_E1;
    LCD_E0;
  } while (busy);
  LCD_OUT = 0;
  LCD_DDR = 0xFF;
}

void LCD_ir(char ch)
{
  LCD_ready();
  //LCD_RS = 0;     // not needed      // select instruction register
  //LCD_RW = 0;     // not needed
  LCD_OUT |= ((ch >> 4) & 0x0F);
  LCD_E1;
  LCD_E0;
  LCD_OUT = 0;
  LCD_OUT |= (ch & 0x0F);
  LCD_E1;
  LCD_E0;
}

void LCD_dr(char ch)
{
  LCD_ready();
  LCD_RS1;        // select data register
  //LCD_RW = 0;      // not needed
  LCD_OUT |= ((ch >> 4) & 0x0F);
  LCD_E1;
  LCD_E0;
  LCD_OUT = 0;
  LCD_RS1;
  LCD_OUT |= (ch & 0x0F);
  LCD_E1;
  LCD_E0;
}

void LCD_txt(char *s)
{
  while(*s) LCD_dr(*s++);
}

void LCD_Countdown_10to0(){
    LCD_Line2();
    LCD_txt("10 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("9 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("8 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("7 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("6 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("5 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("4 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("3 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("2 Seconds");
    __delay_cycles(1000000);
    LCD_Line2();
    LCD_txt("1 Seconds");
    __delay_cycles(1000000);
}

#endif /* LIQUIDCRYSTAL_H_ */
