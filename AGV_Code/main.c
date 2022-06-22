/*
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BV(bit)                 (1 << (bit))
#define SetBit(byte, bit)       (byte |= BV(bit))
#define ClearBit(byte, bit)     (byte &= ~BV(bit))
#define ToggleBit(byte, bit)    (byte ^= BV(bit))
volatile uint16_t POT0_BEGIN;
volatile uint16_t POT2_BEGIN;
volatile uint16_t ADC_waarde_0;
volatile uint16_t ADC_waarde_2;
volatile uint16_t verschil_0;
volatile uint16_t verschil_2;

double MapRange(double X, double A1, double A2, double B1, double B2)
{
    double Y = B1 + (X-A1)*((B2-B1)/(A2-A1));
    return Y;
}

void init_timer(void)
{
    ///timmer settup voor stepper motor1
    ///gebruikt timer0 in CTC mode
    ///met snelheid tussen 70 als snelste en 100 als langzaamste
    TCCR0A  = BV(COM0A0) | BV(WGM01);
    TCCR0B  = BV(CS02);             // clk/256
    OCR0A   = 80;                   //start snelheid
    SetBit(DDRD, PD6);              //enable output timer D6

    ///timmer settup voor stepper motor2
    ///gebruikt timer2 in CTC mode
    ///met snelheid tussen 70 als snelste en 100 als langzaamste
    TCCR2A  = BV(COM2A0) | BV(WGM21);
    ClearBit(ASSR, AS2);             // interne io klok geselecteerd
    TCCR2B  = BV(CS22) | BV(CS21);   // clk/256
    OCR2A   = 80;                    // start snelheid
    SetBit(DDRB, PB3);               // enable output timer D11
}

void init_adc(void)
{
    ADMUX |= (1 << REFS0);                                  // AVCC als referentiespanning
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // Prescaler 128 125kHZ
    ADCSRA |= (1 << ADEN);                                  // Aanzetten ADC


    // Waarde 1 genereren
    ADMUX &= ~(1 << MUX0);                          // ADC op poort 0
    ADCSRA |= (1 << ADSC);                          // Conversatie starten
    while (ADCSRA & (1 << ADSC));{}                 // Wacht tot conversatie klaar is
    POT0_BEGIN = ADC;                               // Waarde meegeven aan variabele


    // Waarde 2 genereren
    ADMUX |= (1 << MUX0);                           // ADC op poort 1
    ADCSRA |= (1 << ADSC);                          // Conversatie starten
    while (ADCSRA & (1 << ADSC));{}                 // Wacht tot conversatie klaar is
    POT2_BEGIN = ADC;                             // Waarde meegeven aan variabele



}

void ADC_Check(void)
{
    // Waarde 1 genereren
    ADMUX &= ~(1 << MUX0);                          // ADC op poort 0
    ADCSRA |= (1 << ADSC);                          // Conversatie starten
    while (ADCSRA & (1 << ADSC));{}                 // Wacht tot conversatie klaar is
    ADC_waarde_0 = ADC;                               // Waarde meegeven aan variabele


    // Waarde 2 genereren
    ADMUX |= (1 << MUX0);                           // ADC op poort 1
    ADCSRA |= (1 << ADSC);                          // Conversatie starten
    while (ADCSRA & (1 << ADSC));{}                 // Wacht tot conversatie klaar is
    ADC_waarde_2 = ADC;                             // Waarde meegeven aan variabele



    ///links
    if((POT0_BEGIN > ADC_waarde_0) && (POT2_BEGIN < ADC_waarde_2))
    {
        verschil_0 = POT0_BEGIN - ADC_waarde_0;
		verschil_2 = ADC_waarde_2 - POT2_BEGIN;
        if(verschil_0 > verschil_2)
        {
            if((verschil_0 - verschil_2) >= 50)
            {
                OCR0A = 100 ;
                OCR2A = 80 ;
            }
            else
            {
                OCR0A = 80 ;
                OCR2A = 80 ;
            }

        }
        else
        {
            if((verschil_2 - verschil_0) >= 50)
            {
                OCR0A = 80 ;
                OCR2A = 100 ;
            }
            else
            {
                OCR0A = 80 ;
                OCR2A = 80 ;
            }

        }
    }
    if(POT2_BEGIN < ADC_waarde_2)
    {
        ///rechts
        verschil_2 = ADC_waarde_2 - POT2_BEGIN;
        if(verschil_2 < 150)
        {
            OCR0A = 95 ;
            OCR2A = 95 ;
        }
        if(verschil_2 > 150)
        {
            OCR0A = 80 ;
            OCR2A = 100 ;
        }

    }
}

int main(void)
{
    init_timer();
    init_adc();

    // Insert code

    while(1)
        ADC_Check();

    ;

    return 0;
}
