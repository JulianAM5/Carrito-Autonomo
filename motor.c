/*
 * motor.c - Implementacion de la libreria de motores (L298N + ATmega328P)
 */

#include <avr/io.h>
#include <util/delay.h>
#include "motor.h"

/* ---------- Definicion de pines ---------- */
/* Motor izquierdo (A) */
#define IN1_PORT  PORTD
#define IN1_DDR   DDRD
#define IN1_PIN   PD7
#define IN2_PORT  PORTB
#define IN2_DDR   DDRB
#define IN2_PIN   PB0
/* Motor derecho (B) */
#define IN3_PORT  PORTB
#define IN3_DDR   DDRB
#define IN3_PIN   PB1
#define IN4_PORT  PORTB
#define IN4_DDR   DDRB
#define IN4_PIN   PB2
/* Enables (PWM): ENA = PD6 (OC0A), ENB = PD5 (OC0B) */

static uint8_t velocidad_actual = VELOCIDAD_BAJA;

/* ---------- Funciones internas ---------- */

static void delay_ms_var(uint16_t ms)
{
    while (ms--) {
        _delay_ms(1);   /* _delay_ms requiere constante en tiempo de compilacion */
    }
}

static void aplicar_pwm(uint8_t porcentaje)
{
    uint8_t duty = (uint8_t)(((uint16_t)porcentaje * 255U) / 100U);
    OCR0A = duty;   /* motor izquierdo */
    OCR0B = duty;   /* motor derecho   */
}

/* Direccion de cada motor: 1 = adelante, -1 = atras, 0 = freno */
static void motor_izq(int8_t sentido)
{
    if (sentido > 0)      { IN1_PORT |=  (1 << IN1_PIN); IN2_PORT &= ~(1 << IN2_PIN); }
    else if (sentido < 0) { IN1_PORT &= ~(1 << IN1_PIN); IN2_PORT |=  (1 << IN2_PIN); }
    else                  { IN1_PORT |=  (1 << IN1_PIN); IN2_PORT |=  (1 << IN2_PIN); }
}

static void motor_der(int8_t sentido)
{
    if (sentido > 0)      { IN3_PORT |=  (1 << IN3_PIN); IN4_PORT &= ~(1 << IN4_PIN); }
    else if (sentido < 0) { IN3_PORT &= ~(1 << IN3_PIN); IN4_PORT |=  (1 << IN4_PIN); }
    else                  { IN3_PORT |=  (1 << IN3_PIN); IN4_PORT |=  (1 << IN4_PIN); }
}

/* ---------- API publica ---------- */

void motor_init(void)
{
    /* Pines de direccion como salida */
    IN1_DDR |= (1 << IN1_PIN);
    IN2_DDR |= (1 << IN2_PIN);
    IN3_DDR |= (1 << IN3_PIN);
    IN4_DDR |= (1 << IN4_PIN);

    /* Pines de PWM como salida: PD6 (OC0A) y PD5 (OC0B) */
    DDRD |= (1 << PD6) | (1 << PD5);

    /* Timer0: Fast PWM 8 bits, salidas no invertidas en OC0A y OC0B,
     * prescaler 64 -> f_PWM = 16 MHz / (64 * 256) = 976 Hz */
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);

    velocidad_actual = VELOCIDAD_BAJA;
    aplicar_pwm(velocidad_actual);
    detener();
}

void motor_set_velocidad(uint8_t porcentaje)
{
    if (porcentaje > 100) porcentaje = 100;
    velocidad_actual = porcentaje;
    aplicar_pwm(porcentaje);
}

uint8_t motor_get_velocidad(void)
{
    return velocidad_actual;
}

void mover_adelante(void)
{
    motor_izq(1);
    motor_der(1);
}

void mover_atras(void)
{
    motor_izq(-1);
    motor_der(-1);
}

void detener(void)
{
    motor_izq(0);   /* freno: IN_A = IN_B = 1 */
    motor_der(0);
}

void girar_continuo(direccion_giro_t dir)
{
    if (dir == DIR_DERECHA) {   /* izquierdo avanza, derecho retrocede */
        motor_izq(1);
        motor_der(-1);
    } else {                    /* derecho avanza, izquierdo retrocede */
        motor_izq(-1);
        motor_der(1);
    }
}

void girar(direccion_giro_t dir, uint16_t grados)
{
    /* Tiempo base a 100% proporcional a los grados; a menor velocidad
     * se necesita mas tiempo (compensacion aproximada, calibrar). */
    uint8_t v = (velocidad_actual == 0) ? 1 : velocidad_actual;
    uint32_t t = ((uint32_t)TIEMPO_GIRO_90_MS_A_100 * grados * 100UL) / (90UL * v);

    girar_continuo(dir);
    delay_ms_var((uint16_t)t);
    detener();
}

void girar_derecha_90(void)
{
    girar(DIR_DERECHA, 90);
}

void girar_izquierda_90(void)
{
    girar(DIR_IZQUIERDA, 90);
}
