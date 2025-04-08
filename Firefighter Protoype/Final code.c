#define F_CPU 16000000L
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include <avr/io.h>
#include <util/delay.h>

#define US_PORT PORTD
#define US_PIN PIND
#define US_DDR DDRD
#define US_POS PORTD4
#define US_ERROR 0xffff
#define US_NO_OBSTACLE 0xfffe

int r;
int RightDistance, LeftDistance, FrontDistance;
volatile char x;
TaskHandle_t xHandle;

static void vADCRead(void* pvParameters);
static void ObjectAvoidance(void* pvParameters);
static void DCPump(void* pvParameters);

uint16_t getPulseWidth() {
    DDRC |= (1 << DDC2);
    uint32_t i, result;

    // Wait for the rising edge
    for (i = 0; i < 600000; i++) {
        if (!(US_PIN & (1 << PORTD7))) continue;
        else break;
    }
    if (i == 600000)
        return 0xffff; // Indicates time out

    // High Edge Found
    // Setup Timer1
    TCCR1A = 0x00;
    TCCR1B = (1 << CS11); // Prescaler = Fcpu/8
    TCNT1 = 0x00; // Init counter

    // Now wait for the falling edge
    for (i = 0; i < 600000; i++) {
        if (US_PIN & (1 << PORTD7)) {
            if (TCNT1 > 60000) break;
            else continue;
        } else
            break;
    }
    if (i == 600000)
        return 0xffff; // Indicates time out

    // Falling edge found
    result = TCNT1;

    // Stop Timer
    TCCR1B = 0x00;

    if (result > 2500) {
        PORTC &= ~(1 << PORTC2);
        return 0xfffe; // No obstacle
    } else {
        PORTC |= (1 << PORTC2);
        return result;
    }
}

int scan() {
    // Set Ultra Sonic Port as out
    US_DDR |= (1 << US_POS);
    _delay_us(10);

    // Give the US pin a 15us High Pulse
    US_PORT |= (1 << US_POS); // High
    _delay_us(15);
    US_PORT &= (~(1 << US_POS)); // Low
    _delay_us(20);

    // Now make the pin input
    US_DDR &= (~(1 << DDD7));

    // Measure the width of pulse
    r = getPulseWidth();
    return r;
}

void init_Timer_for_Servo() {
    DDRB |= (1 << 1);
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // NON Inverted PWM
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10); // PRESCALER=64 MODE 14(FAST PWM)
    ICR1 = 4999; // fPWM=50Hz
    TCNT1 = 0;
}

void motorA_forward() {
    PORTD |= (1 << 1); // 1 is High
    PORTD &= ~(1 << 0); // 0 is Low
}

void motorB_forward() {
    PORTD |= (1 << 3); // 3 is High
    PORTD &= ~(1 << 2); // 2 is Low
}

void forward() {
    motorB_forward();
    motorA_forward();
}

void motorA_backward() {
    PORTD |= (1 << 0); // 0 is High
    PORTD &= ~(1 << 1); // 1 is Low
}

void motorB_backward() {
    PORTD |= (1 << 2); // 2 is High
    PORTD &= ~(1 << 3); // 3 is Low
}

void backward() {
    motorA_backward();
    motorB_backward();
}

void motorA_stop() {
    PORTD &= ~(1 << 0); // 0 is LOW
    PORTD &= ~(1 << 1); // 1 is LOW
}

void motorB_stop() {
    PORTD &= ~(1 << 2); // 2 is LOW
    PORTD &= ~(1 << 3); // 3 is LOW
}

void push_break() {
    motorA_stop();
    motorB_stop();
}

void spin() {
    motorA_forward();
    motorB_backward();
    _delay_ms(500);
}

void left() {
    motorA_stop();
    motorB_forward();
    _delay_ms(500);
}

void right() {
    motorA_forward();
    motorB_stop();
    _delay_ms(500);
}

void navigate() {
    init_Timer_for_Servo();
    OCR1A = 120; // right
    _delay_ms(500);
    RightDistance = scan();
    if (RightDistance == 0xfffe) { // no obstacle found on the right
        right();
    } else {
        init_Timer_for_Servo();
        OCR1A = 555; // left
        _delay_ms(500);
        LeftDistance = scan();
        if (LeftDistance == 0xfffe) { // no obstacle found on the left
            left();
        } else if (!(LeftDistance == 0xfffe)) { // obstacle found on both sides
            spin();
        }
    }
}

static void vADCRead(void* pvParamters) { // for motion
    DDRD |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6);
    TCCR0A = 0b10100001;
    TCCR0B = 0b00000010;
    OCR0A = 255;
    OCR0B = 255;

    // for ADC
    DDRC = 0b00000000;
    ADCSRA |= 0b00000111;
    ADCSRA |= 0b10000000;
    DIDR0 = 0xff;

    while (1) {
        PORTD = 0;
        ADMUX = 0;
        ADCSRA |= 0b01000000;
        while (ADCSRA & 0b01000000);
        if (ADC < 204) {
            x = 0; // fire forward
            xTaskCreate(DCPump, "Pump", 128, NULL, tskIDLE_PRIORITY + 3, NULL); // highest priority
        } else {
            ADMUX = 0b00000001;
            ADCSRA |= 0b01000000;
            while (ADCSRA & 0b01000000);
            if (ADC < 204) { // fire on left
                x = 1;
                left();
                _delay_ms(500);
            } else {
                ADMUX = 0b00000010;
                ADCSRA |= 0b01000000;
                while (ADCSRA & 0b01000000);
                if (ADC < 204) { // fire left x2
                    x = 2;
                    left();
                    _delay_ms(500);
                } else {
                    ADMUX = 0b00000011;
                    ADCSRA |= 0b01000000;
                    while (ADCSRA & 0b01000000);
                    if (ADC < 204) { // fire in the back
                        x = 3;
                        spin();
                        _delay_ms(700);
                    } else {
                        ADMUX = 0b00000100;
                        ADCSRA |= 0b01000000;
                        while (ADCSRA & 0b01000000);
                        if (ADC < 204) { // fire on right
                            x = 4;
                            right();
                            _delay_ms(500);
                        } else {
                            ADMUX = 0b00000101;
                            ADCSRA |= 0b01000000;
                            while (ADCSRA & 0b01000000);
                            if (ADC < 204) { // fire on right x2
                                x = 5;
                                right();
                                _delay_ms(500);
                            }
                        }
                    }
                }
            }
        }
        vTaskDelay(70);
    }
}

static void ObjectAvoidance(void* pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    DDRD |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6);
    TCCR0A = 0b10100001;
    TCCR0B = 0b00000010;
    OCR0A = 255;
    OCR0B = 255;
    forward();

    while (1) {
        init_Timer_for_Servo();
        OCR1A = 316; // ~90 deg center
        _delay_ms(50);
        int FrontDistance = scan();
        if (FrontDistance == (0xfffe)) { // no obstacle in front
            forward();
        } else { // obstacle detected ---> navigate
            push_break();
            navigate();
        }
        vTaskDelay(100);
    }
}

static void DCPump(void* pvParameters) {
    push_break();
    backward();
    _delay_ms(250);
    push_break();
    int x = 0;
    DDRB = 0b00111000;
    TCCR2A = 0b10000001;
    TCCR2B = 0b00000010;
    DDRD = 0b01101111;
    TCCR0A = 0b10100001;
    TCCR0B = 0b00000010;
    PORTB = 0b00010000;
    DDRC = 0b00000000;
    ADCSRA |= 0b00000111;
    ADCSRA |= 0b10000000;
    DIDR0 = 0xff;

    while (1) {
        // to move the pump left and right to better target the fire
        if (x % 2 == 0) {
            left();
            _delay_ms(500);
            x = x + 1;
        } else {
            right();
            _delay_ms(500);
            x = x + 1;
        }

        // read the adc to see if the fire is getting weaker
        ADMUX = 0;
        ADCSRA |= 0b01000000;
        while (ADCSRA & 0b01000000);

        // PWM signal depends on the fire intensity
        OCR2A = (ADC - 400) / 4; // 400 is the maximum result for the flame sensor in case no fire is detected
        OCR0A = 200;
        OCR0B = 200;
    }
}

int main(void) {
    xTaskCreate(ObjectAvoidance, "Avoid", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vADCRead, "ADC", 128, NULL, tskIDLE_PRIORITY + 2, &xHandle);
    vTaskStartScheduler();
}