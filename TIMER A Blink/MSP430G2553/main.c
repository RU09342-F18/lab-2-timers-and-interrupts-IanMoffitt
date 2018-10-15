#include <msp430.h> 
//TimerABlink
/**
 * Using the TIMER module instead of a software loop, control the speed of two LEDS
 * blinking on your development boards. Experiment with the different counting modes
 * available as well as the effect of the pre-dividers. Why would you ever want to use
 * a pre-divider? What about the Capture and Compare registers? Your code should include a
 * function (if you want, place it in its own .c and .h files) which can convert a desired Hz
 * into the proper values required to operate the TIMER modules.
 */

//Returns the value for CCR registers
//Function to convert Hz to values for timer registers
//Pass in 0 for timerModule to set up the CCR0 register
//Pass in 1 for timerModule to set up the interrupt (CCRX) register
//resetTime is the time you want the entire cycle to be reset (in seconds)
//Pass in A for clockName to configure for AClk
//Pass in S for clockName to configure for SMClk
//Pass in M for clockName to configure for MClk
//Returns a 0 if a value other than 0 or 1 for timerModule
/*
int TimerConfig(int timerModule, float Hz, int resetTime, char clockName)
{
    //Here the frequency for the clock selected is set up by reading the variable clockName
    int clockFreq;
    int CCRValue;

    if (clockName == 'A')
    {
        clockFreq = 32768;
    }

    if (clockName == 'S')
    {
        clockFreq = 1000000;
    }

    if (clockName == 'M')
    {
        clockFreq = 16000000;
    }

    switch (timerModule)
    {
        //This sets CCR0 to an optimal value based off the desired frequency and clock frequency
        case 0:
        if (Hz >= 1)
            CCRValue = clockFreq * resetTime;

        return CCRValue;
            break;

        //This sets CCRX to an optimal value based off the desired frequency and clock frequency
        case 1:
        //1Hz is one second, so desired one over Freq * clock freq = ccr0 register
        CCRValue = (1 / Hz) * clockFreq;
        return CCRValue;
            break;

        default:
            return 0;
    }
}
*/

//The main function
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //Want to make multiple LEDs blink at different rates using interrupt
    //Same code as Lab1 part 2 but using the code as an interrupt

    //Set LED Port
    P1SEL = 0x00;
    P1DIR = 0b01000001;
    P1OUT = 0x00;

    //Enable interrupts on CCR1 and CCR2
    TA0CCTL1 = CCIE;
    TA0CCTL2 = CCIE;

    //The timer register "TAxCTL" is where all of the information for configuring the timer is. TASSEL is a macro for enabling the
    //clock to be controlled by a clock, since TASSEL_1 is chosen it is configured by the ACLK.
    //MC is how to configure the counting mode
    //MC_1 is up mode for the counter
    //TAIE is a macro that enables the timer interrupt
    //ID sets the internal divider
    //ID_0 sets the internal divider to 1, so no division is occurring
    TA0CTL = TASSEL_1 + MC_1 + TAIE + ID_0;

    //Calling upon the function to configure the CCR0 Frequency (Check TimerConfig for how this works)
    //TA0CCR0 = TimerConfig(0, 5, 1, 'A');
    TA0CCR0 = 32000;

    //CCR1 controls the LED at P1.0
    //TA0CCR1 = TimerConfig(1, 5, 1, 'A');
    TA0CCR1 = 16000;

    //CCR2 controls the LED at P1.6
    //TA0CCR2 = TimerConfig(1, 3, 1, 'A');
    TA0CCR2 = 30000;

    //Global interrupt is enabled
    __bis_SR_register(GIE);

    //An infinite loop, the interrupt will control everything after it everything is configured
    while(1)
    {}
}
/*
The While loop used in the previous function (Lab1 section 2) is now replaced in the interrupt function, where after
the interrupt vector is set, it runs the code and toggles the LED after a quarter of the "frequency" is reached.
As stated before the code runs every quarter of the frequency desired, so CCR0 is actually divided out of the function
*/

//There are two interrupt vectors that each trigger one of the bits to be toggled,


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    //This line of toggles the state of both the
    switch (TA0IV)
    {
        //This interrupt triggers when CCR1 is reached
        //Toggles both LEDs when CCR1 is reached
        case 0x02:
            P1OUT ^= 0x01;
        break;

        //This interrupt triggers when CCR2 is reached
        //Toggles one LED when CCR2 is reached
        case 0x04:
            P1OUT ^= 0x40;
        break;

    }

}

