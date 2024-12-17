// Libraries
#include <Wire.h>
#include <MQUnifiedsensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Sensor Pins
#define MQ131_PIN A0  // Ozone
#define MQ135_PIN A2  // NH4/CO2
#define MQ4_PIN   A3  // Methane
#define MQ7_PIN   A1  // CO
#define UV_PIN    A6  // HW-837 UV
#define PH_PIN    A5  // pH Sensor

// Soil Sensors
#define ONE_WIRE_BUS 4   // Soil Temperature
#define SOIL_MOISTURE_PIN A4  

// Constants
#define BOARD "Arduino MEGA"
#define VOLTAGE_RESOLUTION 5.0
#define ADC_BIT_RESOLUTION 10

// Calibration for MQ Sensors
#define RATIO_MQ131_CLEAN_AIR 15
#define RATIO_MQ135_CLEAN_AIR 3.6
#define RATIO_MQ4_CLEAN_AIR   4.4
#define RATIO_MQ7_CLEAN_AIR   27

// Capacitive Soil Moisture Sensor Calibration
const int AIR_VALUE = 540;    // Sensor reading in dry air
const int WATER_VALUE = 300;  // Sensor reading when fully submerged

// Sensor Objects
MQUnifiedsensor MQ131(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, MQ131_PIN, "MQ-131");
MQUnifiedsensor MQ135(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, MQ135_PIN, "MQ-135");
MQUnifiedsensor MQ4(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, MQ4_PIN, "MQ-4");
MQUnifiedsensor MQ7(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, MQ7_PIN, "MQ-7");

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasSensors(&oneWire);

// Variables
int phReadings[10];


// Definitions for Stepper motor
#define dirPin 2
#define stepPin 4
#define delayTime 10000

// Relay
#define relayPin 6
int relayState = 0;


// Functions for Initialization and Readings
void initMQSensor(MQUnifiedsensor &sensor, float cleanAirRatio, float a, float b) {
    sensor.setRegressionMethod(1); // Use ppm calculation
    sensor.setA(a);
    sensor.setB(b);
    sensor.init();
    calibrateSensor(sensor, cleanAirRatio);
}

void calibrateSensor(MQUnifiedsensor &sensor, float cleanAirRatio) {
    Serial.print("Calibrating... ");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++) {
        sensor.update();
        calcR0 += sensor.calibrate(cleanAirRatio);
        delay(1000);  // Wait between calibration reads
    }
    sensor.setR0(calcR0 / 10);
    Serial.println("Done!");
}

float readSensor(MQUnifiedsensor &sensor) {
    sensor.update();
    return sensor.readSensor();
}

float readUVSensor(int pin) {
    float sensorValue = analogRead(pin);
    return sensorValue / 1024.0 * 5.0;  // Convert to voltage
}

float readPHSensor(int pin) {
    for (int i = 0; i < 10; i++) {
        phReadings[i] = analogRead(pin);
        delay(10);
    }
    int sum = 0, minValue = phReadings[0], maxValue = phReadings[0];
    for (int i = 0; i < 10; i++) {
        sum += phReadings[i];
        minValue = min(minValue, phReadings[i]);
        maxValue = max(maxValue, phReadings[i]);
    }
    sum -= (minValue + maxValue);
    return (float)sum / 8 * 5.0 / 1024 * 3.5;  // Example calibration factor
}

float readSoilTemperature() {
    dallasSensors.requestTemperatures();
    return dallasSensors.getTempCByIndex(0);
}

int readSoilMoisture(int pin) {
    int sensorValue = analogRead(pin);
    int moisturePercent = map(sensorValue, AIR_VALUE, WATER_VALUE, 0, 100);
    return constrain(moisturePercent, 0, 100);  // Keep within 0-100%
}


// Move Stepper Function
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


void setup() {
    // Initialize MQ Sensors
    initMQSensor(MQ131, RATIO_MQ131_CLEAN_AIR, 23.943, -1.11);
    initMQSensor(MQ135, RATIO_MQ135_CLEAN_AIR, 3, -2.473);
    initMQSensor(MQ4, RATIO_MQ4_CLEAN_AIR, 10, -2.0);
    initMQSensor(MQ7, RATIO_MQ7_CLEAN_AIR, 20.0, -1.5);

    // Initialize Dallas Sensor
    dallasSensors.begin();
    pinMode(UV_PIN, INPUT);

    // Stepper
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);

    // Relay
    pinMode(relayPin, OUTPUT);

    Serial.begin(115200);
}


void loop() {
    // Stepper control + Relay control
    if(Serial.available()>0){
        uint8_t state = (uint8_t)Serial.read();

        Serial.println(state);

        if(state > 0 && state <= 200){
            moveStepper(0, state);
        }
        if(state == 201){
            relayState = !relayState;
            digitalWrite(relayPin, relayState);
        }
    }

    // Take Readings
    float uvVoltage = readUVSensor(UV_PIN);
    float mq7Value = readSensor(MQ7);  // Get ppm value for CO sensor
    float phValue = readPHSensor(PH_PIN);
    float ozoneValue = readSensor(MQ131);  // Ozone sensor ppm value
    float nh4Value = readSensor(MQ135);   // NH4/CO2 sensor ppm value
    float methaneValue = readSensor(MQ4); // Methane sensor ppm value
    float soilTemp = readSoilTemperature();
    int moisturePercent = readSoilMoisture(SOIL_MOISTURE_PIN);

    // Display Readings
    Serial.println("\n--- Sensor Readings ---");
    Serial.print("UV Voltage: "); Serial.println(uvVoltage, 2);
    Serial.print("CO (MQ-7): "); Serial.println(mq7Value, 2);  // ppm value
    Serial.print("pH: "); Serial.println(phValue, 2);
    
    // Display only ppm values for MQ Sensors (No extra details)
    Serial.print("Ozone (MQ-131): "); Serial.println(ozoneValue, 2);  // ppm value
    Serial.print("NH4 (MQ-135): "); Serial.println(nh4Value, 2);  // ppm value
    Serial.print("Methane (MQ-4): "); Serial.println(methaneValue, 2);  // ppm value

    Serial.print("Soil Temp: "); Serial.println(soilTemp, 2);
    Serial.print("Soil Moisture: "); Serial.println(moisturePercent);

    delay(500);
}
