/*******************************************************
Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>

#asm
    .equ __lcd_port= 0x1B; // LCD I/O Port: PORTA
#endasm

#include <lcd.h>
#include <stdlib.h>

static unsigned int timer1_counter = 0;

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    TCNT1=0xE0C0;
    ++timer1_counter;
}

interrupt [TIM1_CAPT] void timer1_capt_isr(void)
{
    char str[10];
    unsigned long period;
    unsigned int lcr1;  
    
    lcr1 = (ICR1H*256)+ICR1L;
    period = ((unsigned long)(((float)lcr1)/((float)(0xffff-0xE0C0)))) 
            + ((unsigned long) timer1_counter) ;
  
    
    timer1_counter = 0;         
    TCNT1=0xE0C0;
                         
    itoa(period,str);    
    
    lcd_clear();              
    lcd_puts("period = ");
    lcd_puts(str);
}


void main(void)
{

DDRA= 0xFF;
DDRC= 0xF0;
PORTC.7 = 0; 

TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0x83;

TCCR1A= 0;
TCCR1B=(1<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
TCNT1=0xE0C0;
ICR1H=0x00;
ICR1L=0x00;

TIMSK=(0<<OCIE2) | (0<<TOIE2) | (1<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

lcd_init(16);
lcd_puts("hello world");

#asm("sei");


while (1){
    TIMSK |= (1<<TICIE1) | (1<<TOIE1);
    TIMSK &= ~(1<<TOIE0); 
}
}
