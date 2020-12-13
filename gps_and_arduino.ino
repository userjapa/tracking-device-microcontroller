#include <SoftwareSerial.h>
#include "TinyGPS.h"

#define GPS_RX 3
#define GPS_TX 4

#define GSM_RX 11
#define GSM_TX 12

#define GPS_Serial 9600
#define GSM_Serial 57600

SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);
TinyGPS gps; 

unsigned long v_date, v_time, v_speed;
float v_lat, v_lon;
int v_year;
byte v_month, v_day, v_hour, v_minute, v_second;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  gpsSerial.begin(GPS_Serial);
  gsmSerial.begin(GSM_Serial);
  Serial.println("Started");
}
 
void loop() {
  gpsSerial.listen();
  while (gpsSerial.available()) {
    if (gps.encode(gpsSerial.read())) {
      gps.get_datetime(&v_date, &v_time);
      gps.f_get_position(&v_lat, &v_lon);

      v_speed = gps.f_speed_kmph();

      Serial.println("Sending coords...");
      gsmSerial.listen();
      delay(1000);
      gsmSerial.println("AT");
      delay(500);
      gsmSerial.println("AT+CMGF=1");
      delay(500);
      gsmSerial.println("AT+CMGS=\"+5544998980060\"");
      delay(500);

      char v_link[] = "Coords: ";

      gsmSerial.print("Time: ");
      gsmSerial.print(v_date);
      gsmSerial.print(" ");
      gsmSerial.println(v_time);
      gsmSerial.print("Link: ");
      gsmSerial.print(v_link);
      gsmSerial.print(v_lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : v_lat, 13);
      gsmSerial.print(",");
      gsmSerial.println(v_lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : v_lon, 13);
      gsmSerial.print("Speed: ");
      gsmSerial.println(v_speed == TinyGPS::GPS_INVALID_F_SPEED ? 0.0 : v_speed, 6);
      delay(500);
      
      gsmSerial.print(char(26));
      Serial.println("Coords sent");

      delay(10000);
    }
  }
}
