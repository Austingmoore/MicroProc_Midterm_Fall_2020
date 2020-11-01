#include <msp430g2553.h>
#include <LiquidCrystal.h>
#include <stdbool.h>

bool isArmed = false;
bool alarmOn = false;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	        // Stop watchdog timer
	
	P1DIR &= ~(BIT5 | BIT6 | BIT7);     // Set P1 bits to input
	P1REN |= BIT6 | BIT7;               // Enable pullup-pulldown resistors
	P1OUT &= ~(BIT6 | BIT7);            // Configure bits to pulldown

	P2SEL = 0;                          // Select I/O function
	P2SEL2 = 0;

	P2DIR = 0xFF;                       // Set P2 bits to output
	P2OUT = 0x00;                       // Set all bits to LOW

	LCD_init();                         // Initializes LCD

	P1IE |= (BIT5 | BIT6 | BIT7);       // Enables P1 interrupts for bits
	P1IES &= ~(BIT5 | BIT6 | BIT7);     // Sets interrupt edge sensitivity to LOW->HIGH
	P1IFG &= ~(BIT5 | BIT6 | BIT7);     // Turns off all inturrupt flags

	LCD_Line1();                        // Move cursor to line 1
	LCD_txt("Alarm System");            // Display text
	LCD_Line2();                        // Move cursor to line 2
	LCD_txt("SW1:ON SW2:OFF");          // Display text

    _BIS_SR(LPM4_bits + GIE);           // Enable LPM4 and maskables

	while(1){}                          // Loop forever
	return 0;
}

//-------------------------------------------------------
// Interrupt Handler for P1
//-------------------------------------------------------
#pragma vector = PORT1_VECTOR
__interrupt void Port1_IH(void){

    if (P1IFG&BIT6 && !isArmed){        // If arm button is pressed and system is currently not armed
        LCD_Line1();                    // Move cursor to line 1
        LCD_txt("Arming System...");    // Display text
        LCD_Countdown_10to0();          // Display countdown from 10
        LCD_clear();                    // Clear display
        LCD_Line1();                    // Move cursor to line 1
        LCD_txt("System Armed");        // Display text

        isArmed = true;                 // Set isArmed to true
        P1IFG &= ~BIT6;                 // Clear flag

    }
    else if (P1IFG&BIT7 && isArmed){    // If disarm button is pressed and system is currently armed
        LCD_clear();                    // Clear display
        LCD_Line1();                    // Move cursor to line 1
        LCD_txt("System Disarmed");     // Display text

        isArmed = false;                // Set isArmed to false
        alarmOn = false;                // Set alarmOn to false
        P2OUT &= ~BIT7;                 // Turn off buzzer alarm
        P1IFG &= ~BIT7;                 // Clear flag
    }
    else if (P1IFG&BIT5 && isArmed && !alarmOn){    // If motion is detected, the system is armed, and alarm is currently off
        LCD_clear();                    // Clear display
        LCD_Line1();                    // Move cursor to line 1
        LCD_txt("Motion Detected");     // Display text
        LCD_Line2();                    // Move cursor to line 2
        LCD_txt("Sounding Alarm");      // Display text

        alarmOn = true;                 // Sets alarmOn to true
        P2OUT |= BIT7;                  // Turn on buzzer alarm
    }
    P1IFG &= ~BIT5;                 // Clear flag after every interrupt
}

