# Read Me for Button interrupt
## Author: Ardit Pranvoku
To run this code, simply import the project folder into code composer, build, then debug.
Implements a timer based interrupt to blink the LED at a designated frequency.
The watchdog timer must be stopped with the line WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD.
Else, the processor will reset.
The desired led pins and bits must be set to 1 to configure it to be an output.
The desired button pin and bit must be to 0 to configure it to be an input.
Also,  PXREN |= BITX; must be used to enable the pullup resistor for that button.     
By using the line PM5CTL0 = ~LOCKLPM5, the default high impedance on the board is disabled.
This high impedance serves to get rid of any cross currents, but is turned off later.

TA0CTL is set to TASSEL_1 + MC_1. This sets up ACLK at 32kHz and enables up mode for the timer.
TA0CCTL1 is set equal to 0x10 to set TACCR1 to compare mode.
Finally, TA0CCR1 is set to 3000, so an interrupt is generated when TAR is equal to TA0CCR1.

The processor is put into LPM4 to prepare for the interrupt from the button.
In the same line, GIE is enabled so the interrupt is not masked.

The interrupt is set up using the lines
#pragma vector=TIMER0_A1_VECTOR

__interrupt void Timer_A(void)

During the interrupt, a switch case is used for TACCR1. This is done because TACCR0 is the only CCR 
that can be used to generate an interrupt automatically. 
