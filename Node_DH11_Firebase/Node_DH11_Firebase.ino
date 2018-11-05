#include <DHT.h>                // Include DHT sensor library
#define DHTTYPE DHT11           // Define type of sensor 
#define DHTPIN  D4              // Define connected pin

DHT dht(DHTPIN, DHTTYPE, 15);   // Initial DHT sensor

void setup() 
{
  dht.begin();                  // Start DHT sensor operation
  Serial.begin(115200);       // set serial port at 115200 bps
}
void loop() 
{
  float humid = dht.readHumidity();     // Read humidity data
  float temp = dht.readTemperature();   // Read temperature data
  Serial.print("Humidity = ");       // Send string to serial port.
  Serial.print(humid);            // Send analog value to serial port.
  Serial.print(" Temperature = ");       // Send string to serial port.
  Serial.println(temp);            // Send analog value to serial port.
  delay(300);                          // Delay 1 second before restart
}
