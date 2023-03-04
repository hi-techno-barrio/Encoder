#include "encoder.h"

Encoder::Encoder(uint gpio_a, uint gpio_b, uint gpio_z, uint gpio_i) {
    m_gpio_a = gpio_a;
    m_gpio_b = gpio_b;
    m_gpio_z = gpio_z;
    m_gpio_i = gpio_i;
    gpio_init(m_gpio_a);
    gpio_set_dir(m_gpio_a, GPIO_IN);
    gpio_pull_up(m_gpio_a);
    gpio_init(m_gpio_b);
    gpio_set_dir(m_gpio_b, GPIO_IN);
    gpio_pull_up(m_gpio_b);
    gpio_init(m_gpio_z);
    gpio_set_dir(m_gpio_z, GPIO_IN);
    gpio_pull_up(m_gpio_z);
    gpio_init(m_gpio_i);
    gpio_set_dir(m_gpio_i, GPIO_IN);
    gpio_pull_up(m_gpio_i);
    gpio_set_irq_enabled_with_callback(m_gpio_a, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &Encoder::encoder_interrupt_A);
    gpio_set_irq_enabled_with_callback(m_gpio_b, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &Encoder::encoder_interrupt_B);
    gpio_set_irq_enabled_with_callback(m_gpio_z, GPIO_IRQ_EDGE_RISE, true, &Encoder::encoder_interrupt_Z);
    gpio_set_irq_enabled_with_callback(m_gpio_i, GPIO_IRQ_EDGE_RISE, true, &Encoder::encoder_interrupt_I);
    m_prev_time = timer_get_us();
}

double Encoder::get_rpm() {
    uint64_t current_time = timer_get_us();
    uint64_t elapsed_time_us = current_time - m_prev_time;
    int position_change = m_position - m_prev_position;
    double rpm = (double)position_change * (double)m_timer_ticks_us * (double)m_encoder_pulses / (double)elapsed_time_us * 60.0;
    m_prev_position = m_position;
    m_prev_time = current_time;
    return rpm;
}

double Encoder::get_velocity() {
    uint64_t current_time = timer_get_us();
    uint64_t elapsed_time_us = current_time - m_prev_time;
    int position_change = m_position - m_prev_position;
    double velocity = (double)position_change * (double)m_timer_ticks_us * (double)m_encoder_pulses / (double)elapsed_time_us * (double)m_timer_ticks_us / 1000000.0;
    m_position = m_position;
m_prev_time = current_time;
return velocity;
}

double Encoder::get_acceleration() {
uint64_t current_time = timer_get_us();
uint64_t elapsed_time_us = current_time - m_prev_time;
int position_change = m_position - m_prev_position;
double acceleration = (double)position_change * (double)m_timer_ticks_us * (double)m_encoder_pulses / (double)elapsed_time_us * (double)m_timer_ticks_us / 1000000.0 / (double)elapsed_time_us * 1000000.0;
m_prev_position = m_position;
m_prev_time = current_time;
return acceleration;
}

char Encoder::get_direction() {
int position_change = m_position - m_prev_position;
char direction = (position_change > 0) ? '+' : '-';
m_prev_position = m_position;
return direction;
}

void Encoder::reset_position() {
m_position = 0;
m_prev_position = 0;
}

void Encoder::encoder_interrupt_A() {
Encoder encoder = reinterpret_cast<Encoder>(gpio_get_irq_user_data(Encoder::m_gpio_a));
if (gpio_get(encoder->m_gpio_a) == gpio_get(encoder->m_gpio_b)) {
encoder->m_position++;
} else {
encoder->m_position--;
}
}

void Encoder::encoder_interrupt_B() {
Encoder encoder = reinterpret_cast<Encoder>(gpio_get_irq_user_data(Encoder::m_gpio_b));
if (gpio_get(encoder->m_gpio_a) == gpio_get(encoder->m_gpio_b)) {
encoder->m_position--;
} else {
encoder->m_position++;
}
}

void Encoder::encoder_interrupt_Z() {
Encoder encoder = reinterpret_cast<Encoder>(gpio_get_irq_user_data(Encoder::m_gpio_z));
encoder->reset_position();
}

void Encoder::encoder_interrupt_I() {
Encoder encoder = reinterpret_cast<Encoder>(gpio_get_irq_user_data(Encoder::m_gpio_i));
// Do something when interrupt pin I is triggered
printf("Interrupt pin I triggered!\n");
}

void Encoder::timer_interrupt() {
uint64_t current_time = timer_get_us();
double rpm = get_rpm();
double velocity = get_velocity();
double acceleration = get_acceleration();
char direction = get_direction();
printf("RPM: %.2f, Velocity: %.2f %c/s, Acceleration: %.2f %c/s^2, Direction: %c\n", rpm, velocity, direction, acceleration, direction, direction);
}


