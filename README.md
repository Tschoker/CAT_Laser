# CAT_Laser

## Idea
Mounting a laser diode movable in 2 degrees using servos to let a cat play. In the current version it only reacts an a button installed. In a future version it is intended to integrate it into a home automation hub using MQTT.

## Components
- NodeMCU or similar ESP
- 2x Servo SG90
- Laser diode: 15x6mm 5V Copper Head
- Button
- 3d printed parts
- power supply (USB 5V)

## Schematics
The Laser Diode is connected:
red: ESP D1
blue: ESP Gnd
The servo for x-axis is connected:
red: ESP 5V
brown: ESP Gnd
yellow: ESP D2
The servo for y-axis is connected:
red: ESP 5V
brown: ESP Gnd
yellow: ESP D3
The button is connected between ESP D4 and GND (the ESP you use needs to have internal pullup on that pin)

## Code
(Please rename the config.cpp.template to config.cpp and set your coresponding credentials and parameters.)
At the top of the code there are some setting to be adapted to your needs:
- speed of the laser point (how fast is your cat?)
- pause range between direction changes
- position above floor of the laser pointer
- borders of the "field of play" (distances are all measured from the dead center of the laser position)
- delay between cycles (can be used to smooth the movement)

## Images
![Image1](/images/image1.jpg)

![Image2](/images/image1.jpg)

![Video1](/images/video1.jpg)
