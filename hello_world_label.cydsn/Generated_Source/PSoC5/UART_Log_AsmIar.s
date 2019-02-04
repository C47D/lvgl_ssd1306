;==============================================================================
; File Name: UART_Log_AsmIar.s
; Version 1.50
;
;  Description:
;   IAR assembly routines for PSoC 5LP.
;
;==============================================================================
; Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
; You may use this file only in accordance with the license, terms, conditions,
; disclaimers, and limitations in the end user license agreement accompanying
; the software package with which this file was provided.
;==============================================================================


    IMPORT UART_Log_pinDrAdress
    IMPORT CyDelayCycles
    IMPORT CyEnterCriticalSection
    IMPORT CyExitCriticalSection
    IMPORT cydelay_freq_hz
    IMPORT __aeabi_uidiv

    SECTION .text:CODE:ROOT(2)
    PUBLIC UART_Log_PutChar
    THUMB


;-------------------------------------------------------------------------------
; Function Name: UART_Log_PutChar
;-------------------------------------------------------------------------------
;
; Summary:
;  Sends one byte via the Tx pin.
;
; Parameters:
;  TxDataByte: Byte to send
;
; Return:
;  None
;
;-------------------------------------------------------------------------------
; void UART_Log_PutChar(uint8 txDataByte)
.thumb_func
UART_Log_PutChar:
SetPinMode:
    PUSH {r1, r2, r3, r4, r5, r6, r7, lr}
    MOV r1, r9
    MOV r2, r10
    MOV r3, r11
    PUSH {r1, r2, r3}
                                    ; cycles  bytes
    MOV r7, r0                      ;   1       2
    LDR r1, PORT_DR_ADDRESS         ;   2       2
    LDR r1, [r1, #0]                ;   2       2
    MOV r10, r1                     ;   1       2   Save pin DR base address [r10]

    LDR r2, FREQ                    ;   2       2   Calculate CPU cycles to wait between bits:
    LDR r2, [r2, #0]                ;   2       2   (CPU Frequency + (UARTBaudRate/2)) / UARTBaudRate
    LDR r0, BAUD_RATE               ;   2       2

    MOVS r1, #2                     ;   1       2
    BL __aeabi_uidiv                ;   4       2
    MOVS r3, r0                     ;   1       2
    LDR r0, FREQ                    ;   2       2
    LDR r0, [r0, #0]                ;   2       2
    ADDS r0, r0, r3                 ;   1       2
    LDR r1, BAUD_RATE               ;   2       2
    BL __aeabi_uidiv                ;   4       2
    SUBS r0, r0, #20                ;   1       2   Remove overhead
    MOV r11, r0                     ;   1       2   Save delay value to [r11]

    MOVS r1, #1                     ;   1       2
    MVNS r1, r1                     ;   1       2
    MOV r4, r1                      ;   1       2   Save pin state mask to [r4]
    MOVS r5, #1                     ;   1       2   Prepare lower bit mask [r5]
    MOVS r6, #0                     ;   1       2   Prepare register for counter
    
    BL CyEnterCriticalSection       ;   4       2   Disable interrupts
    MOV r9, r0                      ;   1       2

                                    ;               Transmitting start bit
    MOV r1, r10                     ;   1       2   Set pin DR to "0"
    LDRB r3, [r1, #0]               ;   2       2
    ANDS r3, r3, r4                 ;   1       2
    STRB r3, [r1, #0]               ;   2       2
    MOV r0, r11                     ;   1       2
    ADDS r0, #1                     ;   1       2
    BL CyDelayCycles                ;   4       2

bitTxLoop:
                                    ;               Transmit bit loop
    MOVS r0, r7                     ;   1       2   Move byte to [r0]
    LSRS r0, r0, r6                 ;   1       2   Shift [r0] by counter
    ANDS r0, r0, r5                 ;   1       2   Extract lower bit
    LDRB r3, [r1, #0]               ;   2       2   Load current pin data reg
    ANDS r3, r3, r4                 ;   1       2   Apply mask to clear pin state bit
    ORRS r3, r3, r0                 ;   1       2   OR result with shifted bit value
    STRB r3, [r1, #0]               ;   2       2   Write register value
    MOV r0, r11                     ;   1       2   Move delay value to [r0]
    BL CyDelayCycles                ;   4       2   Call delay function

    ADDS r6, r6, #1                 ;   1       2   Increment counter
    CMP r6, #7                      ;   1       2   Compare counter
    BLS bitTxLoop                   ;   3(1)    2   Jump to the start of the loop
    
    MOVS r0, #0                     ;   1       2   Wait 4 extra cycles
    ADDS r0, #1                     ;   1       2
    SUBS r0, #1                     ;   1       2
    ADDS r0, #1                     ;   1       2

                                    ;               Transmitting STOP bit
    MOVS r0, #1                     ;   1       2
    LDRB r3, [r1, #0]               ;   2       2
    ANDS r3, r3, r4                 ;   1       2
    ORRS r3, r3, r0                 ;   1       2
    STRB r3, [r1, #0]               ;   2       2
    MOV r0, r11                     ;   1       2
    ADDS r0, #4                     ;   1       2   Adjust delay
    BL CyDelayCycles                ;   4       2

    MOV r0, r9                      ;   1       2
    BL CyExitCriticalSection        ;   4       2   Restore interrupt state
    NOP                             ;   1       2
    NOP                             ;   1       2
    NOP                             ;   1       2

    POP {r1, r2, r3}
    MOV r9, r1
    MOV r10, r2
    MOV r11, r3
    POP {r1, r2, r3, r4, r5, r6, r7, pc}
   
    DATA
PORT_DR_ADDRESS DCD UART_Log_pinDrAdress
FREQ DCD cydelay_freq_hz
BAUD_RATE DCD 115200

    END


;[] END OF FILE
