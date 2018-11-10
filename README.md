# CAT_Laser
This project has not yet been tested!!! Still waiting for my hardware to arrive. Also 3d parts will only be created when my hardware arrived and I can take measures.
## Idea
Mounting a laser diode movable in 2 degrees using servos to let a cat play.
## Components
- NodeMCU or similar ESP
- 2x Servo SG90
- Laser diode: AZDelivery 15x6mm 5V Copper Head
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

## Code
(Please rename the config.cpp.template to config.cpp and set your coresponding credentials and parameters.)
At the top of the code there are some setting to be adapted to your needs:
- speed of the laser point (how fast is your cat?)
- pause range between direction changes
- position above floor of the laser pointer
- borders of the "field of play" (distances are all measured from the dead center of the laser position)
- delay between cycles (can be used to smooth the movement)

## Images
