/*
 * Practica 8: Robot movil diferencial
 * ATmega328P @ 16 MHz, driver L298N, sensor HC-SR04
 *
 * Interruptores de velocidad (con pull-up interno, activos en bajo):
 *   PC0 ON -> 66 %      PC1 ON -> 100 %      ninguno -> 33 %
 *   (si ambos estan ON tiene prioridad 100 %)
 */

#include <avr/io.h>
#include <util/delay.h>
#include "motor.h"
#include "ultrasonico.h"

#define DISTANCIA_OBSTACULO_CM   20
#define SW_66   PC0
#define SW_100  PC1

static void switches_init(void)
{
    DDRC  &= ~((1 << SW_66) | (1 << SW_100));   /* entradas */
    PORTC |=  ((1 << SW_66) | (1 << SW_100));   /* pull-up interno */
}

static uint8_t leer_nivel_velocidad(void)
{
    if (!(PINC & (1 << SW_100))) return VELOCIDAD_ALTA;
    if (!(PINC & (1 << SW_66)))  return VELOCIDAD_MEDIA;
    return VELOCIDAD_BAJA;
}

int main(void)
{
    motor_init();
    ultrasonico_init();
    switches_init();

    _delay_ms(1000);   /* tiempo para colocar el robot */

    while (1) {
        /* Actualizar velocidad segun los interruptores */
        motor_set_velocidad(leer_nivel_velocidad());

        /* Obtener datos de proximidad */
        uint16_t distancia = ultrasonico_leer_cm();

        if (distancia != ULTRASONICO_SIN_ECO && distancia <= DISTANCIA_OBSTACULO_CM) {
            /* Hay obstaculo: alto y gira 90 grados */
            detener();
            _delay_ms(200);
            girar_derecha_90();
            _delay_ms(200);
        } else {
            mover_adelante();
        }

        _delay_ms(60);   /* periodo entre mediciones (evita ecos residuales) */
    }
}
