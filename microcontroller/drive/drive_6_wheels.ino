uint8_t leftFront[] = {35,34}; //md1 {dir1,pwm1};
uint8_t leftBack[] = {22,23};

uint8_t rightFront[] = {13,12};//md3 {dir1,pwm1};
uint8_t rightBack[] = {14,27};

uint8_t leftMiddle[] = {26,25};//md2 {dir1,pwm1};
uint8_t rightMiddle[] = {33,32};



void setMotor(uint8_t motor[]) {
    pinMode(motor[0], OUTPUT); // Direction pin
    pinMode(motor[1], OUTPUT); // PWM pin
}
void motorCtrl(uint8_t motor[], uint8_t dir, uint8_t pwm) {
    digitalWrite(motor[0], dir); // Set direction
    analogWrite(motor[1], pwm);  // Set speed (PWM)
}


void setup() {
  // put your setup code here, to run once:
  setMotor(leftFront);
  setMotor(leftBack);
  setMotor(leftMiddle);
  setMotor(rightFront);
  setMotor(rightBack);
  setMotor(rightMiddle);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
        uint8_t command = (uint8_t)Serial.read(); // First byte is the command
        uint8_t speed = (uint8_t)Serial.read(); // Read the speed as an integer
        
        Serial.print(command);
        Serial.println(speed);
        // Control logic
        switch(command) {
            // Forward
            case 1:
                motorCtrl(leftFront, 0, speed);
                motorCtrl(leftBack, 0, speed);
                motorCtrl(leftMiddle, 0, speed);

                motorCtrl(rightFront, 0, speed);
                motorCtrl(rightBack, 0, speed);
                motorCtrl(rightMiddle, 0, speed);
                break;
            
            // Backward
            case 2:
                motorCtrl(leftFront, 0, speed);
                motorCtrl(leftBack, 0, speed);
                motorCtrl(leftMiddle, 0, speed);
                
                motorCtrl(rightFront, 0, speed);
                motorCtrl(rightBack, 0, speed);
                motorCtrl(rightMiddle, 0, speed);
                break;

            // Left turn
            case 3:
                motorCtrl(leftFront, 0, speed);
                motorCtrl(leftBack, 0, speed*0.5);
                motorCtrl(leftMiddle, 0, speed);
                
                motorCtrl(rightFront, 1, speed);
                motorCtrl(rightBack, 1, speed*0.5);
                motorCtrl(rightMiddle, 1, speed);
                break;

            // Right turn
            case 4:
                motorCtrl(leftFront, 1, speed);
                motorCtrl(leftBack, 1, speed*0.5);
                motorCtrl(leftMiddle, 1, speed);
                
                motorCtrl(rightFront, 0, speed);
                motorCtrl(rightBack, 0, speed*0.5);
                motorCtrl(rightMiddle, 0, speed);
                break;
            
            // Stop
            case 5:
                motorCtrl(leftFront, 0, 0);
                motorCtrl(leftBack, 0, 0);
                motorCtrl(leftMiddle, 0, 0);
                
                motorCtrl(rightFront, 0, 0);
                motorCtrl(rightBack, 0, 0);
                motorCtrl(rightMiddle, 0, 0);
                break;
        }
    }
}
