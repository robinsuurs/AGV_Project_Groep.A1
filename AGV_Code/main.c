/*
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

void init_timer(void)
{
    ///timmer settup voor stepper motor1
    ///gebruikt timer0 in CTC mode
    ///met snelheid tussen 70 als langzaamste en 15 als snelste
    TCCR0A  = BV(COM0A0) | BV(WGM01);
    TCCR0B  = BV(CS02);             // clk/256
    OCR0A   = 70;                   //start snelheid
    SetBit(DDRD, PD6);              //enable output timer D6

    ///timmer settup voor stepper motor2
    ///gebruikt timer2 in CTC mode
    ///met snelheid tussen 70 als langzaamste en 15 als snelste
    TCCR2A  = BV(COM2A0) | BV(WGM21);
    ClearBit(ASSR, AS2);             // interne io klok geselecteerd
    TCCR2B  = BV(CS22) | BV(CS21);   // clk/256
    OCR2A   = 70;                    // start snelheid
    SetBit(DDRB, PB3);               // enable output timer D11
}


int main(void)
{
    init_timer();
    // Insert code

    while(1)
    ;

    return 0;
}
