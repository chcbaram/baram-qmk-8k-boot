#ifndef BOOT_H_
#define BOOT_H_


#include "ap_def.h"

bool     bootInit(void);
uint16_t bootVerifyUpdate(void);
uint16_t bootVerifyFirm(void);
uint16_t bootUpdateFirm(void);
uint16_t bootUpdateFirmFromFile(const char *file_name);
uint16_t bootJumpFirm(void);

#endif