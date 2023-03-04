#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/timer.h"

class Encoder {
    public:
        Encoder(uint gpio_a, uint gpio_b, uint gpio_z, uint gpio_i);
        double get_rpm();
        double get_velocity();
        double get_acceleration();
        char get_direction();
        void reset_position();
    private:
        uint m_gpio_a;
        uint m_gpio_b;
        uint m_gpio_z;
        uint m_gpio_i;
        volatile int m_position;
        volatile int m_prev_position;
        volatile uint64_t m_prev_time;
        static constexpr uint32_t m_encoder_pulses = 64;
        static constexpr uint32_t m_timer_ticks_us = 125;
        static constexpr uint32_t m_timer_interval_us = 10000;
        static void encoder_interrupt_A();
        static void encoder_interrupt_B();
        static void encoder_interrupt_Z();
        static void encoder_interrupt_I();
        static void timer_interrupt();
};
