#include <MQUnifiedsensor.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

#include <SoftwareSerial.h>
#include <MAVLink.h>
SoftwareSerial pixhawkSerial(10, 11); // RX, TX


#define DHT22_PIN 13
#define DHT22_TYPE DHT22
DHT dht22(DHT22_PIN, DHT22_TYPE);
Adafruit_BME280 bme;
#define SEALEVELPRESSURE_HPA (1013.25)
const unsigned long DATA_UPDATE_INTERVAL = 2000; // 2 seconds
unsigned long previousMillis = 0; // Stores the last time data was updated


#define placa "Arduino UNO"
#define Voltage_Resolution 5

#define MQ131_PIN A1
#define MQ135_PIN A3
#define MQ7_PIN A2
#define UV_PIN A0

#define typeMQ131 "MQ-131" //MQ131
#define typeMQ135 "MQ-135" //MQ135
#define typeMQ7 "MQ-7"

#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO

#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm 
#define RatioMQ131CleanAir 15
#define RatioMQ7CleanAir 27.5

MQUnifiedsensor MQ131(placa, Voltage_Resolution, ADC_Bit_Resolution, MQ131_PIN, typeMQ131);
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, MQ135_PIN, typeMQ135);
MQUnifiedsensor MQ7(placa, Voltage_Resolution, ADC_Bit_Resolution, MQ7_PIN, typeMQ7);


void setup() {
  // put your setup code here, to run once:
  MQ_initialise(MQ135,102.2,-2.473, RatioMQ135CleanAir);
  MQ_initialise(MQ131, 23.943, -1.11, RatioMQ131CleanAir);
  MQ_initialise(MQ7, 99.042, -1.518, RatioMQ7CleanAir);
  pixhawkSerial.begin(57600);
  dht22.begin();

  // Initialize the BME280 sensor (default I2C address is 0x76, or 0x77 for some modules)
  if (!bme.begin(0x76)) {
    while (1); // Stop execution if the sensor is not found
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= DATA_UPDATE_INTERVAL) {
    previousMillis = currentMillis;
    // put your main code here, to run repeatedly:
    MQ131.update();
    float mq131reading = MQ131.readSensor();
    MQ135.update();
    float mq135reading = MQ135.readSensor();
    MQ7.update();
    float mq7reading = MQ7.readSensor();

    float uvVoltage = analogRead(UV_PIN) * (5.0 / 1023.0);

    // Read temperature and humidity from DHT22
    float h22 = dht22.readHumidity();
    float t22 = dht22.readTemperature();

    float pressure = bme.readPressure() / 100.0F; // Convert pressure to hPa
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

    sendToBase("mq131", mq131reading);
    sendToBase("mq135", mq135reading);
    sendToBase("mq7", mq7reading);
    sendToBase("uv", uvVoltage);
    sendToBase("humidity", h22);
    sendToBase("temp", t22);
    sendToBase("pressure", pressure);
    sendToBase("altitude", altitude);
  }

  delay(500);
}

void MQ_initialise(MQUnifiedsensor &mq_sensor, float a, float b, float ratioCleanAir){
  mq_sensor.setRegressionMethod(1);
  mq_sensor.setA(a); mq_sensor.setB(b);

  mq_sensor.init();
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    mq_sensor.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += mq_sensor.calibrate(ratioCleanAir);    
  }
  mq_sensor.setR0(calcR0/10);
  
}

void sendToBase(const char* sensorName, float sensorData) {
  mavlink_message_t msg;
  uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

  mavlink_msg_named_value_float_pack(
    1,                      // System ID
    200,                    // Component ID
    &msg,                   // MAVLink message
    millis(),               // Time since boot (ms)
    sensorName,             // Sensor name
    sensorData              // Sensor value
  );

  uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
  pixhawkSerial.write(buffer, len); // Send MAVLink message
}

