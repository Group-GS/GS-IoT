# 🌱 Monitoramento Ambiental IoT com ESP32 + Node-RED + MySQL

## 📌 Visão Geral

Este projeto implementa um sistema completo de **monitoramento ambiental IoT**, utilizando um ESP32 (simulado no Wokwi) para coleta de dados e o Node-RED para processamento, análise, visualização e armazenamento.

O sistema monitora:

* 🌡️ Temperatura
* 💧 Umidade
* 🌪️ Pressão atmosférica

Além disso, realiza:

* 📊 Análise de tendência
* ⚠️ Geração de alertas inteligentes
* 🧠 Integração com API externa (clima real)
* 💾 Armazenamento em banco de dados (MySQL)
* 📈 Visualização em dashboard

---

## 🏗️ Arquitetura do Sistema

```
ESP32 → MQTT → Node-RED → Dashboard
                     ↓
                  MySQL
                     ↓
                  Histórico
                     ↓
                  API externa (clima)
```

---

## 🔌 Tecnologias Utilizadas

* ESP32 (Wokwi)
* MQTT (HiveMQ)
* Node-RED
* MySQL
* Node-RED Dashboard
* OpenWeather API

---

## 📡 Fluxo de Dados

### 1. Coleta de Dados (ESP32)

O ESP32 envia dados via MQTT:

* `iot/temperatura`
* `iot/umidade`
* `iot/pressao`

Os valores são simulados dentro dos intervalos:

* Temperatura: -10°C a 60°C
* Umidade: 0% a 100%
* Pressão: 800 a 1200 hPa

---

### 2. Processamento no Node-RED

Cada sensor possui:

#### 🔍 Análise

Classificação dos dados:

* Temperatura: Frio, Normal, Quente, Calor extremo
* Umidade: Seco, Ideal, Úmido
* Pressão: Chuva, Estável, Seco

#### 📈 Tendência

Baseado nos últimos 5 valores:

* Subindo 📈
* Caindo 📉
* Estável ➖

#### 🚨 Alertas

Exemplo:

* Temperatura ≥ 35 → CRÍTICO
* Umidade < 30 → CRÍTICO
* Pressão < 980 → Tempestade

Os alertas são enviados para:

* Dashboard (UI)
* MQTT (LED físico)

---

### 3. Consolidação de Dados

Os dados são acumulados no Node-RED:

```js
{
  temperatura,
  umidade,
  pressao,
  timestamp
}
```

E enviados para o banco MySQL.

---

## 🗄️ Banco de Dados (MySQL)

### 📌 Tabela: sensores

```sql
CREATE TABLE sensores (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperatura FLOAT,
    umidade FLOAT,
    pressao FLOAT,
    data_hora DATETIME
);
```

### 📥 Inserção automática:

```sql
INSERT INTO sensores (temperatura, umidade, pressao, data_hora)
VALUES (?, ?, ?, NOW());
```

---

## 📊 Dashboard (Node-RED)

### Componentes:

* Gauge (tempo real)
* Gráficos (histórico)
* Status textual
* Alertas visuais
* Botões de interação

---

## 📜 Histórico de Dados

### 🔘 Botão: "Carregar Histórico"

Executa:

```sql
SELECT * FROM sensores 
ORDER BY data_hora DESC 
LIMIT 20;
```

### Exibe:

* 📋 Tabela com dados
* 📈 Gráfico de temperatura

---

## 🌍 Integração com API Externa

API utilizada: OpenWeather

### 📡 Endpoint:

```
https://api.openweathermap.org/data/2.5/weather
```

### 📊 Dados exibidos:

* Cidade
* Temperatura atual
* Clima (descrição)
* Alertas básicos (frio/calor)

---

## 🚨 Sistema de Alertas

### Temperatura

| Faixa | Nível   |
| ----- | ------- |
| ≥ 35  | CRÍTICO |
| ≥ 30  | ATENÇÃO |

### Umidade

| Faixa | Nível   |
| ----- | ------- |
| < 30  | CRÍTICO |
| < 40  | ATENÇÃO |
| > 85  | ATENÇÃO |

### Pressão

| Faixa  | Nível   |
| ------ | ------- |
| < 980  | CRÍTICO |
| < 1000 | ATENÇÃO |
| > 1030 | ATENÇÃO |

---

## Youtube

Link: https://www.youtube.com/watch?v=VZ1IeIuFmQQ

---

## 🔁 MQTT Utilizado

### Entrada:

* `iot/temperatura`
* `iot/umidade`
* `iot/pressao`

### Saída:

* `iot/alerta_temp`
* `iot/alerta_umidade`
* `iot/led`

---

## 👨‍💻 Autores

Aluno: Lucas Rafael Solimene / RM: 565194

Aluno: Samyr Couto Oliveira / RM: 565562

Aluno: Henrique Teixeira Cesar / RM: 563088
