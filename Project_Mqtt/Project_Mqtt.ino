#include<WiFi.h>
#include<ArduinoMqttClient.h>


//UART port 2
HardwareSerial StmSerial(2);

const int RX_PIN = 16;
const int TX_PIN = 17;

const char *ssid = "POCOF5";
const char *password = "1234567890";

const char *host = "10.163.98.34";
const int port = 1883;
const char *topic = "airsense/data";

WiFiClient wifiClient;
MqttClient publisher(wifiClient);


void setup() {
  // put your setup code here, to run once:
  
  //main Serial Monitor for debug
  Serial.begin(115200);

  // Custom Uart to recieve from STM32
  StmSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nConnected to WiFi");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  if(!publisher.connect(host, port))
  {
    Serial.print("MQTT connection failed: ");
    Serial.println(publisher.connectError());
  }
  Serial.println("Connected to the broker !!!");

 
}

void loop() {
  // put your main code here, to run repeatedly:
  publisher.poll();

  while (StmSerial.available())
  {
    String data = StmSerial.readStringUntil('\n');
    publisher.beginMessage(topic);
    publisher.print(data);
    publisher .endMessage();
  }

  // JSON Data 
  // {
  //   "temperature": 28.3,
  //   "humidity":65.3,
  //   "airquality":"good",
  //   "pm2.5":29,
  //   "alarm":1
  // };

}
