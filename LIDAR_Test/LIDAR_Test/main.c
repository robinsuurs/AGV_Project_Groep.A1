/*
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

#define ServoHoek       OCR1A
#define ServoPin        PB1     /// D9

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

    SetBit(DDRB, ServoPin);



    while(1)
    {
        _delay_ms(500);
        ServoHoek = 4000;
        _delay_ms(500);
        ServoHoek = 2000;
        _delay_ms(500);
        ServoHoek = 1000;

    }
    // Insert code

    while(1)
    ;

    return 0;
}
