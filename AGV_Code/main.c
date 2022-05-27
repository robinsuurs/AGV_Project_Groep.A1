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
    ///timmer settup voor stepper motor
    ///gebruikt timer0 in CTC mode
    ///met snelheid tussen 180 als langzaamste en 135 als snelste
    TCCR0A = BV(COM0A0) | BV(WGM01);
    TCCR0B = BV(CS01) | BV(CS00);
    OCR0A = 185; //start snelheid
    SetBit(DDRD, PD6); //enable output timer

}


int main(void)
{
    init_timer();
    // Insert code

    while(1)
    ;

    return 0;
}
