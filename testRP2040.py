from machine import Pin
import utime
from encoder import Encoder

encoder = Encoder(2, 3, 4, 5)  # Initialize the encoder with GPIO pins A = 2, B = 3, Z = 4, and I = 5

while True:
    rpm = encoder.get_rpm()
    velocity = encoder.get_velocity()
    acceleration = encoder.get_acceleration()
    direction = encoder.get_direction()
    print("RPM: {:.2f}, Velocity: {:.2f} {} / s, Acceleration: {:.2f} {} / s^2, Direction: {}".format(rpm, velocity, direction, acceleration, direction, direction))
    utime.sleep_ms(1000)
