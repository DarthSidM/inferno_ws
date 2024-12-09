#define dirPin 2
#define stepPin 4
#define stepsPerRevolution 200

#define delayTime 1000

int stepsArr[] = {67, 67, 66};
int iter=0;

void setup() {
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);

    Serial.begin(9600);
}

void moveStepper(int dir, int steps) {
    digitalWrite(dirPin, dir);
    // Move the motor step by step
    for (int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);  // Send pulse to STEP pin
        delayMicroseconds(delayTime); // Delay for step timing
        digitalWrite(stepPin, LOW);   // End pulse
        delayMicroseconds(delayTime); // Delay for step timing
    }
}

void loop() {
    if(Serial.available() > 0){
        char step = Serial.read();
        if(step == 'o'){
            moveStepper(0, stepsArr[iter]);
            
            if(iter == 2){iter = 0;}
            else{iter++;}
        }
    }
}
