/*
 * ultrasonico.h - Libreria para sensor HC-SR04 (ATmega328P @ 16 MHz)
 * Usa Timer1 (prescaler 8 -> 0.5 us por cuenta) para medir el pulso ECHO.
 *
 * Conexiones: TRIG = PB3, ECHO = PB4
 */
#ifndef ULTRASONICO_H
#define ULTRASONICO_H

#include <stdint.h>

#define ULTRASONICO_SIN_ECO  0xFFFFU   /* No se detecto objeto en el rango */

void ultrasonico_init(void);

/* Devuelve la distancia en cm o ULTRASONICO_SIN_ECO si hubo timeout */
uint16_t ultrasonico_leer_cm(void);

#endif /* ULTRASONICO_H */
