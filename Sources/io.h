/*
 * io.h
 *
 *  Created on: Dec 17, 2019
 *      Author: toby
 */

#ifndef SOURCES_IO_H_
#define SOURCES_IO_H_


#define LED_R (1<<25) // PTH1
#define LED_G (1<<26) // PTH2
#define LED_B (1<<7)  // PTE7

#define PB_F6 (1<<14) // on my board this pin has bad levels (1v/3.1v) (broken?) -- can still be used for triggering
#define PB_F7 (1<<15)
#define PB_G1 (1<<17)
#define PB_H6 (1<<30)

#define SYNC_PIN PB_G1
#define VIDEO_PIN PB_F7


// 4 nops ~ 569ns
// 10 nops ~ 942ns
// 30 nops ~ 2.18µs
// 32 nops ~ 2.30µs
// 64 nops ~ 4.29µs
#define NOPx32 \
	PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); \
	PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); \
	PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); \
	PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();

#define NOPx16 \
	PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); \
	PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP(); PE_NOP();

extern volatile unsigned field;
extern volatile unsigned intr;
extern volatile unsigned video;


#endif /* SOURCES_IO_H_ */
