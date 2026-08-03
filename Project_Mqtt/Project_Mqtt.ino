#include<WiFi.h>
#include<ArduinoMqttClient.h>


//UART port 2
HardwareSerial StmSerial(2);

const int RX_PIN = 16;
const int TX_PIN = 17;


void setup() {
  // put your setup code here, to run once:
  
  //main Serial Monitor for debug
  Serial.begin(115200);

  // Custom Uart to recieve from STM32
  // StmSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

 
}

void loop() {
  // put your main code here, to run repeatedly:
  publisher.poll();

  while (StmSerial.available())
  {
    String data = StmSerial.readStringUntil('\n');
    Serial.println(data);
  }

  // JSON Data 
  // {
  //   "temperature": 28.3,
  //   "humidity":65.3,
  //   "airquality":"good",
  //   "pm2.5":29,
  //   "alarm":1
  // };

  
  delay(5000);
}
