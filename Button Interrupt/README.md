# Button Interrupt
Here is the toggle button code from lab one, except it uses an interrupt to change the state of the LED. The code can be categorized into two sections, pre and post interrupt

## Pre-Interrupt
The code before the global interrupt is enabled is mostly to configure the LED at Port1.0. This is the same as we've seen before in Lab1 with configuring the select, and direction registers. After the LED is configured there are a couple of lines of code which set the button to be set as a trigger for the interrupt, and when to trigger the interrupt on either the rising or falling edge of the clock cycle. After the button is set as an interrupt, a function is called which enables interrupts to happen anywhere, along with activating low power mode.

## Post-Interrupt
After the interrupt is triggered the LED is then toggled from off to on. Once it is on it enters a while loop which constantly checks whether or not the button is still pressed, if not it turns the LED off. When the entire function finishes 
