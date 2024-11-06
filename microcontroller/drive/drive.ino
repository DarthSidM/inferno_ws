uint8_t leftMotor[] = {26, 25};  // {dir1, pwm1} //md2
uint8_t rightMotor[] = {33, 32}; // {dir2, pwm2} //md2

// Function to initialize motor pins
void setMotor(uint8_t motor[]) {
    pinMode(motor[0], OUTPUT); // Direction pin
    pinMode(motor[1], OUTPUT); // PWM pin
}

//Setup
void setup() {
    setMotor(leftMotor);
    setMotor(rightMotor);
    Serial.begin(115200);
}

// Function to control motors
void motorCtrl(uint8_t motor[], uint8_t dir, uint8_t pwm) {
    digitalWrite(motor[0], dir); // Set direction
    analogWrite(motor[1], pwm);  // Set speed (PWM)
}

//Main Loop
void loop() {
    if (Serial.available() > 0) {
        uint8_t command = (uint8_t)Serial.read(); // First byte is the command
        uint8_t speed = (uint8_t)Serial.read(); // Read the speed as an integer
        
        Serial.print(command);
        Serial.println(speed);
        // Control logic
        switch(command) {
            // Forward
            case 1:
                motorCtrl(leftMotor, 0, speed);
                motorCtrl(rightMotor, 0, speed);
                break;
            
            // Backward
            case 2:
                motorCtrl(leftMotor, 1, speed);
                motorCtrl(rightMotor, 1, speed);
                break;

            // Left turn
            case 3:
                motorCtrl(leftMotor, 0, speed);
                motorCtrl(rightMotor, 1, speed);
                break;

            // Right turn
            case 4:
                motorCtrl(leftMotor, 1, speed);
                motorCtrl(rightMotor, 0, speed);
                break;
            
            // Stop
            case 5:
                motorCtrl(leftMotor, 0, 0);
                motorCtrl(rightMotor, 0, 0);
                break;
        }
    }
}
