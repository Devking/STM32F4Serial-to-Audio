#include <stdio.h>
#include "STM32F4xx.h"

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port8(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
