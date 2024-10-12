#include <ros.h>
#include <sensor_msgs/Joy.h>

ros::NodeHandle nh;

uint8_t receivedData[4];
uint8_t leftMotor[] = {4, 5};  // {dir, pwm}
uint8_t rightMotor[] = {7, 6}; // {dir, pwm}


void setMotor(uint8_t motor[]) {
  pinMode(motor[0], OUTPUT); // Direction pin
  pinMode(motor[1], OUTPUT); // PWM pin
}
void motorCtrl(uint8_t motor[], uint8_t dir, uint8_t pwm) {
  digitalWrite(motor[0], dir); // Set direction
  analogWrite(motor[1], pwm);  // Set speed (PWM)
}



void joyCallback(const sensor_msgs::Joy& joy_msg) {
  // Process joystick data
  float xData = joy_msg.axes[3];
  float yData = joy_msg.axes[1];

  if(xData>0){
    motorCtrl(leftMotor, 1, (uint8_t)(xData*255));
    motorCtrl(rightMotor, 0, (uint8_t)(xData*255));
  }
  else if(xData<0){
    motorCtrl(leftMotor, 0, (uint8_t)(xData*255));
    motorCtrl(rightMotor, 1, (uint8_t)(xData*255));
  }
  else if(yData>0){
    motorCtrl(leftMotor, 1, (uint8_t)(yData*255));
    motorCtrl(rightMotor, 1, (uint8_t)(yData*255));
  }
  else if(yData<0){
    motorCtrl(leftMotor, 0, (uint8_t)(yData*255));
    motorCtrl(rightMotor, 0, (uint8_t)(yData*255));
  }
  else{
    motorCtrl(leftMotor, 0, 0);
    motorCtrl(rightMotor, 0, 0);
  }


  Serial.print("y axis: ");
  Serial.print(joy_msg.axes[1]);
  Serial.println();

  Serial.print("x axis: ");
  Serial.print(joy_msg.axes[3]);
  Serial.println();
}

ros::Subscriber<sensor_msgs::Joy> sub("/joy", joyCallback);

void setup() {
  Serial.begin(57600);
  nh.initNode();
  nh.subscribe(sub);
  setMotor(leftMotor);
  setMotor(rightMotor);
}

void loop() {
  nh.spinOnce();
}
