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
#include "TI2.h"
#include "TU2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#include <stdint.h>

#include "io.h"

// To disassemble:
// $ /Applications/KDS_v3.app/Contents/toolchain/bin/arm-none-eabi-objdump -d Debug/Sources/main.o

// We want two interrupts for every line time.
// The ideal line freq = 15,734.264

// goal: 31,468 Hz /2 = measured: 15,873
// adjusted target freq: 31,192 ; measured 15,718

// Overall frame rate (525 lines) measured 29.938 Hz

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  FGPIOB_PSOR = LED_R | LED_G | LED_B;

/*
  __asm volatile (
		  "str %1,[%0]\n"
		  : // no outputs
		  :
		  "r" (__vect_table.__fun + 0x25),
		  "r" (test_isr)
  );
*/

  // We need to carefully arrange the skew between TU1 and TU2 interrupts.
  // TU1 is the usual sync interrupt. TU2 is used to raise the SYNC line
  // for the "long" (27.1µs) pulses, since a single ISR is too close to the
  // timer period to work reliably.
  // The following initialisations are therefore carefully timed:

  /*PIT_MCR = 0; */

  // Must turn off interrupts because we're going to poll instead
  /*
  PIT_TCTRL0 = PIT_TCTRL1 = 0;
  PIT_TFLG0  = PIT_TFLG_TIF_MASK; // clear interrupt flag
  PIT_TFLG1  = PIT_TFLG_TIF_MASK; // clear interrupt flag
   */
  PIT_TCTRL0 = PIT_TCTRL1 = PIT_TCTRL_TEN_MASK;

  // wait for timer expiry
  while(!(PIT_TFLG0 & PIT_TFLG_TIF_MASK)) ;
  PIT_TFLG0  = PIT_TFLG_TIF_MASK; // clear interrupt flag
  PIT_TCTRL0 = 0; // disable timer

  while(!(PIT_TFLG1 & PIT_TFLG_TIF_MASK)) ; // should take 4-5 cycles each poll (LDR, TST, BEQ) so could be problematic with periods < 6 cycles?
  PIT_TFLG1  = PIT_TFLG_TIF_MASK; // clear interrupt flag
  PIT_TCTRL1 = 0; // disable timer

  //PIT_LDVAL0 = PIT_LDVAL1 = 0x70; // values less than this result in TI2 interrupt being lost (!)

  PIT_TCTRL0 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
  // This delay is also dependent on the overheads before the SYNC pulse is lowered in the vertical sync region
  NOPx32;NOPx32;NOPx32; // ~6µs
  NOPx32;NOPx32;NOPx32; // ~6µs
  NOPx32;NOPx32;NOPx32; // ~6µs
  NOPx32;NOPx32;NOPx32; // ~6µs
  NOPx32;NOPx32;NOPx32;
  PIT_TCTRL1 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; // re-enable timers


  for(;;) {
	  // wait for video flag
	  // when set by ISR, (field & 1) will be 1 (field 1) or 0 (field 2)
	  // intr will be a line counter, field 1: 40, 42, 44, 46... ; field 2: 41, 43, 45, 47...
	  // timing is up to this routine to bit bang the horizontal video signal

	  unsigned i;
	  while(!(i = video))
		  ;

	  if (i & 32)
	  {
		  FGPIOB_PSOR = VIDEO_PIN | LED_B;
		  PE_NOP();
		  PE_NOP();
		  PE_NOP();
		  PE_NOP();
		  FGPIOB_PCOR = VIDEO_PIN | LED_B;
	  }

	  while(video == i)
		  ;

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
