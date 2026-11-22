/*
 * Project: Automated Conveyor Belt Sorting System with Sensors
 * Intern: Mohamed Essam Refaat | ID: CA/DF1/440462
 * MCU: ATmega32 @ 16 MHz
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define IR_ENTRY_PIN   PD2
#define TRIG_PIN       PD3
#define ECHO_PIN       PD4
#define METAL_PIN      PD5
#define HEIGHT_LIMIT   15   // Maximum valid package height in cm

void System_Init(void) {
    DDRD &= ~((1 << IR_ENTRY_PIN) | (1 << ECHO_PIN) | (1 << METAL_PIN));
    DDRD |= (1 << TRIG_PIN) | (1 << PD7);
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB3);
    DDRA |= (1 << PA0);

    // Timer0 Fast PWM for DC Conveyor Motor Speed (PB3 / OC0)
    TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS01) | (1 << CS00);
    OCR0 = 0; // Motor initially stopped
}

void Motor_Run(uint8_t duty_percent) {
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);
    OCR0 = (uint8_t)((duty_percent * 255UL) / 100UL);
}

void Motor_Stop(void) {
    PORTB &= ~((1 << PB0) | (1 << PB1));
    OCR0 = 0;
}

void Set_Servo_Angle(uint8_t angle) {
    // 1ms = 0 deg, 1.5ms = 45 deg, 2ms = 90 deg
    uint16_t pulse_us = 1000 + ((uint32_t)angle * 1000UL) / 90UL;
    PORTD |= (1 << PD7);
    for (uint16_t i = 0; i < pulse_us; i++) _delay_us(1);
    PORTD &= ~(1 << PD7);
    _delay_ms(18);
}

uint16_t Measure_Height_CM(void) {
    uint16_t count = 0;
    PORTD |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN);

    while (!(PIND & (1 << ECHO_PIN)));
    while (PIND & (1 << ECHO_PIN)) {
        count++;
        _delay_us(1);
    }
    uint16_t distance = count / 58;
    return (distance < 30) ? (30 - distance) : 0; // Sensor mounted 30cm above belt
}

int main(void) {
    System_Init();
    Set_Servo_Angle(0);

    while (1) {
        if (!(PIND & (1 << IR_ENTRY_PIN))) { // Package detected (Active Low IR)
            Motor_Run(75);
            _delay_ms(400); // Move package to inspection zone

            uint16_t height = Measure_Height_CM();
            uint8_t is_metal = !(PIND & (1 << METAL_PIN));

            if (height > HEIGHT_LIMIT) {
                PORTA |= (1 << PA0); // Trigger reject buzzer
                Set_Servo_Angle(90); // Route to Reject Bin
                _delay_ms(600);
                PORTA &= ~(1 << PA0);
            } else if (is_metal) {
                Set_Servo_Angle(45); // Route to Bin A (Metallic)
            } else {
                Set_Servo_Angle(0);  // Route to Bin B (Non-Metallic)
            }

            _delay_ms(1200);         // Allow package to clear diverter
            Set_Servo_Angle(0);
        } else {
            Motor_Stop();
        }
    }
}
