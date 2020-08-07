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


#include "../include/usart0.h"


/* Static Variables holding the ringbuffer used in IRQ mode */
static uint8_t          USART0_rxbuf[USART0_RX_BUFFER_SIZE];
static volatile uint8_t USART0_rx_head;
static volatile uint8_t USART0_rx_tail;
static volatile uint8_t USART0_rx_elements;
static uint8_t          USART0_txbuf[USART0_TX_BUFFER_SIZE];
static volatile uint8_t USART0_tx_head;
static volatile uint8_t USART0_tx_tail;
static volatile uint8_t USART0_tx_elements;

void (*USART0_rx_isr_cb)(void) = &USART0_DefaultRxIsrCb;

void (*USART0_tx_isr_cb)(void) = &USART0_DefaultTxIsrCb;

void USART0_DefaultRxIsrCb(void)
{
    uint8_t data;
    uint8_t tmphead;

    /* Read the received data */
    data = USART0.RXDATAL;
    /* Calculate buffer index */
    tmphead = (USART0_rx_head + 1) & USART0_RX_BUFFER_MASK;
        
    if (tmphead == USART0_rx_tail) {
            /* ERROR! Receive buffer overflow */
    }else {
    /*Store new index*/
    USART0_rx_head = tmphead;
    
    /* Store received data in buffer */
    USART0_rxbuf[tmphead] = data;
    USART0_rx_elements++;
    }
}

void USART0_DefaultTxIsrCb(void)
{
    uint8_t tmptail;

    /* Check if all data is transmitted */
    if (USART0_tx_elements != 0) {
        /* Calculate buffer index */
        tmptail = (USART0_tx_tail + 1) & USART0_TX_BUFFER_MASK;
        /* Store new index */
        USART0_tx_tail = tmptail;
        /* Start transmission */
        USART0.TXDATAL = USART0_txbuf[tmptail];
        
        USART0_tx_elements--;
    }

    if (USART0_tx_elements == 0) {
            /* Disable Tx interrupt */
            USART0.CTRLA &= ~(1 << USART_DREIE_bp);
    }
}

void USART0_SetISRCb(usart_callback cb, usart0_cb_t type)
{
    switch (type) {
        case USART0_RX_CB:
                USART0_rx_isr_cb = cb;
                break;
        case USART0_TX_CB:
                USART0_tx_isr_cb = cb;
                break;
        default:
                // do nothing
                break;
    }
}

void USART0_SetRXISRCb(usart_callback cb)
{
    USART0_SetISRCb(cb,USART0_RX_CB);
}

void USART0_SetTXISRCb(usart_callback cb)
{
    USART0_SetISRCb(cb,USART0_TX_CB);
}

/* Interrupt service routine for RX complete */
ISR(USART0_RXC_vect)
{
    if (USART0_rx_isr_cb != NULL)
    {
        (*USART0_rx_isr_cb)();
    }
}

/* Interrupt service routine for Data Register Empty */
ISR(USART0_DRE_vect)
{
    if (USART0_tx_isr_cb != NULL)
    {
        (*USART0_tx_isr_cb)();
    }
}

ISR(USART0_TXC_vect)
{
    USART0.STATUS |= USART_TXCIF_bm;
}

bool USART0_IsTxReady()
{
    return (USART0_tx_elements != USART0_TX_BUFFER_SIZE);
}

bool USART0_IsRxReady()
{
    return (USART0_rx_elements != 0);
}

bool USART0_IsTxBusy()
{
    return (!(USART0.STATUS & USART_TXCIF_bm));
}

bool USART0_IsTxDone()
{
    return (USART0.STATUS & USART_TXCIF_bm);
}

uint8_t USART0_Read(void)
{
    uint8_t tmptail;

    /* Wait for incoming data */
    while (USART0_rx_elements == 0)
            ;
    /* Calculate buffer index */
    tmptail = (USART0_rx_tail + 1) & USART0_RX_BUFFER_MASK;
    /* Store new index */
    USART0_rx_tail = tmptail;
    ENTER_CRITICAL(R);
    USART0_rx_elements--;
    EXIT_CRITICAL(R);

    /* Return data */
    return USART0_rxbuf[tmptail];
}

void USART0_Write(const uint8_t data)
{
    uint8_t tmphead;

    /* Calculate buffer index */
    tmphead = (USART0_tx_head + 1) & USART0_TX_BUFFER_MASK;
    /* Wait for free space in buffer */
    while (USART0_tx_elements == USART0_TX_BUFFER_SIZE)
            ;
    /* Store data in buffer */
    USART0_txbuf[tmphead] = data;
    /* Store new index */
    USART0_tx_head = tmphead;
    ENTER_CRITICAL(W);
    USART0_tx_elements++;
    EXIT_CRITICAL(W);
    /* Enable Tx interrupt */
    USART0.CTRLA |= (1 << USART_DREIE_bp);
}

void USART0_Initialize()
{
    //set baud rate register
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);
	
    //RXCIE enabled; TXCIE enabled; DREIE disabled; RXSIE enabled; LBME disabled; ABEIE disabled; RS485 OFF; 
    USART0.CTRLA = 0xD0;
	
    //RXEN enabled; TXEN enabled; SFDEN disabled; ODME disabled; RXMODE NORMAL; MPCM disabled; 
    USART0.CTRLB = 0xC0;
	
    //CMODE ASYNCHRONOUS; PMODE DISABLED; SBMODE 1BIT; CHSIZE 8BIT; UDORD disabled; UCPHA disabled; 
    USART0.CTRLC = 0x03;
	
    //DBGCTRL_DBGRUN
    USART0.DBGCTRL = 0x00;
	
    //EVCTRL_IREI
    USART0.EVCTRL = 0x00;
	
    //RXPLCTRL_RXPL
    USART0.RXPLCTRL = 0x00;
	
    //TXPLCTRL_TXPL
    USART0.TXPLCTRL = 0x00;
	

    uint8_t x;

    /* Initialize ringbuffers */
    x = 0;

    USART0_rx_tail     = x;
    USART0_rx_head     = x;
    USART0_rx_elements = x;
    USART0_tx_tail     = x;
    USART0_tx_head     = x;
    USART0_tx_elements = x;

}

void USART0_Enable()
{
    USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

void USART0_EnableRx()
{
    USART0.CTRLB |= USART_RXEN_bm;
}

void USART0_EnableTx()
{
    USART0.CTRLB |= USART_TXEN_bm;
}

void USART0_Disable()
{
    USART0.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

uint8_t USART0_GetData()
{
    return USART0.RXDATAL;
}