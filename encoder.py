from machine import Pin
import utime

class Encoder:
    def __init__(self, gpio_a, gpio_b, gpio_z, gpio_i):
        self.gpio_a = gpio_a
        self.gpio_b = gpio_b
        self.gpio_z = gpio_z
        self.gpio_i = gpio_i
        self.position = 0
        self.prev_position = 0
        self.prev_time = utime.ticks_us()
        self.encoder_pulses = 64
        self.timer_ticks_us = 125
        self.timer_interval_us = 10000
        self.init_gpio()
        self.init_interrupts()
    
    def init_gpio(self):
        self.pin_a = Pin(self.gpio_a, Pin.IN, Pin.PULL_UP)
        self.pin_b = Pin(self.gpio_b, Pin.IN, Pin.PULL_UP)
        self.pin_z = Pin(self.gpio_z, Pin.IN, Pin.PULL_UP)
        self.pin_i = Pin(self.gpio_i, Pin.IN, Pin.PULL_UP)
    
    def init_interrupts(self):
        self.pin_a.irq(handler=self.encoder_interrupt_A, trigger=(Pin.IRQ_RISING | Pin.IRQ_FALLING), priority=1)
        self.pin_b.irq(handler=self.encoder_interrupt_B, trigger=(Pin.IRQ_RISING | Pin.IRQ_FALLING), priority=1)
        self.pin_z.irq(handler=self.encoder_interrupt_Z, trigger=Pin.IRQ_RISING, priority=1)
        self.pin_i.irq(handler=self.encoder_interrupt_I, trigger=Pin.IRQ_RISING, priority=1)

    def get_rpm(self):
        current_time = utime.ticks_us()
        elapsed_time_us = current_time - self.prev_time
        position_change = self.position - self.prev_position
        rpm = (position_change * self.timer_ticks_us * self.encoder_pulses / elapsed_time_us) * 60.0
        self.prev_position = self.position
        self.prev_time = current_time
        return rpm
    
    def get_velocity(self):
        current_time = utime.ticks_us()
        elapsed_time_us = current_time - self.prev_time
        position_change = self.position - self.prev_position
        velocity = (position_change * self.timer_ticks_us * self.encoder_pulses / elapsed_time_us) * (self.timer_ticks_us / 1000000.0)
        self.prev_position = self.position
        self.prev_time = current_time
        return velocity
    
    def get_acceleration(self):
        current_time = utime.ticks_us()
        elapsed_time_us = current_time - self.prev_time
        position_change = self.position - self.prev_position
        acceleration = (position_change * self.timer_ticks_us * self.encoder_pulses / elapsed_time_us) * (self.timer_ticks_us / 1000000.0) / (elapsed_time_us / 1000000.0)
        self.prev_position = self.position
        self.prev_time = current_time
        return acceleration
    
    def get_direction(self):
        position_change = self.position - self.prev_position
        direction = '+' if position_change > 0 else '-'
        self.prev_position = self.position
        return direction
    
    def reset_position(self):
        self.position = 0
        self.prev_position = 0
