#define forward_back_input 0
#define steering_input 1
#define read_switch 2

#define left_motor_pin 26
#define right_motor_pin 27

#include <Servo.h>

Servo left_motor;
Servo right_motor;

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

/*
void motor_control(int forward_back_val, int steering_val){
  left_speed = forward_back_val + steering_val;
  right_speed = forward_back_val - steering_val;
  //left_speed = constrain(left_speed, -100, 100);
  //right_speed = constrain(right_speed, -100, 100);
  //left_motor_control(map(left_speed, -100, 100, 1000, 2000));
  //right_motor_control(map(right_speed, -100, 100, 1000, 2000));
}
*/
//Both of the motor control fucntions will write the values into microeconds
//so we can control both of the motors through the different wires.
void left_motor_control(int val) {
  left_motor.writeMicroseconds(val);
}

void right_motor_control(int val){
  right_motor.writeMicroseconds(val);
}

void setup(){
  Serial.begin(115200);
  //forward and backward movement input and outputs
  pinMode(forward_back_input, INPUT);
  //stearing left and right input and outputs
  pinMode(steering_input, INPUT);
  //reading switch input to read the inputs and outputs
  pinMode(read_switch, INPUT);
  //left_motors and right_motors will be attatched. 1000 and 2000 values 
  //represents the pwm ranges, which the 1000 represents the minimum value
  //and the 2000 represents the maximum value
  left_motor.attach(left_motor_pin, 1000, 2000);
  right_motor.attach(right_motor_pin, 1000, 2000);
}

int forward_back_value, steering_value;
int left_speed, right_speed;
bool read_switch_value;

void loop() {
  //these 3 values will read from the channel and the switch, based on their
  //default values. -100 and 100 values represents a range of possible inputs
  //in resprectable axis.
  forward_back_value = readChannel(forward_back_input, -100, 100, 0);
  steering_value = readChannel(steering_input, -100, 100, 0);
  read_switch_value = readSwitch(read_switch_value, 0);

  //the left_motor and right_motor controls will have values that are 
  //converted from the map functions, where it converts values from -100 and
  //100, to values from 1000 to 2000

  left_speed = forward_back_value + steering_value;
  right_speed = forward_back_value - steering_value;
  left_speed = constrain(left_speed, -100, 100);
  right_speed = constrain(right_speed, -100, 100);

  left_motor_control(map(left_speed, -100, 100, 1000, 2000));
  right_motor_control(map(right_speed, -100, 100, 1000, 2000));
  
  //printing out the values of forward/back, stearing, and read switch, and
  //they change, depending on the input of the rc control.
  Serial.print("Forward/Back: ");
  Serial.print(forward_back_value);
  Serial.print(" Steering: ");
  Serial.print(steering_value);
  Serial.print(" Read switch: ");
  Serial.print(read_switch);
  Serial.print(" Left motor: ");
  Serial.print(left_speed);
  Serial.print(" Right motor: ");
  Serial.print(right_speed);
  Serial.println();
  //Serial.println();

  //motor_control(forward_back_value, steering_value);
}
