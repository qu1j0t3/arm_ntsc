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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

#define LED_R (1<<25) // PTH1
#define LED_G (1<<26) // PTH2
#define LED_B (1<<7)  // PTE7


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	unsigned i, j;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  GPIOB_PDDR = LED_R | LED_G | LED_B; // 0 = input, 1 = output
		  // inputs are high-Z if disabled in GPIOx_PIDR
  GPIOB_PIDR = ~0; // all inputs disabled

  GPIOB_PSOR = LED_R | LED_G | LED_B;
  for(;;) {
	  for(j = 6; j--;) {
		  GPIOB_PTOR = LED_R;
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
