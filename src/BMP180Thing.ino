#include <Thing.h>
#include <BlinkPattern.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);
Adafruit_BMP085 bmp;

BlinkPattern::Pattern<2> panic{{1,1},25};
BlinkPattern::Pattern<2> start{{1,9},25};
BlinkPattern::Pattern<2> normal{{1,39},25};

void setup() 
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("Client:" + thing.clientId());

  led.setPattern(start);
  
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  String sensor;
  sensor += "sensor/bmp180/";
  sensor += thing.clientId();
  thing.addSensor(sensor, 5000, [](Value& value){
    value = bmp.readPressure();
    led.setPattern(normal);
    //Serial.println("Read " + value);
  });

  String display;
  display += "display/bmp180/";
  display += thing.clientId();
  thing.addActuator(display, [](Value& value){
    //Serial.println("Got " +(String) value);
  });

  if (!bmp.begin()) 
  {
    Serial.println("Could not find BMP180 sensor at 0x77");
    led.setPattern(panic);
  }
  
  thing.begin();
}

void loop()
{
  thing.handle();
  led.handle();
}
