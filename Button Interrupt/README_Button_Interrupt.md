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

The processor is put into LPM4 to prepare for the interrupt from the button.
In the same line, GIE is enabled so the interrupt is not masked.

The interrupt is set up using the lines
#pragma vector=PORT5_VECTOR

__interrupt void PORT_5(void)

During the interrupt, the LED is toggled, and then a delay of 1000 cycles is activated so that
the frequency is low enough for the user to see the LED blink.

After, the interrupt flag for P5.5 is cleared so the interrupt can activate again with a new button press.
