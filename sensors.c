#include <msp430.h>				
#include "ADC.h"
#include "Afficheur.h"

#define SEUIL_MAX 1021
#define SEUIL_MIN 100

#define CATTEUR_DROIT BIT0
#define CATTEUR_LEFT BIT1
#define CATTEUR_DROIT_ANALOG 0
/**
 * blink.c
 */

int return_line_follower_right(){
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR &= ~BIT0;                 // configure P1.0 as output

    int valeur;
    ADC_init();
    ADC_Demarrer_conversion(CATTEUR_DROIT_ANALOG);
    valeur=ADC_Lire_resultat ();

 // RETOURNE 0 SI ON EST SUR DU BLANC ET 1 SUR DU NOIR
    if(convert_Hex_Dec(valeur) < SEUIL_MIN ){
        return 0;
    }
    else{
        return 1 ;
    }
}


