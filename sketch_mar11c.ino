#define BLYNK_TEMPLATE_ID "user17"
#define BLYNK_TEMPLATE_NAME "user17@server.wyns.it"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHT_PIN 15
#define LED_PIN 19
#define POTENTIOMETER_PIN 34

char auth[] = "rNMOW0VqlnWRbo4ztkvELneVv-vsd_Dx"; 
char ssid[] = "embed";
char pass[] = "weareincontrol";

float gemetenTemp = 0; 
int ketelStatus = 0; // 0: uit, 30: aan
bool useSlider = true;
float gewensteTemp = 0;

DHT dht(DHT_PIN, DHT11);

BLYNK_WRITE(V2)
{
  if (!useSlider) {
    gewensteTemp = param.asFloat();
  }
}
BLYNK_WRITE(V7) {
  useSlider = !useSlider;
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
}

void loop() {
  Blynk.run();
  
  gemetenTemp = dht.readTemperature();

  if (useSlider) {
    int potValue = analogRead(POTENTIOMETER_PIN);
    gewensteTemp = map(potValue, 0, 4096, 0, 30);
  }
  
  if (gemetenTemp < gewensteTemp) {
    digitalWrite(LED_PIN, HIGH); 
    ketelStatus = 30;
  } else {
    digitalWrite(LED_PIN, LOW);
    ketelStatus = 0;
  }

  Serial.print("Gemeten temperatuur: ");
  Serial.print(gemetenTemp);
  Serial.print(" °C, Gewenste temperatuur: ");
  Serial.print(gewensteTemp);
  Serial.print(" °C, Verwarmingsketel: ");
  Serial.println(ketelStatus ? "Aan" : "Uit");

  Blynk.virtualWrite(V1, gemetenTemp);
  Blynk.virtualWrite(V2, gewensteTemp);
  Blynk.virtualWrite(V3, ketelStatus);
  delay(2000);
}
