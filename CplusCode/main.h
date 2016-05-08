#ifndef __MAIN_H__
#define __MAIN_H__

#define LOBYTE(w)   ((uint8_t)(w))
#define HIBYTE(w)   ((uint8_t)(((uint16_t)(w) >> (uint8_t)8) & 0xff))

#endif