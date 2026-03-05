#include <TinyGPS++.h>

TinyGPSPlus gps;

HardwareSerial gpsSerial(2);

void setup()
{
  Serial.begin(115200);

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Testing GPS Module...");
}

void loop()
{
  while (gpsSerial.available() > 0)
  {
    gps.encode(gpsSerial.read());

    if (gps.location.isUpdated())
    {
      Serial.print("Latitude  : ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitude : ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());

      Serial.print("Altitude  : ");
      Serial.print(gps.altitude.meters());
      Serial.println(" m");

      Serial.println("----------------------");
    }
  }
}
