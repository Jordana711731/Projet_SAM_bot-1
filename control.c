#include <msp430.h>

// Déclaration des variables
volatile unsigned int compteur_gauche = 0;
volatile unsigned int compteur_droit = 0;
unsigned char etat1_gauche = 1;
unsigned char etat1_droit  = 1;

// Variables pour le contrôle PID
volatile int erreur_integrale = 0;
volatile int erreur_precedente = 0;

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
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog

    // Direction pins
    P2DIR &= ~(BIT0 | BIT3);                    // P2.0 et P2.3 en entrée
    P2DIR |=  (BIT1 | BIT2 | BIT4 | BIT5 | BIT6); // Ajout de BIT6 en sortie
    P2OUT &= ~(BIT1 | BIT5 | BIT6);             // Initialisation à 0
    P2OUT |= BIT0;

    // PWM pins
    P2SEL |= BIT2 | BIT4;

    TA1CCR0 = 1000 - 1;        // Période du PWM
    TA1CCR1 = 150;             // 15% cycle moteur gauche (départ)
    TA1CCR2 = 150;             // 15% cycle moteur droit (départ)

    TA1CTL = TASSEL_2 | MC_1;
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL2 = OUTMOD_7;

    moteur_avant_gauche();
    moteur_arriere_droit();

    etat1_gauche = (P2IN & BIT0) ? 1 : 0;
    etat1_droit  = (P2IN & BIT3) ? 1 : 0;

    unsigned int compteur_regulation = 0;
    int vitesse_base = 150;    // Vitesse de référence (15%)

    while (1){
        // lectures actuelles
        unsigned char etat2_gauche = (P2IN & BIT0) ? 1 : 0;
        unsigned char etat2_droit  = (P2IN & BIT3) ? 1 : 0;

        // Détection de front descendant
        if (etat1_gauche == 1 && etat2_gauche == 0) {
            compteur_gauche++;
        }
        if (etat1_droit == 1 && etat2_droit == 0) {
            compteur_droit++;
        }

        // mettre à jour les états précédents
        etat1_gauche = etat2_gauche;
        etat1_droit  = etat2_droit;

        // Régulation PID
        compteur_regulation++;
        if (compteur_regulation >= 5000) {
            compteur_regulation = 0;

            // Erreur = différence entre les compteurs
            int erreur = compteur_gauche - compteur_droit;

            // Calcul PID
            // Proportionnel
            int correction_P = erreur * 3;  // Kp = 3

            // Intégral (accumulation de l'erreur)
            erreur_integrale += erreur;
            // Limiter l'intégrale pour éviter le windup
            if (erreur_integrale > 50) erreur_integrale = 50;
            if (erreur_integrale < -50) erreur_integrale = -50;
            int correction_I = erreur_integrale / 2;  // Ki = 0.5

            // Dérivé (vitesse de changement de l'erreur)
            int erreur_derivee = erreur - erreur_precedente;
            int correction_D = erreur_derivee * 2;  // Kd = 2
            erreur_precedente = erreur;

            // Correction totale
            int correction = correction_P + correction_I + correction_D;

            // Appliquer la correction (ajuster les deux moteurs symétriquement)
            TA1CCR1 = vitesse_base - correction;  // Si erreur > 0, ralentir gauche
            TA1CCR2 = vitesse_base + correction;  // Si erreur > 0, accélérer droit

            // Limiter le PWM entre 10% et 25% (100 à 250)
            if (TA1CCR1 < 100) TA1CCR1 = 100;
            if (TA1CCR1 > 250) TA1CCR1 = 250;
            if (TA1CCR2 < 100) TA1CCR2 = 100;
            if (TA1CCR2 > 250) TA1CCR2 = 250;

            // Réinitialiser les compteurs après régulation
            compteur_gauche = 0;
            compteur_droit = 0;
        }

        __delay_cycles(50);
    }
}
