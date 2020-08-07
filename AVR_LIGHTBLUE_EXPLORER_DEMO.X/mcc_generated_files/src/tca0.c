/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   1.0.0
*/
/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/


#include "../include/tca0.h"

void TCA0_DefaultCMP0IsrCallback(void);
void (*TCA0_CMP0_isr_cb)(void) = &TCA0_DefaultCMP0IsrCallback;
void TCA0_DefaultCMP1IsrCallback(void);
void (*TCA0_CMP1_isr_cb)(void) = &TCA0_DefaultCMP1IsrCallback;
void TCA0_DefaultCMP2IsrCallback(void);
void (*TCA0_CMP2_isr_cb)(void) = &TCA0_DefaultCMP2IsrCallback;
void TCA0_DefaultOVFIsrCallback(void);
void (*TCA0_OVF_isr_cb)(void) = &TCA0_DefaultOVFIsrCallback;

void TCA0_DefaultCMP0IsrCallback(void)
{
    //Add your ISR code here
}

void TCA0_DefaultCMP1IsrCallback(void)
{
    //Add your ISR code here
}

void TCA0_DefaultCMP2IsrCallback(void)
{
    //Add your ISR code here
}

void TCA0_DefaultOVFIsrCallback(void)
{
    //Add your ISR code here
}

void TCA0_SetOVFIsrCallback(TCA0_cb_t cb)
{
    TCA0_OVF_isr_cb = cb;
}

void TCA0_SetCMP0IsrCallback(TCA0_cb_t cb)
{
    TCA0_CMP0_isr_cb = cb;
}

void TCA0_SetCMP1IsrCallback(TCA0_cb_t cb)
{
    TCA0_CMP1_isr_cb = cb;
}

void TCA0_SetCMP2IsrCallback(TCA0_cb_t cb)
{
    TCA0_CMP2_isr_cb = cb;
}

ISR(TCA0_CMP0_vect)
{
    if (TCA0_CMP0_isr_cb != NULL)
        (*TCA0_CMP0_isr_cb)();
    
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCA0_CMP1_vect)
{
    if (TCA0_CMP1_isr_cb != NULL)
        (*TCA0_CMP1_isr_cb)();
    
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP1_bm;
}

ISR(TCA0_CMP2_vect)
{
    if (TCA0_CMP2_isr_cb != NULL)
        (*TCA0_CMP2_isr_cb)();
    
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP2_bm;
}

ISR(TCA0_OVF_vect)
{
    if (TCA0_OVF_isr_cb != NULL)
        (*TCA0_OVF_isr_cb)();
    
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}


/**
 * \brief Initialize TCA interface
 */
int8_t TCA0_Initialize()
{
    //Compare 0
    TCA0.SINGLE.CMP0 = 0x4C4A;

    //Compare 1
    TCA0.SINGLE.CMP1 = 0x4C4A;

    //Compare 2
    TCA0.SINGLE.CMP2 = 0x4C4A;

    //Count
    TCA0.SINGLE.CNT = 0x00;

    //CMP2EN disabled; CMP1EN disabled; CMP0EN disabled; ALUPD disabled; WGMODE NORMAL; 
    TCA0.SINGLE.CTRLB = 0x00;

    //CMP2OV disabled; CMP1OV disabled; CMP0OV disabled; 
    TCA0.SINGLE.CTRLC = 0x00;

    //SPLITM disabled; 
    TCA0.SINGLE.CTRLD = 0x00;

    //CMD NONE; LUPD disabled; DIR disabled; 
    TCA0.SINGLE.CTRLECLR = 0x00;

    //CMD NONE; LUPD disabled; DIR UP; 
    TCA0.SINGLE.CTRLESET = 0x00;

    //CMP2BV disabled; CMP1BV disabled; CMP0BV disabled; PERBV disabled; 
    TCA0.SINGLE.CTRLFCLR = 0x00;

    //CMP2BV disabled; CMP1BV disabled; CMP0BV disabled; PERBV disabled; 
    TCA0.SINGLE.CTRLFSET = 0x00;

    //DBGRUN disabled; 
    TCA0.SINGLE.DBGCTRL = 0x00;

    //EVACT POSEDGE; CNTEI disabled; 
    TCA0.SINGLE.EVCTRL = 0x00;

    //CMP2 disabled; CMP1 disabled; CMP0 disabled; OVF disabled; 
    TCA0.SINGLE.INTCTRL = 0x00;

    //CMP2 disabled; CMP1 disabled; CMP0 disabled; OVF disabled; 
    TCA0.SINGLE.INTFLAGS = 0x00;

    //Period
    TCA0.SINGLE.PER = 0x4C4A;

    //Temporary data for 16-bit Access
    TCA0.SINGLE.TEMP = 0x00;

    //CLKSEL DIV1024; ENABLE enabled; 
    TCA0.SINGLE.CTRLA = 0x0F;

    return 0;
}

void TCA0_WriteTimer(uint16_t timerVal)
{
    TCA0.SINGLE.CNT=timerVal;
}

uint16_t TCA0_ReadTimer(void)
{
    uint16_t readVal;

    readVal = TCA0.SINGLE.CNT;

    return readVal;
}

void TCA0_EnableInterrupt(void)
{
        TCA0.SINGLE.INTCTRL = 1 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */
}
void TCA0_DisableInterrupt(void)
{
    TCA0.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: disabled */
}
void TCA0_ClearOverflowInterruptFlag(void)
{
    TCA0.SINGLE.INTFLAGS &= ~TCA_SINGLE_OVF_bm; /* Overflow Interrupt: disabled */
}
bool TCA0_IsOverflowInterruptEnabled(void)
{
    return ((TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm) > 0);
}
