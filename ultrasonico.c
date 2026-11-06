#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonico.h"

#define TRIG_PIN  PB3
#define ECHO_PIN  PB4

/* Con prescaler 8: 1 cuenta = 0.5 us. Distancia(cm) = t(us)/58 = cuentas/116 */
#define CUENTAS_POR_CM     116U
#define TIMEOUT_SUBIDA     30000U   /* ~15 ms esperando que ECHO suba  */
#define TIMEOUT_BAJADA     46400U   /* ~23 ms (~4 m maximo del sensor) */

void ultrasonico_init(void)
{
    DDRB  |=  (1 << TRIG_PIN);    /* TRIG salida */
    DDRB  &= ~(1 << ECHO_PIN);    /* ECHO entrada */
    PORTB &= ~(1 << TRIG_PIN);

    TCCR1A = 0;
    TCCR1B = (1 << CS11);         /* Timer1 modo normal, prescaler 8 */
}

uint16_t ultrasonico_leer_cm(void)
{
    /* Pulso de disparo de 10 us */
    PORTB &= ~(1 << TRIG_PIN);
    _delay_us(2);
    PORTB |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG_PIN);

    /* Esperar flanco de subida de ECHO */
    TCNT1 = 0;
    while (!(PINB & (1 << ECHO_PIN))) {
        if (TCNT1 > TIMEOUT_SUBIDA) return ULTRASONICO_SIN_ECO;
    }

    /* Medir el ancho del pulso */
    TCNT1 = 0;
    while (PINB & (1 << ECHO_PIN)) {
        if (TCNT1 > TIMEOUT_BAJADA) return ULTRASONICO_SIN_ECO;
    }

    return TCNT1 / CUENTAS_POR_CM;
}
