#include <Arduino.h>
#include <math.h>
#include <Servo.h>

//Pin definitions
#define servo_x D1
#define servo_y D2
#define laser D3
#define button D4

/* This is how the coordinate system looks from the top
                   ^
                   |
                   + y_back
                   |
                   |
                   + y_front
                   |
---+---------------+----------------------+
x_left        laser position          x_right
*/

//variables for configuration ############################################################################
int speed = 500; //speed in mm/sec
int duration = 120; //duration of game in seconds

int x_left = 800; //left border of gamefield in mm
int x_right = 800; //right border of gamefield in mm
int y_front = 500; //front border of gamefield in mm
int y_back = 1200; //back border of gamefield in mm
int height = 1850; //placement of laser in mm

int wait_min = 200; //wait time when reached target
int wait_max = 500; //wait time when reached target

int delay_time = 10; //delay per cycle in ms

//do not touch anything below unless you know what you are doing #########################################
int x_pos = 0; //current position
int y_pos = 0; //current position
int x_step = 0; //next step target
int y_step = 0; //next step traget
int x_tar = 0; //target position
int y_tar = 0; //target position
int x_serv = 0; //servo setting for x
int y_serv = 0; //servo setting for y


int dist = speed * delay_time / 1000; //distance to travel per cycle

bool game_is_running = 0; //game is currently running y/n
float game_started = 0; //start time of last game

const float pi = 3.14159;

Servo x_servo;
Servo y_servo;


//general functions ######################################################################################
int cDistance(){
  //calculates the distance of the current position to the target
  int dx = x_tar - x_pos;
  int dy = y_tar - y_pos;
  return sqrt(sq(dx)+sq(dy));
}

void cStepTarget(){
  //calculates target for next step
  if(cDistance() >= dist){
    float alpha = asin((abs(y_tar-y_pos))/cDistance());
    int dy = dist * sin(alpha);
    int dx = dist * cos(alpha);
    //set target points (check if target is up/down & left/right)
    if (x_tar >= x_pos){
      x_step = x_pos + dx;
    }
    else {
      x_step = x_pos - dx;
    }
    if (y_tar >= y_pos){
      y_step = y_pos + dy;
    }
    else {
      y_step = y_pos - dy;
    }
  } else {
    x_step = x_tar;
    y_step = y_tar;
  }
}

long fMap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void moveServo(){
  //moves the servos to point to the given coordinates
  float alpha_x = 0;
  if(y_step == 0){
    alpha_x = 0;
  } else {
    alpha_x = atan((float)x_step/(float)y_step); // -pi/2 < alpha_x < +pi/2 (could be left or right)
  }
  int delta = sqrt(sq(x_step)+sq(y_step)); //distance from the root of the device on the floor (x=0 y=0) to step point
  float alpha_y = atan((float)delta/(float)height); // 0 < alpha_y < +pi/2 (0 points perpendicular to the floor and +pi/2 against opposite wall)
  /*
  Serial.print("alpha_x: ");
  Serial.print(alpha_x);
  Serial.print(" alpha_y: ");
  Serial.print(alpha_y);
  Serial.print(" x degree: ");
  Serial.print(90+(alpha_x * 57.2957795));
  Serial.print(" y degree: ");
  Serial.println(alpha_y * 57.2957795);
  */
  float deg_x = 90-(alpha_x * 57.2957795);
  float deg_y = alpha_y * 57.2957795;

  //set alpha_x/y to the servos.....
  x_servo.write(deg_x);
  y_servo.write(deg_y);
  x_pos = x_step;
  y_pos = y_step;
}

void setLaser(int power){
  //power in % (0-100)
  if (power >= 100){
    power = 100;
  }
  if (power <= 0){
    power = 0;
  }
  Serial.print("Setting laser to: ");
  Serial.println(power);
  analogWrite(laser, power*255/100);
}

void demo(){
  Serial.println("Starting Demo.....");
  Serial.println("Moving to start");
  //set front left point as startpoint
  x_step = -x_left;
  y_step = y_front;
  moveServo();
  delay(500);
  setLaser(100);
  Serial.println("Move left to right along front border...");
  do {
    x_step = x_step + dist;
    moveServo();
    delay(delay_time);
  } while (x_step <= x_right);
  x_step = x_right;
  Serial.println("Move front to back along right border...");
  do {
    y_step = y_step + dist;
    moveServo();
    delay(delay_time);
  } while (y_step <= y_back);
  y_step = y_back;
  Serial.println("Move right to left along back border...");
  do {
    x_step = x_step - dist;
    moveServo();
    delay(delay_time);
  } while (x_step >= -x_left);
  x_step = -x_left;
  Serial.println("Move back to front along left border...");
  do {
    y_step = y_step - dist;
    moveServo();
    delay(delay_time);
  } while (y_step >= y_front);
  Serial.println("Demo completed...");
  delay(500);
}
void StartGame(){
  //when game starts: initiate start position in the middle of the field and activate laser
  Serial.println("Starting new Game!");
  game_is_running = 1;
  x_step = (x_right - x_left)/2;
  y_step = (y_back - y_front)/2 + y_front;

  setLaser(100);
  Serial.println("Randomizing new target.");
  x_tar = random(-x_left, x_right);
  y_tar = random(y_front, y_back);
  Serial.print("New Target: ");
  Serial.print(x_tar);
  Serial.print("/");
  Serial.println(y_tar);
  game_started = millis();

  /*
  moveServo();
  Serial.println("4");
  //delay(500); //wait for laser to be in right position befor activating laser
  Serial.println("Setaser");
  setLaser(100);
  Serial.println("Randomizing new target.");
  delay(random(wait_min, wait_max));
  //set new target
  x_tar = random(-x_left, x_right);
  y_tar = random(y_front, y_back);
  Serial.print("New Target: ");
  Serial.print(x_tar);
  Serial.print("/");
  Serial.println(y_tar);
  game_started = millis();
  */
}

void setup(){
  Serial.begin(115200);
  randomSeed(analogRead(0));
  pinMode(laser,OUTPUT);
  x_servo.attach(servo_x);
  y_servo.attach(servo_y);
  pinMode(button,INPUT);
  attachInterrupt(digitalPinToInterrupt(button), StartGame, FALLING);

  //start demo
  demo();

  setLaser(0);
  x_step = 0;
  y_step = 0;
  moveServo();
}

void loop(){
  //game limit reached?
  if ((game_started + duration * 1000 <= millis()) && game_is_running){
    Serial.println("Game limit reached - deactivating laser and go to parking position");
    game_started = 0;
    game_is_running = 0;
    setLaser(0);
    x_step = 0;
    y_step = 0;
    moveServo();
  }

  if (game_is_running){
    //reached last target?
    if(x_pos==x_tar && y_pos==y_tar){
      //wait for some time
      Serial.print("Randomizing new target.");
      delay(random(wait_min, wait_max));
      //set new target
      x_tar = random(-x_left, x_right);
      y_tar = random(y_front, y_back);
      Serial.print(" New Target: ");
      Serial.print(x_tar);
      Serial.print("/");
      Serial.println(y_tar);
    }
    else {
      //calculate new step
      Serial.println("Moving to next step.");
      cStepTarget();
      //move servos
      moveServo();
    }
    delay(delay_time); //allow the MC to pause for a while or watchdog could kick in
  }
  else {
    //check MQTT for start signal
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

    delay(5000); //pause a while to prevent MQTT server from beeing queried unnecessary often
  }
}
