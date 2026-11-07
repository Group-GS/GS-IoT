#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>

// ================================================
// ALERTACIDADAO - MONITORAMENTO DE ENCHENTES
// ESP32 + HC-SR04 + potenciometro (chuva simulada)
// ================================================

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

const char* MQTT_BROKER = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;

const char* PONTO_MONITORAMENTO = "alertacidadao_01";
const char* MQTT_TOPIC =
  "enchente/alertacidadao_01/telemetria";

const int PIN_TRIG = 5;
const int PIN_ECHO = 18;
const int PIN_CHUVA = 34;

// Altura entre o sensor e o fundo do local monitorado.
const float ALTURA_SENSOR_CM = 200.0f;

// O potenciometro representa chuva de 0 a 100 mm.
const float CHUVA_SIMULADA_MAX_MM = 100.0f;

const unsigned long INTERVALO_LEITURA_MS = 5000;
const unsigned long INTERVALO_WIFI_RETRY_MS = 5000;
const unsigned long INTERVALO_MQTT_RETRY_MS = 5000;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long ultimaLeituraMs = 0;
unsigned long ultimaTentativaWiFiMs = 0;
unsigned long ultimaTentativaMqttMs = 0;

// ================================================
// WIFI
// ================================================

void conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Conectando ao Wi-Fi ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  ultimaTentativaWiFiMs = millis();
}

void manterWiFiConectado() {
  if (WiFi.status() == WL_CONNECTED) return;

  const unsigned long agora = millis();

  if (agora - ultimaTentativaWiFiMs >=
      INTERVALO_WIFI_RETRY_MS) {
    conectarWiFi();
  }
}

// ================================================
// DATA E HORA VIA NTP
// ================================================

void configurarHorarioNtp() {
  configTime(
    0,
    0,
    "pool.ntp.org",
    "time.google.com"
  );

  Serial.print("Sincronizando horario NTP");

  const unsigned long inicio = millis();

  while (
    time(nullptr) < 1700000000 &&
    millis() - inicio < 15000
  ) {
    delay(250);
    Serial.print(".");
  }

  Serial.println();

  if (time(nullptr) >= 1700000000) {
    Serial.println("Horario sincronizado.");
  } else {
    Serial.println(
      "NTP indisponivel. O envio aguardara horario valido."
    );
  }
}

bool obterDataHoraISO8601(
  char* destino,
  size_t tamanho
) {
  const time_t agora = time(nullptr);

  if (agora < 1700000000) {
    return false;
  }

  struct tm utc;
  gmtime_r(&agora, &utc);

  return strftime(
    destino,
    tamanho,
    "%Y-%m-%dT%H:%M:%SZ",
    &utc
  ) > 0;
}

// ================================================
// MQTT
// ================================================

bool conectarMQTT() {
  if (mqttClient.connected()) return true;

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  char clientId[48];

  const uint64_t mac = ESP.getEfuseMac();

  snprintf(
    clientId,
    sizeof(clientId),
    "AlertaCidadao-%04X%08X",
    (uint16_t)(mac >> 32),
    (uint32_t)mac
  );

  Serial.print("Conectando ao broker MQTT...");

  if (mqttClient.connect(clientId)) {
    Serial.println(" conectado!");
    return true;
  }

  Serial.print(" falhou. Estado MQTT: ");
  Serial.println(mqttClient.state());

  return false;
}

void manterMQTTConectado() {
  if (
    mqttClient.connected() ||
    WiFi.status() != WL_CONNECTED
  ) {
    return;
  }

  const unsigned long agora = millis();

  if (
    agora - ultimaTentativaMqttMs >=
    INTERVALO_MQTT_RETRY_MS
  ) {
    ultimaTentativaMqttMs = agora;
    conectarMQTT();
  }
}

// ================================================
// SENSOR ULTRASSONICO
// ================================================

float lerDistanciaCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(3);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_TRIG, LOW);

  const unsigned long duracao = pulseIn(
    PIN_ECHO,
    HIGH,
    30000UL
  );

  if (duracao == 0) {
    return -1.0f;
  }

  return (float)duracao / 58.0f;
}

// ================================================
// CALCULO DO NIVEL DA AGUA
// ================================================

float calcularNivelAguaCm(float distanciaCm) {
  float nivel =
    ALTURA_SENSOR_CM - distanciaCm;

  if (nivel < 0.0f) {
    nivel = 0.0f;
  }

  if (nivel > ALTURA_SENSOR_CM) {
    nivel = ALTURA_SENSOR_CM;
  }

  return nivel;
}

// ================================================
// SENSOR DE CHUVA SIMULADO
// ================================================

float lerChuvaSimuladaMm() {
  const int leituraADC = analogRead(PIN_CHUVA);

  return (
    (float)leituraADC / 4095.0f
  ) * CHUVA_SIMULADA_MAX_MM;
}

// ================================================
// PUBLICACAO DA TELEMETRIA
// ================================================

void publicarTelemetria() {
  if (!mqttClient.connected()) {
    Serial.println(
      "MQTT desconectado; telemetria nao publicada."
    );
    return;
  }

  const float distanciaCm = lerDistanciaCm();

  if (
    distanciaCm < 0.0f ||
    distanciaCm < 2.0f ||
    distanciaCm > 400.0f
  ) {
    Serial.println(
      "Leitura HC-SR04 invalida ou sem eco."
    );
    return;
  }

  const float nivelAguaCm =
    calcularNivelAguaCm(distanciaCm);

  const float chuvaMm =
    lerChuvaSimuladaMm();

  char dataHora[30];

  if (
    !obterDataHoraISO8601(
      dataHora,
      sizeof(dataHora)
    )
  ) {
    Serial.println(
      "Aguardando sincronizacao NTP."
    );
    return;
  }

  char payload[256];

  const int tamanho = snprintf(
    payload,
    sizeof(payload),
    "{\"ponto_monitoramento\":\"%s\","
    "\"nivel_agua_cm\":%.2f,"
    "\"chuva_mm\":%.2f,"
    "\"data_hora\":\"%s\"}",
    PONTO_MONITORAMENTO,
    nivelAguaCm,
    chuvaMm,
    dataHora
  );

  if (
    tamanho <= 0 ||
    tamanho >= (int)sizeof(payload)
  ) {
    Serial.println("Erro ao montar JSON.");
    return;
  }

  const bool enviado = mqttClient.publish(
    MQTT_TOPIC,
    payload
  );

  Serial.println("----------------------------------------");

  Serial.print("Topico: ");
  Serial.println(MQTT_TOPIC);

  Serial.print("Distancia ate a agua: ");
  Serial.print(distanciaCm, 2);
  Serial.println(" cm");

  Serial.print("Nivel estimado da agua: ");
  Serial.print(nivelAguaCm, 2);
  Serial.println(" cm");

  Serial.print("Chuva simulada: ");
  Serial.print(chuvaMm, 2);
  Serial.println(" mm");

  Serial.print("JSON: ");
  Serial.println(payload);

  Serial.println(
    enviado
      ? "Publicacao MQTT: OK"
      : "Publicacao MQTT: FALHOU"
  );
}

// ================================================
// SETUP
// ================================================

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_CHUVA, INPUT);

  analogReadResolution(12);

  mqttClient.setServer(
    MQTT_BROKER,
    MQTT_PORT
  );

  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(5);
  mqttClient.setBufferSize(256);

  Serial.println();
  Serial.println(
    "=== AlertaCidadao | Monitoramento de Enchentes ==="
  );

  Serial.print("Ponto de monitoramento: ");
  Serial.println(PONTO_MONITORAMENTO);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  ultimaTentativaWiFiMs = millis();

  const unsigned long inicioWiFi = millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - inicioWiFi < 20000
  ) {
    delay(250);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi conectado. IP: ");
    Serial.println(WiFi.localIP());

    configurarHorarioNtp();
    conectarMQTT();
  } else {
    Serial.println(
      "Wi-Fi ainda nao conectado. Continuara tentando."
    );
  }
}

// ================================================
// LOOP
// ================================================

void loop() {
  manterWiFiConectado();
  manterMQTTConectado();

  mqttClient.loop();

  const unsigned long agora = millis();

  if (
    agora - ultimaLeituraMs >=
    INTERVALO_LEITURA_MS
  ) {
    ultimaLeituraMs = agora;
    publicarTelemetria();
  }

  delay(10);
}