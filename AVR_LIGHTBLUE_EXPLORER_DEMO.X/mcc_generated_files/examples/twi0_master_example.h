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


#ifndef TWI0_MASTER_EXAMPLE_H
#define TWI0_MASTER_EXAMPLE_H

#include <stdint.h>
#include <stdio.h>
#include "../include/twi0_master.h"

uint8_t I2C0_example_read1ByteRegister(twi0_address_t address, uint8_t reg);
uint16_t I2C0_example_read2ByteRegister(twi0_address_t address, uint8_t reg);
void I2C0_example_write1ByteRegister(twi0_address_t address, uint8_t reg, uint8_t data);
void I2C0_example_write2ByteRegister(twi0_address_t address, uint8_t reg, uint16_t data);

void I2C0_example_writeNBytes(twi0_address_t address, void* data, size_t len);
void I2C0_example_readDataBlock(twi0_address_t address, uint8_t reg, void *data, size_t len);
void iI2C0_example_readNBytes(twi0_address_t address, void *data, size_t len);

#endif /* TWI0_MASTER_EXAMPLE_H */