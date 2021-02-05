/*******************************************************
Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>

static int active = 0;

unsigned char t_ary[4] = {0};
static unsigned int timer = 0;

char bcd(int n){
    switch (n){
     case 0:    return 0b0111111;
     case 1:    return 0b0000110;
     case 2:    return 0b1011011;
     case 3:    return 0b1001111;
     case 4:    return 0b1100110;
     case 5:    return 0b1101101;
     case 6:    return 0b1111101;
     case 7:    return 0b0000111;
     case 8:    return 0b1111111;
     case 9:    return 0b1101111;
    }
}

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{

    TCNT0= 0xFF;
    
    if (active>=4)
        active = 0;             

    PORTC = 0x0F & ~(1<<active);
    PORTD = bcd(t_ary[active]);
    
    active++;
}

// Timer 1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
    int i,d=timer;
    
    TCNT1 = 0xFFFF - 0x7A11;
    
    timer++;  
    if (timer>9999)
        timer =0;
    
    for (i=0;i<4;i++){
        t_ary[i] = d%10;
        d /= 10;
    }
}


void main(void)
{
// Declare your local variables here

DDRC= 0x0F; 
DDRD= 0xFF; 

PORTC=0xFE;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 7812 Hz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0xFF;



// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Timer Period: 1 s
// Timer1 Overflow Interrupt: On
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);

//TCNT1H=0xE0;
//TCNT1L=0xC0;
TCNT1 = 0xFFFF - 0x7A11;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// Global enable interrupts
#asm("sei")

timer = 1;
while (1){
      // Write your code here
};
}
