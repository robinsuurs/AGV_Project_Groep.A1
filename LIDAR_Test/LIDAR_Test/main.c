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


void Init_ADC(void)
{
    SetBit(ADMUX, REFS0);    //referentie voltage VCC
    SetBit(ADCSRA, ADEN);    //zet ADC aan
    ADCSRA |= BV(ADPS2) | BV(ADPS1) | BV(ADPS0); //prescale 128
}


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

double MapRange(double X, double A1, double A2, double B1, double B2)
{
    double Y = B1 + (X-A1)*((B2-B1)/(A2-A1));
    return Y;
}



int main(void)
{
    ///aanzetten timer fuctie voor controle servo
    //ORC1A wordt gebruikt om de servo een positie te geven
    Servo_Setup();

    SetBit(DDRB, ServoPin);



    while(1)
    {
        _delay_ms(1000);
        ServoHoek = 4800; //180 graden
        _delay_ms(1000);
        ServoHoek = 3050; //90 graden
        _delay_ms(1000);
        ServoHoek = 1300; //0 graden

    }
    // Insert code

    while(1)
    ;

    return 0;
}
