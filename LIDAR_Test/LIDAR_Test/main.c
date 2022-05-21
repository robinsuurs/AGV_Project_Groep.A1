/*
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

void Servo_Setup(void)
{
    //set fast PWM mode aan van timer1 met ICR1 als TOP waarde
    TCCR1A |= BV(WGM11);
    TCCR1A &= ~BV(WGM10);
    TCCR1B |= BV(WGM12) | BV(WGM13);

    //set Compare Output Mode van COM A
    TCCR1A |= BV(COM1A1);  //set OC0A pin laag op COM match en set OC0A pin hoog bij BOTTOM


    //set clock snelheid op clk/8
    TCCR1B |= BV(CS11);

    //set pulsfrequentie
    ICR1 = 40000; //20ms standaard voor servo pulsfrequentie

}

int main(void)
{
    ///aanzetten timer fuctie voor controle servo
    //ORC1A wordt gebruikt om de servo een positie te geven
    Servo_Setup;


    // Insert code

    while(1)
    ;

    return 0;
}
