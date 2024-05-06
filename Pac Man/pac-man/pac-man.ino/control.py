import serial
import time
import pyautogui

# Return time in ms
def millis():
    return int(round(time.time() * 1000))

# Arduino serial port interface
ser = serial.Serial('/dev/ttyACM1', 115200, timeout=1)

# Timing variable
timer = millis()

latency = 24

# Movement state variables
left_pressed = False
right_pressed = False
up_pressed = False
down_pressed = False

# Infinite loop
while True:
    try:
        # Process serial data
        data = ser.readline().decode('utf-8').strip()
        
        # Debounce
        if (millis() - timer) > latency:
            timer = millis()

            # Release all movement keys
            pyautogui.keyUp('left')
            pyautogui.keyUp('right')
            pyautogui.keyUp('up')
            pyautogui.keyUp('down')

            # Control Pac-Man's movement
            if int(data) == 1:
                left_pressed = True
            elif int(data) == 2:
                right_pressed = True
            elif int(data) == 3:
                up_pressed = True
            elif int(data) == 4:
                down_pressed = True

            # Press the corresponding keys based on the movement state
            if left_pressed:
                pyautogui.keyDown('left')
            if right_pressed:
                pyautogui.keyDown('right')
            if up_pressed:
                pyautogui.keyDown('up')
            if down_pressed:
                pyautogui.keyDown('down')

        ser.flushInput()

    except Exception as e:
        print(e, "\nBlink now...")
        continue

