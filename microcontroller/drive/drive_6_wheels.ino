// Use a float between 0 to 1
#define frontRatio 1
#define middleRatio 0.3
#define backRatio 1

uint8_t leftFront[] = {19,21}; //md1 {dir1,pwm1};
uint8_t leftBack[] = {22,23};

uint8_t leftMiddle[] = {33,32}; //md2 {dir1,pwm1};
uint8_t rightMiddle[] = {26,25};

uint8_t rightFront[] = {13,12}; //md3 {dir1,pwm1};
uint8_t rightBack[] = {14,27};


void setMotor(uint8_t motor[]) {
    pinMode(motor[0], OUTPUT); // Direction pin
    pinMode(motor[1], OUTPUT); // PWM pin
}
void motorCtrl(uint8_t motor[], uint8_t dir, uint8_t pwm) {
    digitalWrite(motor[0], dir); // Set direction
    analogWrite(motor[1], pwm);  // Set speed (PWM)
}

void linear(int dir, int pwm){
    motorCtrl(leftFront, dir, pwm);
    motorCtrl(leftBack, dir, pwm);
    motorCtrl(leftMiddle, dir, pwm*(6.0/7));

    motorCtrl(rightFront, dir, pwm);
    motorCtrl(rightBack, dir, pwm);
    motorCtrl(rightMiddle, dir, pwm*(6.0/7));
}

void turn(int dir, int pwm){
    motorCtrl(leftFront, !dir, pwm*frontRatio);
    motorCtrl(leftBack, !dir, pwm*backRatio);
    motorCtrl(leftMiddle, !dir, pwm*middleRatio);
    
    motorCtrl(rightFront, dir, pwm*frontRatio);
    motorCtrl(rightBack, dir, pwm*backRatio);
    motorCtrl(rightMiddle, dir, pwm*middleRatio);
}

void ctrl(void *pvParameters) {
    while (true) {
        if (Serial.available() > 0) {
            uint8_t command = (uint8_t)Serial.read(); // First byte is the command
            uint8_t speed = (uint8_t)Serial.read(); // Read the speed as an integer
            
            Serial.print(command);
            Serial.print(" ");
            Serial.println(speed);
            // Control logic
            switch(command) {
                // Forward
                case 1:
                    linear(0, speed);
                    break;
                // Backward
                case 2:
                    linear(1, speed);
                    break;
                // Left turn
                case 3:
                    turn(0, speed);
                    break;
                // Right turn
                case 4:
                    turn(1, speed);
                    break;
                // Stop
                case 5:
                    linear(0, 0);
                    break;
            }
        }
    }
}


void setup() {
    setMotor(leftFront);
    setMotor(leftBack);
    setMotor(leftMiddle);
    setMotor(rightFront);
    setMotor(rightBack);
    setMotor(rightMiddle);
    Serial.begin(115200);

    xTaskCreatePinnedToCore(ctrl, "Control", 10000, NULL, 1, NULL, 0);
}

void loop() {
    // Main loop does nothing, tasks are running on cores
    Serial.println("drive");
    delay(1000);
}
