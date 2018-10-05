#include <msp430.h> 
/**
 *This code is implementing the button pushing function using interrupts
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //The initial setup for the msp430g2553

    //First set the Port 1 to be IO
    P1SEL = 0x00;
    //Set the direction of Port 1 to be input for button without damaging contents of the register
    P1DIR &= 0xF7;
    //Set the direction of Port 1 to be the output for the LED
    P1DIR |= 0x01;
    //Ensure the LED is Off initially and button is push down
    P1OUT = 0x08;

    //Enable an interrupt for the button (Port 1.3)
    P1IE = 0x08;
    //Read the set the flag at the rising edge
    P1IES = 0x00;

    //Enable Low Power mode and have the global interrupt (any interrupt is permitted) enabled
    __bis_SR_register(LPM4_bits + GIE);

    //Enter an infinite loop, the interrupt is what causes the button to blink
    while(1)
    {
    }
}

//This is the code that runs when an interrupt at port 1.3 occurs
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{

        //Take the input from the button and take out only the bit for the button
        int input;
        input = P1IN & 0x08;

        //Enter and stay in a loop if the input is read as on
        //It is does not equal due to when testing the loop would enter when the button was not pressed
        //This led me to believe that the Push button is a Push button normally closed (On by default)
        //So the loop will now be entered when the button is clicked
        while(input == 0x08)
        {
            P1OUT = 0x01;
            input = P1IN & 0x08;
        }

        //Set the state of the LED to Off again
        P1OUT = 0x00;

        //Clear the flags
        P1IFG = 0x00;

}
