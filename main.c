#include <msp430.h>

void moteur_arriere_gauche(){
    P2OUT |= BIT1;
    P2OUT &= ~BIT1;

}

void moteur_avant_gauche(){
    P2OUT &= ~BIT1;
    P2OUT |= BIT1;
}

void moteur_arriere_droit(){
    P2OUT |= BIT5;
    P2OUT &= ~BIT5;
}

void moteur_avant_droit(){
    P2OUT &= ~BIT5;
    P2OUT |= BIT5;
}

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Direction pins
    P2DIR |= BIT1 | BIT5;

    // PWM pins
    P2DIR |= BIT2 | BIT4;
    P2SEL |= BIT2 | BIT4;

    TA1CCR0 = 1000 - 1;        // Période du PWM
    // Rapport cyclique (50%)
    TA1CCR1 = 500;
    TA1CCR2 = 500;
    TA1CTL = TASSEL_2 | MC_1;

    TA1CCR2 = 500;
    // Mode sortie : Reset/Set
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;


    moteur_avant_gauche();
    moteur_arriere_droit();


    while (1);
}
