#define SENSOR_BIT 9
#include <SchooMyUtilities.h>
#include <OneWire.h>
#include <DallasTemperature.h>
SchooMyUtilities scmUtils = SchooMyUtilities();

OneWire _tempWire_5(5);
DallasTemperature _tempSensors_5(&_tempWire_5);

float _sbeGetBrightness(int pinNumber, int res, float vol) {
  pinMode(pinNumber, INPUT);
  float cds_ad = analogRead(pinNumber);
  float cds_v = cds_ad * vol / res;
  float v_res = vol - cds_v;
  if (v_res < 0.01) {
    v_res = 0.01;
  }
  return 10000 * cds_v / v_res / 1000;
}

float _sbeGetTemperature(DallasTemperature& sensors) {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void setup() {
  Serial.begin(9600);
  _tempSensors_5.begin();
  _tempSensors_5.setResolution(SENSOR_BIT);
  scmUtils.soundSensorBegin(A1);
}

void loop() {
  float temp = _sbeGetTemperature(_tempSensors_5);
  float sound = analogRead(A1);
  float brightness = _sbeGetBrightness(A5, 1023, 5);

  float tempScore    = constrain(map((long)abs(temp - 23), 0, 10, 0, 10), 0, 10);
  float soundScore   = constrain(map((long)sound, 0, 1023, 0, 10), 0, 10);
  float brightScore  = constrain(map((long)brightness, 0, 15, 0, 10), 0, 10);

  float sleepIndex = tempScore + soundScore + brightScore;

  Serial.print("温度: ");       Serial.print(temp);       Serial.print(" °C  |  ");
  Serial.print("音: ");         Serial.print(sound);      Serial.print("  |  ");
  Serial.print("明るさ指数: "); Serial.print(brightness); Serial.print("  |  ");
  Serial.print("睡眠快適指数: "); Serial.println(sleepIndex);

  delay(500);
}
