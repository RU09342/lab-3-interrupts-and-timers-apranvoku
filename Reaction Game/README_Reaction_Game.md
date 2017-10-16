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

First, 2 unsigned ints player1 and player 2 are initialized as 0.
Then, the code designates 3 LEDS as outputs.
Next, it designates 2 buttons as inputs, and clears their interrupt flags. It sets the falling edge
as the interrupt enable because the button is on an "on" state when not pressed.

TA0CTL is set to TASSEL_1 + MC_1. This sets up ACLK at 32kHz and enables up mode for the timer.
TA0CCTL1 is set equal to 0x10 to set TACCR1 to compare mode.
Finally, TA0CCR1 is set to 500, so an interrupt is generated when TAR is equal to TA0CCR1.

The processor is put into LPM4 to prepare for the interrupt from the button.
In the same line, GIE is enabled so the interrupt is not masked.
__no_operation(); is used to make sure the processor does nothing else.

Everytime the timer interrupt is enabled, 
player1 is compared to player2. If player 1 is higer,
LED P3.0 is turned on. Else, LED P3.0 is turned off.
If either score is higher than 60,000, both scores are set to 0 to avoid overflow.

When either button is pressed, one of two button interrupts are enabled. These interrupts work similarly,
so only one will be described.
The interrupt temporarily disables the interrupt on the button pressed to avoid debouncing.
Player 1 or player 2 is incremented.
A led is toggled to show that the interrupt has been entered, and finally the corresponding flag is cleared
so that the interrupt is available again.
