# Read Me for Button based delay
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
TA0CTL is configured using the desired settings. We used Timer A, up mode, and a divider of 2 so our code was:
TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, and divider 2.
Next, the timer is put into compare mode and TA0CCR0 is set to 6000 for up mode.

The program uses the falling edge and rising edge of the Button to change TA0CCR0.
When the button is pressed, the button interrupt is triggered, and the first if condition is 
entered as buttonPressed = 0. TA1CCR0 is set to capture mode. This will copy the value of TA1R later. 
buttonPressed is set to 1.

When the button is released, the button interrupt is triggered again, and TA1R is assigned to TA1CCR0.
When the timer counts up again, it will count up to the new value of TA1CCR0, causing the frequency to be
designated by the user. Timer A1 is cleared, and buttonPressed is set to 0, ready for a new button press.
