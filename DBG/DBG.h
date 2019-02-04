/*
 * DBG.h
 *
 *  Created on: 08/08/2018
 *      Author: Ingenieria6
 */

#ifndef DBG_H_
#define DBG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define BOOL_PARAM(param)	(param ? '1' : '0')

void DBG_clear_screen(void);
void DBG_println(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* DBG_H_ */
