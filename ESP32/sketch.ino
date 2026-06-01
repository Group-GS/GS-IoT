#include <WiFi.h>
#include <PubSubClient.h>

// ================= WIFI =================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ================= MQTT =================
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// ================= LEDs =================
#define LED_TEMP 2
#define LED_UMIDADE 4

// ================= VARIÁVEIS SIMULADAS =================
float temp = 25.0;
float hum = 60.0;
float press = 1013.25;

// ================= WIFI =================
void setup_wifi() {
  Serial.print("Conectando no WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
}

// ================= MQTT CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  String topicStr = String(topic);

  Serial.print("Recebido [");
  Serial.print(topicStr);
  Serial.print("]: ");
  Serial.println(msg);

  if (topicStr == "iot/alerta_temp") {
    digitalWrite(LED_TEMP, msg == "ON" ? HIGH : LOW);
  }

  if (topicStr == "iot/alerta_umidade") {
    digitalWrite(LED_UMIDADE, msg == "ON" ? HIGH : LOW);
  }
}

// ================= MQTT =================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println(" conectado!");
      client.subscribe("iot/alerta_temp");
      client.subscribe("iot/alerta_umidade");
    } else {
      Serial.print(" erro...");
      delay(2000);
    }
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  Serial.println("Sistema iniciado!");

  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_UMIDADE, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ================= LOOP =================
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // ===== SIMULAÇÃO TEMPERATURA (-10 a 60) =====
  temp += random(-5, 6) * 0.3;
  if (temp < -10) temp = -10;
  if (temp > 60) temp = 60;

  // ===== SIMULAÇÃO UMIDADE (0 a 100) =====
  hum += random(-5, 6) * 0.5;
  if (hum < 0) hum = 0;
  if (hum > 100) hum = 100;

  // ===== SIMULAÇÃO PRESSÃO (800 a 1200) =====
  press += random(-5, 6) * 0.8;
  if (press < 800) press = 800;
  if (press > 1200) press = 1200;

  Serial.println("------");

  Serial.print("Temp: ");
  Serial.println(temp);

  Serial.print("Umidade: ");
  Serial.println(hum);

  Serial.print("Pressao: ");
  Serial.println(press);

  client.publish("iot/temperatura", String(temp).c_str());
  client.publish("iot/umidade", String(hum).c_str());
  client.publish("iot/pressao", String(press).c_str());

  Serial.println("Enviado para MQTT!");

  delay(3000);
}