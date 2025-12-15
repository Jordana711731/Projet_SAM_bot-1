#include <msp430.h>
#include "ADC.h"
#include "Afficheur.h"

#define SEUIL_MAX 1021
#define SEUIL_MIN 100

#define CATTEUR_RIGHT BIT0
#define CATTEUR_LEFT BIT1
#define CATTEUR_RIGHT_ANALOG 0
#define CATTEUR_LEFT_ANALOG 1
/**
 * blink.c
 */

int return_line_follower_right(){
   // WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR &= ~CATTEUR_RIGHT;                 // configure P1.0 as output

    Aff_Init();   //initialisation des methodes
    ADC_init();
    int valeur;
    ADC_Demarrer_conversion(CATTEUR_RIGHT_ANALOG);
    valeur=ADC_Lire_resultat ();

 // RETOURNE 0 SI ON EST SUR DU BLANC ET 1 SUR DU NOIR
    if(convert_Hex_Dec(valeur) < SEUIL_MIN ){
        Aff_valeur( 0);
        return 0;
    }
    else{
        Aff_valeur( 1);
        return  1;
    }
}

int return_line_follower_left(){
   // WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1DIR &= ~CATTEUR_LEFT;                 // configure P1.0 as output

    int valeur;
    Aff_Init();   //initialisation des methodes
    ADC_init();
    ADC_Demarrer_conversion(CATTEUR_LEFT_ANALOG);
    valeur=ADC_Lire_resultat ();

 // RETOURNE 0 SI ON EST SUR DU BLANC ET 1 SUR DU NOIR
    if(convert_Hex_Dec(valeur) < SEUIL_MIN ){

        Aff_valeur( 0);
        return 0;
    }
    else{

        Aff_valeur( 1);
        return  1;
    }
}


void main_test_capteurs (){
    WDTCTL = WDTPW | WDTHOLD;
   //Aff_valeur(return_line_follower_left);
  //  return_line_follower_left();
    Aff_Init();   //initialisation des methodes
    ADC_init();
    int i;
    while(1)
    {
    return_line_follower_right();
   for(i=1000;i>0;i--);
    }
}

