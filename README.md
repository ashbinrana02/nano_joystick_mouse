# Arduino Nano Joystick Mouse Controller

This project uses an **Arduino Nano + a joystick module** to control mouse movement on a PC using Python (`pyautogui`) and serial communication.

The joystick sends X/Y values over serial, and a Python script reads the data and moves the mouse accordingly.  
A button on the joystick is used for left-click.

---

# Hardware Used
- Arduino Nano (ATmega328P)
- Joystick module (with X, Y and SW pin)
- USB cable
- Few jumper wires
- Computer running Python


| Joystick Pin | Arduino Nano Pin |
|--------------|------------------|
| VRx          | A0               |
| VRy          | A1               |
| SW           | A2               |
| +5V          | 5V               |
| GND          | GND              |

---

# Software Used
- [Arduino IDE](https://www.arduino.cc/en/software/)
- [pyautogui Library](https://pyautogui.readthedocs.io/en/latest/index.html)
- ([pyserial library](https://pyserial.readthedocs.io/en/latest/index.html))


# To use
- Make sure the com port defined in the python script is the same com port that the arduino appers as on the computer
