#define BLYNK_TEMPLATE_ID "TMPL2q8BWFCdv"
#define BLYNK_TEMPLATE_NAME "Fire sensor"
#define BLYNK_AUTH_TOKEN "3o0LBidUuONBM13bHljIvH673HPmJ56W"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "XX";
char pass[] = "XXXXXXXXXX";

#define DHTPIN 4
#define DHTTYPE DHT11
#define GAS_PIN 34       // MQ sensor analog input

#define BUZZER_PIN 26    // GPIO connected to buzzer
#define LIGHT_PIN 27     // GPIO connected to light/LED

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LIGHT_PIN, LOW);

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensor);  // Read and send every 5 sec
}

void loop() {
  Blynk.run();
  timer.run();
}

// Send temperature, humidity, gas values to Blynk
void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gas = analogRead(GAS_PIN);

  if (isnan(h) || isnan(t)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
    return;
  }

  Blynk.virtualWrite(V0, gas);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  Serial.print("📤 Sent - Gas: ");
  Serial.print(gas);
  Serial.print(" | Temp: ");
  Serial.print(t);
  Serial.print(" °C | Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}

// Receive prediction result from Python backend (class 0 or 1)
BLYNK_WRITE(V3) {
  int predictedClass = param.asInt();
  Serial.print("📩 Received prediction: Class ");
  Serial.println(predictedClass);

  if (predictedClass == 0) { // turn ON at class 0
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LIGHT_PIN, HIGH);
    Serial.println("⚠️ Alert! Buzzer and Light ON (Class 0)");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LIGHT_PIN, LOW);
    Serial.println("✅ Safe. Buzzer and Light OFF (Class 1)");
  }
}

// Manual buzzer control from Blynk (Button on V4)
BLYNK_WRITE(V4) {
  int buzzerControl = param.asInt();  // 1 = ON, 0 = OFF
  if (buzzerControl == 1) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("📲 Buzzer turned ON from cloud");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("📲 Buzzer turned OFF from cloud");
  }
}
