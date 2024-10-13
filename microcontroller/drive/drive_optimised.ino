uint8_t receivedData[4];
uint8_t leftMotor[] = {4, 5};  // {dir, pwm}
uint8_t rightMotor[] = {7, 6}; // {dir, pwm}

// Function to initialize motor pins
void setMotor(uint8_t motor[]) {
    pinMode(motor[0], OUTPUT); // Direction pin
    pinMode(motor[1], OUTPUT); // PWM pin
}

// Function to control motors
void motorCtrl(uint8_t motor[], uint8_t dir, uint8_t pwm) {
    digitalWrite(motor[0], dir); // Set direction
    analogWrite(motor[1], pwm);  // Set speed (PWM)
}

// Setup
void setup() {
    setMotor(leftMotor);
    setMotor(rightMotor);
    Serial.begin(115200);
}

// Main loop
void loop() {
    if (Serial.available() >= 4) {  // Check if at least 4 bytes are available
        // Read the 4 bytes into the array
        for (int i = 0; i < 4; i++) {
            receivedData[i] = (uint8_t)Serial.read();
        }

        // Extract the values from the array
        uint8_t leftDir = receivedData[0];  // a can be 0 or 1
        uint8_t leftPWM = receivedData[1];  // b can be 0-255
        uint8_t rightDir = receivedData[2]; // c can be 0 or 1
        uint8_t rightPWM = receivedData[3]; // d can be 0-255

        // Control the motors based on the received data
        motorCtrl(leftMotor, leftDir, leftPWM);
        motorCtrl(rightMotor, rightDir, rightPWM);

        // Debugging print statements to check the received data
        Serial.print("Left Motor: Dir = ");
        Serial.print(leftDir);
        Serial.print(", PWM = ");
        Serial.println(leftPWM);

        Serial.print("Right Motor: Dir = ");
        Serial.print(rightDir);
        Serial.print(", PWM = ");
        Serial.println(rightPWM);
    }
}
