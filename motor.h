/*
 * motor.h - Libreria de control de motores DC con driver L298N
 * Microcontrolador: ATmega328P @ 16 MHz
 *
 * PWM por hardware con Timer0 (Fast PWM 8 bits, prescaler 64 -> ~976 Hz)
 *
 * Conexiones:
 *   Motor izquierdo (canal A): ENA = PD6 (OC0A), IN1 = PD7, IN2 = PB0
 *   Motor derecho   (canal B): ENB = PD5 (OC0B), IN3 = PB1, IN4 = PB2
 */
#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/* Niveles de velocidad (porcentaje) */
#define VELOCIDAD_BAJA    33
#define VELOCIDAD_MEDIA   66
#define VELOCIDAD_ALTA    100

/* Tiempo (ms) que tarda el robot en girar 90 grados sobre su eje a 100% de
 * velocidad. ES UN VALOR A CALIBRAR segun tu chasis, motores y bateria. */
#define TIEMPO_GIRO_90_MS_A_100   400

typedef enum {
    DIR_DERECHA = 0,
    DIR_IZQUIERDA
} direccion_giro_t;

/* Configura pines y Timer0. Velocidad inicial = VELOCIDAD_BAJA, motores detenidos. */
void motor_init(void);

/* Establece la velocidad (0-100 %). Se aplica de inmediato aunque el robot este en movimiento. */
void motor_set_velocidad(uint8_t porcentaje);

/* Devuelve la velocidad actual en porcentaje. */
uint8_t motor_get_velocidad(void);

/* Movimientos continuos (no bloqueantes) */
void mover_adelante(void);
void mover_atras(void);
void detener(void);

/* Giro continuo sobre su propio eje (no bloqueante) */
void girar_continuo(direccion_giro_t dir);

/* Giro de 'grados' (bloqueante, basado en tiempo, se compensa segun la velocidad).
 * Al terminar el robot queda detenido. */
void girar(direccion_giro_t dir, uint16_t grados);

/* Atajos para giros de 90 grados */
void girar_derecha_90(void);
void girar_izquierda_90(void);

#endif /* MOTOR_H */
