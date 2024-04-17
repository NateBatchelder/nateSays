#simon says game
#Made by Nate Batchelder
#Forked-Lightning (c)2024

import random
import time
import os
import GPIO


#define GPIO pins
redLED = 0
blueLED = 1
greenLED = 2
yellowLED = 3
redButton = 4
blueButton = 5
greenButton = 6
yellowButton = 7

#buzzer
buzzer = 8

#start/stop button
startButton = 9


#GPIO Pinmode for LEDs
def setup():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(redLED, GPIO.OUT)
    GPIO.setup(blueLED, GPIO.OUT)
    GPIO.setup(greenLED, GPIO.OUT)
    GPIO.setup(yellowLED, GPIO.OUT)
    GPIO.setup(buzzer, GPIO.OUT)
    GPIO.setup(startButton, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(redButton, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(blueButton, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(greenButton, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(yellowButton, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)





#game logic
def simonSays():
    #variables
    simon = []
    player = []
    score = 0
    #game loop
    while True:
        #add random number to simon
        simon.append(random.randint(0,3))
        #print simon
        for i in simon:
            if i == 0:
                print("Red")
            elif i == 1:
                print("Blue")
            elif i == 2:
                print("Green")
            elif i == 3:
                print("Yellow")
            time.sleep(1)
            os.system('cls' if os.name == 'nt' else 'clear')
        #get player input
        for i in simon:
            if i == 0:
                player.append("Red")
            elif i == 1:
                player.append("Blue")
            elif i == 2:
                player.append("Green")
            elif i == 3:
                player.append("Yellow")
        #check player input
        if player == simon:
            score += 1
            print("Correct!")
        else:
            print("Incorrect! Your score was: " + str(score))
            break
        player = []