/* If you look at the Teensy 3.2 Pin Assignments (https://www.pjrc.com/teensy/pinout.html), the LED pin is pin 13. 
 *  This is not really the pin. This is the high level pin.
 *  Every microcontroller has a setup that is slightly different.
 *  Our microcontroller has several ports. Each port has several pins.
 *  In order for us to identify what pin 13 is, that is a certain pin on a certain port that Arduino calls pin 13.
 *  
 *  Let us look at the Teensy 3.2 Sechematic instead (https://www.pjrc.com/teensy/schematic.html).
 *  In the schematic diagram, the right most pins are the high level pins.
 *  So if we follow the Pin 13 (LED), we can see that it is PTC5 (Port C Pin 5).
 *  
 *  Let us now refer to the K20 Sub-Family Reference Manual (https://www.nxp.com/docs/en/reference-manual/K20P64M72SF1RM.pdf)
 *  Let us refer to Chapter 49 (page 1331)
 *  
 *  Let us now go to page 1333 and refer to the GPIO (memory map)
 *        - Width is 32 bits because this is a 32 bit processor
 *        
 *  The first thing that we need to do is set the direction to an output of Port C.
 *  Let us refer to the Port Data Direction Register (GPIOx_PDDR) in page 1337.
 *  In order for us to use this for Port C, we can call this but replace the x with C in the register name.
 *  So with this, instead of having to deal with pointers and actual addresses, we can directly do this: GPIOC_PDDR = <something> //And there is a macro that points that to the right address of that register.
 *  If you go into the Arduino folder and way deep into the library folder, you will find this file called: kinetis.h //The macro is defined here.
 *  
 *  Let us now refer to Chapter 11 (page 219)
 *  Pin Control Register n (PORTx_PCRn) in page 227.
 *  A pin can be used for different functions (such as GPIO, UART, Timer, etc)
 *  We don't have to worry about most of these stuff here for now (such as pins for Interrupts, Pull Up/Down Registers). One thing we do have to do is set the Pin Mux Control
 *  
 *  Now we need to back to Chapter 49 and refer to Port Data Output Register (GPIOx_PDOR) in page 1335.
 */



void setup() {

  GPIOC_PDDR |= (1U << 5); // 1U, where the U is unsigned int. So 1U automatically makes 00000000000000000000000000000001. So that 1 bit is being shifted to the 5th position (because the pin that we are interested in is Port C Pin 5.
                           // So what it is actually doing is setting Port C Pin 5, and doing nothing to the other bits. When you set it to a 1, it is output, and 0 is input.
  PORTC_PCR5 |= 0x00000100; // For MUX, if you want to have a GPIO, pins 10-8 have to be 001. This method is actually not safe because it does set bit 8 to 1, but does not explicitly set bits 9 and 10 to 0.
                             // It is fine for now because by default, all pins are set to 0. But had this pin already have some value beforehand, this method is not safe. To be safe, we should've followed this by an AND for pins 9 and 10.
}

void loop() {

  GPIOC_PDOR |= (1U << 5); // portC5 data output register
  delay(1000); // The other way we could've done this was with a loop that loops over a certain number of NOP cycles. But the right way to do it is to set up a timer, and get an interrupt. 
  GPIOC_PDOR &= ~(1U << 5); // portC5 data output register
  delay(1000); 

}
