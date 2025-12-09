#include <msp430.h>				
#include "ADC.h"
#include "Afficheur.h"

/**
 * blink.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR &= ~BIT0;					// configure P1.0 as output

	int valeur;
	int i =0;
	ADC_init();
	Aff_Init();
	while(1){
	    ADC_Demarrer_conversion(0);
	    valeur=ADC_Lire_resultat ();
	    Aff_valeur(convert_Hex_Dec(valeur));

	    for(i=0;i<10000;i++);
	}
}


