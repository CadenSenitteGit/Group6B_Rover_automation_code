import keyboard
import requests
import time

##Keyboard controls to control Pololu Qik motors and Servos

print("go!")

while True:
    try:
        if keyboard.is_pressed("a"):
            requests.get("http://192.168.4.1/l") ##left

        elif keyboard.is_pressed("d"):
            requests.get("http://192.168.4.1/r") ##right

        elif keyboard.is_pressed("s"):
            requests.get("http://192.168.4.1/b") ##backward

        elif keyboard.is_pressed("w"):
            requests.get("http://192.168.4.1/g") ##forward

        elif keyboard.is_pressed("l"):
            requests.get("http://192.168.4.1/u") ##arm up

        elif keyboard.is_pressed("j"):
            requests.get("http://192.168.4.1/d") ##arm down

        elif keyboard.is_pressed("i"):
            requests.get("http://192.168.4.1/o") ##scoop up

        elif keyboard.is_pressed("k"):
            requests.get("http://192.168.4.1/k") ##scoop down
            
        else:
            requests.get("http://192.168.4.1/s") ##stop
            
    except Exception as e:
        print("Not connected to car's wifi!")
        time.sleep(2)  # Sleep for 2 seconds
