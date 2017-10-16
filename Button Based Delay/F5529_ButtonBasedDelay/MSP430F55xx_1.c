//******************************************************************************
//   MSP430F552x Demo
//      Button Based Interrupt
//
//   Description: Interrupt a ongoing process by pushing a button. The delay time
//              is based upon how long the button is pressed.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F552x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   Thai Nghiema
//   Rowan University
//   September 2017
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

int buttonPressed;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
    P1SEL &= ~BIT0; //select GPIO
    P1DIR |= BIT0; //set Port 1.0 output ---LED

    P1DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN |= BIT1; //enable pull-up resistor on
    P1OUT |= BIT1;


    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES |= BIT1; //set as falling edge, pullup resistor.
    P1IFG &= ~(BIT1); //clear interrupt flag

    TA0CTL = TASSEL_1 + MC_1 + TACLR + ID_2;//Set up Timer A using ACLK(32kHZ), Up mode, clear TAR, and 4 divider.
    TA1CCR0 = 6000; //Counts up to 6000, generates interrupt, then resets.
    TA1CCTL0 = 0; //Set up compare mode for CCTL
     // LED will blink at (((32kHz/4)/6000)/2) = 0.66 Hz
    //32kHZ is AClk frequency, 4 is the divider, 6000 is CCR0, and 2 interrupts are needed to blink button on and off.


    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); // Enter Low Power Mode 4
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A(void)
{

    P1OUT ^= 0x01; //Toggle LED

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debouncing
    P1IE &= ~BIT1;
    __delay_cycles(1);

    if (buttonPressed == 0) //Falling-edge of a button
    {
        TA1CCR0 = 1; //Reset CCR0
        TA1CCTL0 = 0; //Capture mode
        buttonPressed = 1;

    }
    else if (buttonPressed == 1) //Rising-edge of a button
    {
        TA1CTL = MC_0; //Stop Counting
        if (TA1CCR0 < 2000) //Faster
                  TA1CCR0 = 2000;
        TA1CCR0 = TA1R; //Assign new value for CCR0
        TA1CTL = TACLR; //Clear Timer A1
        TA1CCTL0 = 0x10; //Reset CCR to compare mode.
        buttonPressed = 0;
    }

    P1IES ^= BIT1; //toggle to set as rising edge
    P1IE |= BIT1; // Enable interrupt
    P1IFG &= ~(BIT1); // Clear flag
}
