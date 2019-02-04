/*******************************************************************************
* File Name: UART_Log_PVT.h
* Version 1.50
*
* Description:
*  This file provides constants and parameter values for the Software Transmit
*  UART Component.
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_SW_TX_UART_UART_Log_PVT_H
#define CY_SW_TX_UART_UART_Log_PVT_H

#include "UART_Log.h"


#if(UART_Log_PIN_STATIC_MODE == 1u)
    #define UART_Log_PIN_NUMBER                 (UART_Log_tx__0__SHIFT)
    #define UART_Log_PIN_PORT_NUMBER            (UART_Log_tx__0__PORT)
    #if (CY_PSOC4)
        #define UART_Log_PIN_DR                 (UART_Log_tx__DR)
    #else
        #define UART_Log_PIN_DR                 (UART_Log_tx__0__PC)
    #endif /* (CY_PSOC4) */
    
#else
    #if (!CY_PSOC4)
        #define UART_Log_PORT_CNF_BASE          (CYREG_PRT0_PC0)
    #else
        #if (CY_PSOC4A)
            #define UART_Log_PORT_CNF_BASE          ((uint32)CYDEV_PRT0_BASE)
            #define UART_Log_PORT_CNF_SIZE          ((uint32)CYDEV_PRT0_SIZE)
            #define UART_Log_PORT_CNF_MODE_OFFSET   ((uint32)(CYREG_PRT0_PC - CYDEV_PRT0_BASE))
            #define UART_Log_PORT_CNF_DR_OFFSET     ((uint32)(CYDEV_PRT0_BASE - CYREG_PRT0_DR))     
        #else
            #define UART_Log_PORT_CNF_BASE          ((uint32)CYDEV_GPIO_PRT0_BASE)
            #define UART_Log_PORT_CNF_SIZE          ((uint32)CYDEV_GPIO_PRT0_SIZE)
            #define UART_Log_PORT_CNF_MODE_OFFSET   ((uint32)(CYREG_GPIO_PRT0_PC - CYDEV_GPIO_PRT0_BASE))
            #define UART_Log_PORT_CNF_DR_OFFSET     ((uint32)(CYDEV_GPIO_PRT0_BASE - CYREG_GPIO_PRT0_DR))
        #endif /* (CY_PSOC4A) */
    #endif /* (!CY_PSOC4) */
#endif /* (UART_Log_PIN_STATIC_MODE == 1u) */

/* Nibble Offset and Mask */
#define UART_Log_BYTE_UPPER_NIBBLE_SHIFT        (0x04u)
#define UART_Log_U16_UPPER_BYTE_SHIFT           (0x08u)
#define UART_Log_BYTE_LOWER_NIBBLE_MASK         (0x0Fu)
#define UART_Log_U16_LOWER_BYTE_MASK            (0xFFu)

#if (!CY_PSOC4)
    #define UART_Log_MAX_PORT_NUMBER            (15u)
#else
    #if (CY_PSOC4_4100 || CY_PSOC4_4200)
        #define UART_Log_MAX_PORT_NUMBER            (4u)
    #else
        #define UART_Log_MAX_PORT_NUMBER            ((-1u) - 1u)
    #endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */
#endif /* (!CY_PSOC4) */

#define UART_Log_MAX_PIN_NUMBER                 (7u)
#endif /* CY_SW_TX_UART_UART_Log_PVT_H */


/* [] END OF FILE */
