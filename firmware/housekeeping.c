#include "msp430g2553.h"
void chiptemp_setup(void)
{
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;
    ADC10CTL1 = INCH_10 + ADC10DIV_3;   // Channel 10 = Temp Sensor
}

int chiptemp_read(void)
{
    unsigned adc;

    // ENC = enable conversion, ADC10SC = start conversion
    ADC10CTL0 |= ENC + ADC10SC;
    while (!(ADC10CTL0 & ADC10IFG))
        /* wait until conversion is completed */ ;

    adc = ADC10MEM;
    // shut off conversion and lower flag to save power.
    // ADC10SC is reset automatically.
    while (ADC10CTL0 & ADC10BUSY)
        /* wait for non-busy per section 22.2.6.6 in User's Guide */ ;
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~ADC10IFG;

    // return degrees F
    return (int)((adc * 48724L - 30634388L) >> 16);
}

