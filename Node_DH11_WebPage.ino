#include <SimpleDHT.h>
#include <ESP8266WiFi.h>

const char* ssid = "blacky99"; //your-SSID
const char* password = "blacky99"; //your-password
const char* node_name = "NodeMCU V2 Development Kit"; //your NodeMCU

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

int pinDHT11 = D1;
SimpleDHT11 dht11;
int tempValue, humidValue;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("NodeMCU server started");
  
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/nodeMCU/dht11\n");

  // DHT11 start working...
  Serial.println("/*** Initialize DHT11... ***/");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println();Serial.println("Request from client");
  while(!client.available()){
    delay(1);
  }

  digitalWrite(LED_BUILTIN, LOW);
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
  
  Serial.print("Sample OK: ");Serial.print("Temp = ");
  Serial.print((int)temperature); Serial.print(" *C, "); Serial.print("Humidity = ");
  Serial.print((int)humidity); Serial.println(" %");

  tempValue = (int)temperature;humidValue = (int)humidity;

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  String s;
  if(req.indexOf("/nodeMCU/dht11") != -1){
    // Prepare the response
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<h2>This is the message from your ";
    s += "\""; s += node_name; s += "\""; s += "</h2>";
    s += "<br> Temperature is now "; s += tempValue; s += " degree celcius";
    s += "<br> Humidity is now "; s += humidValue; s += " percent<br>";
    s += "<br><h2 style=\"color:red;\">  End of the message  </h2>";
    s += "</html>\n";
  }else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

 
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
