#!/usr/bin/env python3
from math import sin, cos, pow, fabs, radians, degrees
import socket, re

def almostEqual(a, b, float64EqualityThreshold = 1) -> bool:
	return fabs(a-b) <= float64EqualityThreshold


s = socket.socket()
s.connect(('localhost', 8181))

for i in range(100):
    data = s.recv(4096).decode()
    print(data)
    enemy_x, enemy_y, enemy_angle, enemy_velocity, enemy_acceleration = [float(e) for e in re.findall(r"σημείο: ([\d\.]*) ([\d\.]*).*\n.*κατεύθυνση: ([\d\.]*)\n.*γρήγορα: ([\d\.]*) m\/s\n.*επιταχύνεται: ([\d\.]*)", data, re.UNICODE)[0]]
    print("input:", enemy_x, enemy_y, enemy_angle, enemy_velocity, enemy_acceleration)
    print(f"ship #{i}")
    for alpha in (x * 0.001 for x in range(0, 3141)):
        for theta in (x * 0.001 for x in range(0, 6282)):
            leftX = (60 * 60 * sin(2*alpha) * cos(theta)) / 9.81
            rightX = (enemy_velocity*((2 * 60 * sin(alpha)) / 9.81)+enemy_acceleration*pow(((2 * 60 * sin(alpha)) / 9.81), 2)/2)*cos(radians(enemy_angle)) + enemy_x
            leftY = (60 * 60 * sin(2*alpha) * sin(theta)) / 9.81
            rightY = (enemy_velocity*((2 * 60 * sin(alpha)) / 9.81)+enemy_acceleration*pow(((2 * 60 * sin(alpha)) / 9.81), 2)/2)*sin(radians(enemy_angle)) + enemy_y
            # resultX = ((60 * 60 * sin(radians(2*alpha)) * cos(theta)) / 9.81) - ((enemy_velocity * ((2 * 60 * sin(alpha)) / 9.81) + enemy_acceleration*pow((2 * 60 * sin(alpha)) / 9.81, 2)/2)*cos(radians(enemy_angle)) + enemy_x)
            # resultY = ((60 * 60 * sin(radians(2*alpha)) * sin(theta)) / 9.81) - ((enemy_velocity * ((2 * 60 * sin(alpha)) / 9.81) + enemy_acceleration*pow((2 * 60 * sin(alpha)) / 9.81, 2)/2)*sin(radians(enemy_angle)) + enemy_y)
            if almostEqual(leftX, rightX) and almostEqual(leftY, rightY):
                print(leftX - rightX, leftY - rightY)
                print("solution:", degrees(alpha), degrees(theta))
                print("solution:", alpha, theta)
                s.send((str(degrees(alpha)) + " " + str(degrees(theta)) + "\n").encode())
                # data = s.recv(4096).decode()
                # print(data)
                break
        else:
            continue
        break
    else:
        print("no solution found!")
        break
data = s.recv(4096).decode()
print(data)
# ((60 * 60 * sin(radians(2*alpha)) * cos(theta)) / 9.81) - ((v * ((2 * 60 * sin(alpha)) / 9.81) + a*pow((2 * 60 * sin(alpha)) / 9.81, 2)/2)*cos(radians(c)) + x) = 0
# ((60 * 60 * sin(radians(2*alpha)) * sin(theta)) / 9.81) - ((v * ((2 * 60 * sin(alpha)) / 9.81) + a*pow((2 * 60 * sin(alpha)) / 9.81, 2)/2)*sin(radians(c)) + y) = 0
