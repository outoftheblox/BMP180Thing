#include <Thing.h>
#include <BlinkPattern.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

using namespace ootb;

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
  
  thing.begin();

  thing.addSensor("bmp180/pressure/" + thing.clientId(), 5000, [](Value& value){
    value = bmp.readPressure();
    led.setPattern(normal);
    Serial.println("Read " + String(value));
  });

  thing.addActuator("bmp180/display/" + thing.clientId(), [](Value& value){
    Serial.println("Got " + String(value));
  });

  if (!bmp.begin())
  {
    Serial.println("Could not find BMP180 sensor at 0x77");
    led.setPattern(panic);
  }
}

void loop()
{
  thing.handle();
  led.handle();
}
