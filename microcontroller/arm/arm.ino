uint8_t actuator1[] = {19, 21}; //md1
uint8_t actuator2[] = {22, 23}; //md1
uint8_t pitch[] = {32, 33};//md2
uint8_t base[] = {25, 26};//md2
uint8_t roll[] = {5, 4};//md3
uint8_t grip[] = {2, 15};//md3


void setMotor(uint8_t motor[]){
    for(int i=0; i<2; i++){
        pinMode(motor[i], OUTPUT);
    }
}

void motorCtrl(uint8_t motor[], uint8_t dir, uint8_t pwm){
    digitalWrite(motor[0], dir);
    analogWrite(motor[1], pwm);
}


void setup() {
    setMotor(actuator1);
    setMotor(actuator2);
    setMotor(pitch);
    setMotor(base);
    setMotor(roll);
    setMotor(grip);

    Serial.begin(115200);
}

void loop() {
    if (Serial.available() > 0){
        uint8_t num = (uint8_t)Serial.read();
        uint8_t dir = (uint8_t)Serial.read();
        uint8_t pwm = (uint8_t)Serial.read();

        switch(num){  
            case 1:
                motorCtrl(actuator1, dir, pwm);
                break;
            case 2:
                motorCtrl(actuator2, dir, pwm);
                break;
            case 3:
                motorCtrl(pitch, dir, pwm);
                break;
            case 4:
                motorCtrl(base, dir, pwm);
                break;
            case 5:
                motorCtrl(roll, dir, pwm);
                break;
            case 6:
                motorCtrl(grip, dir, pwm);
                break;
            case 7:
                motorCtrl(actuator1, 0, 0);
                motorCtrl(actuator2, 0, 0);
                motorCtrl(pitch, 0, 0);
                motorCtrl(base, 0, 0);
                motorCtrl(roll, 0, 0);
                motorCtrl(grip, 0, 0);
                break;
        }
    }
}