import serial
import time
import pyautogui

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=0.01)
time.sleep(2)
arduino.reset_input_buffer()  # clear any junk at startup

buffer = ""
mouse_held = False   # tracks whether mouse is currently down

while True:
    # Read all available bytes from Arduino
    while arduino.in_waiting > 0:
        buffer += arduino.read().decode("ascii", errors="ignore")

    # Process only complete lines
    if "\n" in buffer:
        lines = buffer.split("\n")
        buffer = lines[-1]  # keep last (possibly incomplete) line in buffer
        latest_line = lines[-2].strip()  # take the most recent complete line

        if latest_line:  # make sure it's not empty
            try:
                xValue, yValue, buttonState = latest_line.split(",")
                xValue = int(xValue)
                yValue = int(yValue)
                buttonState = int(buttonState)

                # Debug print
                print(f"X:{xValue}, Y:{yValue}, Btn:{buttonState}")

                # Only move if joystick is actually moved
                if abs(xValue) > 1 or abs(yValue) > 1:
                    pyautogui.moveRel(xValue, -yValue)  # instant movement

                # Click if button pressed
                if buttonState == 1 and not mouse_held:
                    pyautogui.mouseDown()   # press and hold
                    mouse_held = True

                if buttonState == 0 and mouse_held:
                    pyautogui.mouseUp()     # release
                    mouse_held = False

            except ValueError:
                print("Bad data:", latest_line)
