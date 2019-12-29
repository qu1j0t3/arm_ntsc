/* ###################################################################
**     Filename    : Events.c
**     Project     : tryoutpe
**     Processor   : MKE02Z64VQH2
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-12-11, 09:38, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */


#include "io.h"

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKE02Z64QH2]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/



volatile unsigned field = 0; // low bit 1 = field 1; low bit 0 = field 2
volatile unsigned intr = 0;  // interrupt count = half line count; intr 0 (mod 525) is beginning of field
// In field 1, intr=40 marks start of sync pulse before first full line of video
// In field 2, intr=41 marks start of sync pulse before first full line of video
volatile unsigned video = 0; // set to 1 at end of horizontal blanking period, outside vertical blanking interval

// Vertical blanking interval starts at intr 0

void TI1_OnInterrupt(LDD_TUserData *UserDataPtr)
{
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

	// The ISR is called every horizontal half line = ~ 31.8µs -- about 508 clock cycles
	// LM1881 datasheet says that
	//   "the vertical sync pulse should last for at least 85% of the horizontal half line (47% of a full horizontal
	//   line). A vertical sync pulse from any standard should meet this requirement; both NTSC and PAL do meet this
	//   requirement (the serration pulse is the remainder of the period, 10% to 15% of the horizontal half line)."
	// i.e. at least 27µs
	// but the overheads are significant: at least 1µs in TU1_Interrupt,
	// at least 0.5µs in TU1_OnCounterRestart,
	// 0.25µs in TI1_OnInterrupt.
	// Then an additional 1µs to update field and intr variables.
	// This puts a limit on the long sync pulse time as it is very close to the maximum available time.
	// If we wait too long, the next interrupt will misfire too early, corrupting the output pulses.

#if 0
	// Level test
	FGPIOB_PCOR = VIDEO_PIN | PB_F6;
	NOPx32;
	FGPIOB_PTOR = VIDEO_PIN | SYNC_PIN | PB_F6;
	NOPx32;
	FGPIOB_PTOR = VIDEO_PIN;
	NOPx32;
	FGPIOB_PTOR = VIDEO_PIN | SYNC_PIN;
#endif


#if 1
	if(0b111111000000111111 & (1 << intr)) { // Equalising pulse nominal: 2.3µs +/- .1µs
		FGPIOB_PCOR = SYNC_PIN;
		NOPx32; // ~ 2.29µs
		FGPIOB_PSOR = SYNC_PIN; // end of sync pulse
	} else if(0b111111000000 & (1 << intr)) { // vertical blanking interval pulse nominal 27.1µs
		FGPIOB_PCOR = SYNC_PIN;
		// TI2 interrupt will fire and raise SYNC after ~ 27.1µs
	} else if( (intr ^ field) & 1 ) {
		// Field 1: First sync pulse at intr 18; Even numbered interrupts trigger sync pulse ... intr 40 begins first full video line
		// Field 2: First sync pulse at intr 19; Odd numbered interrupts trigger sync pulse ... intr 41 begins first full video line

		FGPIOB_PCOR = SYNC_PIN | LED_R | PB_F6; // start of sync -- vertical sync pulse nominal 4.7µs +/- .1µs

		NOPx32;
		NOPx32;
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		PE_NOP();
		// ~ 4.7µs

		FGPIOB_PSOR = SYNC_PIN | LED_R; // end of sync pulse
		// sync to blanking end should be nominal 9.4µs +/- .1µs
		NOPx32;
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();
		// ~ 9.4µs

		// Announce the horizontal start of line video, to main program
		// Always the start of the first FULL line of video, after the half line that begins field 2
		// (Expression is written this way and not as a ternary in order to have constant cycle count)
		video = intr & -((intr + (field & 1)) >= 41); // field 1: intr >= 40 ; field 2(0): intr >= 41

		//if(  (intr + (field & 1)) == 41 )
			FGPIOB_PSOR = PB_F6;
	} else {
		// no neg sync pulse
	}

	// At least 12 cycles for this update: (~ 1µs)

	if((++intr) >= 525) {
		intr = 0;
		++field;
		FGPIOB_PCOR = VIDEO_PIN; // always kill video during vertical blanking interval
	}
#endif
}

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt_LDD]
*/
/*!
**     @brief
**         Called if periodic event occur. Component and OnInterrupt
**         event must be enabled. See [SetEventMask] and [GetEventMask]
**         methods. This event is available only if a [Interrupt
**         service/event] is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer passed as
**                           the parameter of Init method.
*/
/* ===================================================================*/
void TI2_OnInterrupt(LDD_TUserData *UserDataPtr)
{
	FGPIOB_PSOR = SYNC_PIN;
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
