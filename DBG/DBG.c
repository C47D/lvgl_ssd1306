/*
 * DBG.c
 *
 *  Created on: 08/08/2018
 *      Author: Ingenieria6
 */

#include <string.h>
#include <stdio.h>

#include "project.h"

#include "DBG.h"

char buffer[500] = "";

void DBG_clear_screen(void)
{
    UART_Log_PutChar(0x0C);
}

void DBG_println(const char *fmt, ...)
{
    memset(buffer, '\0', sizeof buffer);
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

    UART_Log_PutString(buffer);
    UART_Log_PutString("\r\n");
}
