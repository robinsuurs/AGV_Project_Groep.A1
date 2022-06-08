/*
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))

volatile uint16_t ADC_waarde[3];
volatile int8_t ADC_pin;

ISR(ADC_vect)
{
    ADC_waarde[ADC_pin] = ADC;
    for(ADC_pin; ADC_pin < 3; ++ADC_pin)
    {
        ADMUX = (0XF0 & ADMUX) | ADC_pin;
        SetBit(ADCSRA, ADSC);
        break;
    }
}

double MapRange(double X, double A1, double A2, double B1, double B2)
{
    double Y = B1 + (X-A1)*((B2-B1)/(A2-A1));
    return Y;
}

void init_timer(void)
{
    ///timmer settup voor stepper motor1
    ///gebruikt timer0 in CTC mode
    ///met snelheid tussen 70 als langzaamste en 15 als snelste
    TCCR0A  = BV(COM0A0) | BV(WGM01);
    TCCR0B  = BV(CS02);             // clk/256
    OCR0A   = 15;                   //start snelheid
    SetBit(DDRD, PD6);              //enable output timer D6

    ///timmer settup voor stepper motor2
    ///gebruikt timer2 in CTC mode
    ///met snelheid tussen 70 als langzaamste en 15 als snelste
    TCCR2A  = BV(COM2A0) | BV(WGM21);
    ClearBit(ASSR, AS2);             // interne io klok geselecteerd
    TCCR2B  = BV(CS22) | BV(CS21);   // clk/256
    OCR2A   = 15;                    // start snelheid
    SetBit(DDRB, PB3);               // enable output timer D11
}

void init_adc(void)
{
    ADMUX |= (1 << REFS0);                                  // AVCC als referentiespanning
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // Prescaler 128 125kHZ
    ADCSRA |= (1 << ADEN) | BV(ADIE);                       // Aanzetten ADC

    // Waarde 1 genereren
    ADMUX &= ~(1 << MUX0);                          // ADC op poort 0
    ADCSRA |= (1 << ADSC);                          // Conversatie starten
}


int main(void)
{
    init_timer();
    init_adc();
    // Insert code

    while(1)
    {
        if(ADC_waarde[0] <= ADC_waarde[1])
        {
            OCR0A = 70;     //slow speed
            OCR2A = 15;     //full speed
        }
        else if(ADC_waarde[0] > ADC_waarde[1])
        {
            OCR2A = 70;     //slowspeed
            OCR0A = 15;     //fullspeed
        }
    }


    return 0;
}
