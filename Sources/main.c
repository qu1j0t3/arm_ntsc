/* ###################################################################
**     Filename    : main.c
**     Project     : tryoutpe
**     Processor   : MKE02Z64VQH2
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-12-11, 09:38, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TI1.h"
#include "TU1.h"
#include "GPIO1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#define LED_R (1<<25) // PTH1
#define LED_G (1<<26) // PTH2
#define LED_B (1<<7)  // PTE7

#define PB_F6 (1<<14)
#define PB_F7 (1<<15)

volatile x;

// We want two interrupts for every line time.
// The ideal line freq = 15,734.264

// goal: 31,468 Hz /2 =
// measured: 15,873
// adjusted target freq: 31,192 ; measured 15,719

void my_isr() {
	// Following Grass Valley NTSC Studio Timing pdf
	// Interrupt fires every half line
	// Field 1
	// Line 1,2,3: every int neg sync pulse 2.3µs (equalising)
	// = int 0,1,2,3,4,5
	// Line 4,5,6: every int long neg sync pulse 27.1µs (vertical sync)
	// = int 6,7,8,9,10,11
	// Line 7,8,9: same as 1,2,3 (equalising)
	// = int 12,13,14,15,16,17
	// Lines 10-20: every 2nd int (every line) long neg sync pulse ???µs (horizontal pulses no video)
	// = int 18,20,...,38   (10 long pulses)
	// Lines 21-262: video full lines
	// = int 40,42,...524 begins a half line of video
	// at int 525 (= 0 mod 525), field 2 begins, but line 1 is counted from field 2 int 1
	// pulse train is same as field 1 relative to interrupt number in field,
	// but the 2nd set of equalizing pulses skips the last pulse
	// horizontal non-video lines begin at int 19 (rather than int 18 as in field 1)
	// the first half-line of video begins at int 39 (sync pulse at int 38)
	GPIOB_PTOR = PB_F6;
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	unsigned i, j;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/


  //GPIOB_PDDR = LED_R | LED_G | LED_B | PB_F6; // 0 = input, 1 = output
  GPIOB_PIDR = ~0; // all inputs disabled; inputs are high-Z if disabled in GPIOx_PIDR

  GPIOB_PSOR = LED_R | LED_G | LED_B;
  for(;;) {
	  /*
	  for(j = 6; j--;) {
		  //GPIOB_PTOR = LED_R;
		  for(i = 300000; --i; ) ;
	  }
	  for(j = 6; j--;) {
		  GPIOB_PTOR = LED_G;
		  for(i = 300000; --i; ) ;
	  }
	  for(j = 6; j--;) {
		  GPIOB_PTOR = LED_B;
		  for(i = 300000; --i; ) ;
	  }
	  */
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
