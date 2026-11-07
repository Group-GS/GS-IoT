# AlertaCidadão — Monitoramento Inteligente de Enchentes

## 1. Sobre o projeto

O **AlertaCidadão** é uma solução de monitoramento inteligente de enchentes que utiliza Internet das Coisas (IoT), Machine Learning e Inteligência Artificial Generativa para acompanhar condições de risco em áreas sujeitas a alagamentos.

A proposta é coletar informações por meio de sensores, analisar os dados para classificar o nível de risco e apresentar mensagens explicativas com orientações preventivas para a população.

O projeto busca contribuir com a prevenção de desastres, a disseminação de informações e o apoio à tomada de decisão dos cidadãos.

> **Importante:** O AlertaCidadão é um protótipo acadêmico. Ele não substitui os sistemas oficiais de monitoramento, os alertas da Defesa Civil ou as orientações das autoridades competentes.

## 2. Problema identificado

Enchentes e alagamentos podem causar prejuízos materiais, colocar vidas em risco e dificultar a mobilidade da população.

A falta de informações acessíveis e compreensíveis sobre as condições de risco pode prejudicar a prevenção e a preparação dos moradores de regiões vulneráveis.

O AlertaCidadão propõe utilizar sensores e modelos inteligentes para transformar medições ambientais em informações mais claras e acessíveis.

## 3. Público beneficiado

- Moradores de regiões sujeitas a enchentes e alagamentos.
- Comunidades em áreas de risco.
- Pessoas que precisam acompanhar condições ambientais antes de se deslocar.
- Órgãos e iniciativas relacionados à prevenção de desastres e à proteção civil.

## 4. Objetivos

- Monitorar o nível da água e a quantidade de chuva.
- Calcular a velocidade de elevação do nível da água.
- Classificar o risco utilizando Machine Learning.
- Gerar explicações e orientações por meio de IA Generativa.
- Apresentar os dados em um dashboard de monitoramento.
- Demonstrar como IoT e Inteligência Artificial podem contribuir para iniciativas CitizenTech.

## 5. Arquitetura da solução

A solução é organizada em cinco componentes principais:

1. **ESP32 e sensores:** coletam as informações ambientais e enviam os dados por MQTT.
2. **Node-RED:** recebe as mensagens, processa os dados e integra os serviços.
3. **MySQL:** armazena o histórico das medições.
4. **API de Machine Learning:** classifica o nível de risco.
5. **API de IA Generativa:** transforma a classificação e as medições em uma mensagem explicativa.

O dashboard do Node-RED apresenta as informações coletadas, o histórico e os resultados da análise.

### Fluxo de funcionamento

```text
Sensores
   |
   v
ESP32
   |
   | MQTT
   v
Node-RED
   |
   +------> MySQL
   |
   +------> API Machine Learning
   |              |
   |              v
   |       Classificação do risco
   |              |
   +------> API IA Generativa
   |              |
   |              v
   |       Mensagem explicativa
   |
   v
Dashboard
```

## 6. Tecnologias utilizadas

| Tecnologia | Aplicação |
|---|---|
| ESP32 | Processamento e envio das medições |
| HC-SR04 | Simulação da medição de distância até a superfície da água |
| Potenciômetro | Simulação da intensidade de chuva |
| Wokwi | Simulação do circuito eletrônico |
| MQTT | Comunicação entre o ESP32 e o Node-RED |
| HiveMQ | Broker MQTT |
| Node-RED | Integração, processamento e dashboard |
| MySQL | Armazenamento das medições |
| Python | Desenvolvimento das APIs |
| FastAPI | Criação dos serviços HTTP |
| Scikit-learn | Desenvolvimento do modelo de Machine Learning |
| Random Forest | Classificação do nível de risco |
| OpenAI API | Geração de mensagens explicativas |
| PowerShell | Execução e validação dos serviços |

## 7. Monitoramento IoT

O protótipo utiliza um ESP32 conectado a dois componentes:

- **Sensor ultrassônico HC-SR04:** utilizado para simular a medição do nível da água.
- **Potenciômetro:** utilizado para simular a intensidade da chuva.

Os dados são enviados por MQTT para o seguinte tópico:

```text
enchente/alertacidadao_01/telemetria
```

Broker utilizado:

```text
broker.hivemq.com
Porta: 1883
```

### Exemplo de mensagem MQTT

```json
{
  "ponto_monitoramento": "alertacidadao_01",
  "nivel_agua_cm": 85.5,
  "chuva_mm": 12.4,
  "data_hora": "2026-11-06T08:00:00Z"
}
```

O Node-RED recebe os dados, valida as informações e calcula a velocidade de elevação da água com base nas medições anteriores.

**Observação:** os componentes são utilizados em uma simulação acadêmica. O potenciômetro representa a intensidade de chuva, não uma medição pluviométrica real.

## 8. Machine Learning

O projeto utiliza um modelo **Random Forest Classifier**, desenvolvido com Scikit-learn.

A função do modelo é classificar as condições de monitoramento em quatro categorias:

| Classificação | Significado |
|---|---|
| NORMAL | Condições consideradas normais pelo modelo |
| ATENCAO | Condições que exigem acompanhamento |
| ALERTA | Condições que indicam risco elevado |
| EMERGENCIA | Condições classificadas como críticas |

### Variáveis utilizadas

- Nível da água em centímetros.
- Intensidade da chuva em milímetros.
- Velocidade de elevação do nível da água em centímetros por hora.

O modelo também retorna uma probabilidade associada à classificação e fatores utilizados para explicar o resultado.

### API de Machine Learning

A API foi desenvolvida com FastAPI e disponibiliza os seguintes endpoints:

| Método | Endpoint | Descrição |
|---|---|---|
| GET | `/health` | Verifica a disponibilidade da API |
| GET | `/metrics` | Disponibiliza informações de métricas do modelo |
| POST | `/predict` | Realiza a classificação do risco |
| GET | `/docs` | Documentação interativa da API |

### Exemplo de requisição

```json
{
  "ponto_monitoramento": "alertacidadao_01",
  "nivel_agua_cm": 85.5,
  "chuva_mm": 12.4,
  "elevacao_cm_h": 3.2,
  "data_hora": "2026-11-06T08:00:00Z"
}
```

### Treinamento e limitações

O protótipo utiliza um conjunto de 6.000 amostras sintéticas para treinamento inicial do Random Forest.

Esses dados permitem demonstrar o funcionamento da classificação, mas não representam um conjunto histórico real de enchentes.

Para uma aplicação em ambiente real, será necessário obter dados históricos rotulados, validar o modelo com dados independentes e avaliar seu desempenho para cada categoria de risco.

## 9. Inteligência Artificial Generativa

A solução utiliza a API da OpenAI para transformar o resultado do Machine Learning em uma explicação compreensível para o cidadão.

A IA Generativa recebe:

- Classificação de risco produzida pelo modelo.
- Probabilidade retornada pelo modelo.
- Fatores associados à classificação.
- Medições ambientais disponíveis.

Com essas informações, o serviço deve produzir uma mensagem clara, contextualizada e com orientações preventivas.

**A IA Generativa não realiza a classificação do risco.** Essa responsabilidade pertence exclusivamente ao modelo de Machine Learning.

A IA Generativa também não deve inventar medições, alterar a classificação recebida ou apresentar informações como se fossem alertas oficiais.

### API de IA Generativa

A segunda API também foi desenvolvida com FastAPI.

| Método | Endpoint | Descrição |
|---|---|---|
| GET | `/health` | Verifica a disponibilidade do serviço |
| POST | `/gerar-alerta` | Solicita a geração de uma mensagem |
| GET | `/docs` | Documentação interativa da API |

### Configuração da chave

A chave da OpenAI deve ser configurada por variável de ambiente, utilizando um arquivo `.env` local.

Exemplo de configuração:

```env
OPENAI_API_KEY=sua_chave_aqui
```

O arquivo `.env` não deve ser enviado ao GitHub.

O repositório deve conter apenas um arquivo `.env.example`, sem credenciais reais.

### Tratamento de falhas

Caso a API de IA Generativa esteja indisponível ou ocorra uma falha na comunicação com a OpenAI, o Node-RED apresenta uma mensagem de contingência.

Essa mensagem informa que a explicação generativa não foi produzida e orienta o cidadão a acompanhar os canais oficiais da Defesa Civil.

## 10. Banco de dados

O projeto utiliza MySQL para armazenar o histórico das medições.

Banco de dados:

```sql
CREATE DATABASE IF NOT EXISTS alertacidadao;
USE alertacidadao;
```

Tabela principal:

```sql
CREATE TABLE IF NOT EXISTS leituras_enchente (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ponto_monitoramento VARCHAR(50) NOT NULL,
    nivel_agua_cm DECIMAL(10,2) NOT NULL,
    chuva_mm DECIMAL(10,2) NOT NULL DEFAULT 0,
    elevacao_cm_h DECIMAL(10,2) NOT NULL DEFAULT 0,
    data_hora DATETIME NOT NULL,
    data_registro TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

O histórico permite acompanhar a evolução das condições monitoradas.

## 11. Dashboard

O dashboard é desenvolvido no Node-RED e disponibiliza informações para acompanhamento do monitoramento.

Entre os elementos previstos estão:

- Indicador do nível da água.
- Indicador da intensidade da chuva.
- Gráfico histórico das medições.
- Tabela de registros.
- Classificação produzida pelo Machine Learning.
- Mensagem explicativa da IA Generativa ou mensagem de contingência.

O dashboard pode ser acessado pelo endereço:

```text
http://localhost:1880/ui
```

## 12. Estrutura do projeto

A organização proposta para o repositório é:

```text
AlertaCidadao/
│
├── esp32/
│   ├── sketch.ino
│   ├── diagram.json
│   └── libraries.txt
│
├── node-red/
│   └── AlertaCidadao_NodeRED_Flow_Corrigido.json
│
├── sql/
│   └── alertacidadao.sql
│
├── API_ML/
│   ├── main.py
│   ├── requirements.txt
│   └── README.md
│
├── API_GenAI/
│   ├── main.py
│   ├── requirements.txt
│   ├── .env.example
│   └── .gitignore
│
├── docs/
│   ├── arquitetura/
│   ├── testes/
│   └── evidencias/
│
├── .gitignore
└── README.md
```

A estrutura deve ser conferida com os arquivos efetivamente utilizados no projeto antes da publicação final.

## 13. Como executar o projeto

### 13.1. Pré-requisitos

Instalar e configurar:

- Python 3.12.x.
- Node.js e Node-RED.
- MySQL.
- Wokwi ou ambiente compatível para simulação do ESP32.
- Broker MQTT acessível.
- Chave da API OpenAI para testar a geração de mensagens.

### 13.2. Configurar o banco de dados

1. Iniciar o MySQL.
2. Executar o arquivo `sql/alertacidadao.sql`.
3. Conferir a criação do banco `alertacidadao`.
4. Verificar a criação da tabela `leituras_enchente`.
5. Configurar a conexão MySQL no Node-RED.

### 13.3. Executar a API de Machine Learning

Abrir um terminal na pasta `API_ML`:

```powershell
cd API_ML
```

Criar o ambiente virtual:

```powershell
py -3.12 -m venv .venv
```

Ativar o ambiente:

```powershell
.\.venv\Scripts\Activate.ps1
```

Atualizar o pip:

```powershell
python -m pip install --upgrade pip
```

Instalar as dependências:

```powershell
python -m pip install --only-binary=:all: -r requirements.txt
```

Iniciar a API:

```powershell
python -m uvicorn main:app --host 0.0.0.0 --port 8000 --reload
```

Documentação:

```text
http://127.0.0.1:8000/docs
```

Health check:

```text
http://127.0.0.1:8000/health
```

### 13.4. Executar a API de IA Generativa

Abrir outro terminal na pasta `API_GenAI`:

```powershell
cd API_GenAI
```

Criar e ativar o ambiente virtual:

```powershell
py -3.12 -m venv .venv
.\.venv\Scripts\Activate.ps1
```

Instalar as dependências:

```powershell
python -m pip install --upgrade pip
python -m pip install --only-binary=:all: -r requirements.txt
```

Criar o arquivo `.env` com a chave válida da OpenAI, conforme o `.env.example`.

Iniciar a API:

```powershell
python -m uvicorn main:app --host 0.0.0.0 --port 8001 --reload
```

Documentação:

```text
http://127.0.0.1:8001/docs
```

Health check:

```text
http://127.0.0.1:8001/health
```

### 13.5. Importar o fluxo no Node-RED

1. Iniciar o Node-RED.
2. Acessar `http://localhost:1880`.
3. Abrir o menu de importação.
4. Selecionar o arquivo `AlertaCidadao_NodeRED_Flow_Corrigido.json`.
5. Importar o fluxo.
6. Instalar os nós adicionais necessários:
   - `node-red-node-mysql`
   - `node-red-dashboard`
   - `node-red-node-ui-table`
7. Configurar a conexão com o MySQL.
8. Conferir o endereço do broker MQTT.
9. Conferir os endereços das APIs nas requisições HTTP.
10. Clicar em Deploy.

### 13.6. Executar a simulação ESP32

1. Abrir o projeto no Wokwi.
2. Conferir os arquivos `sketch.ino` e `diagram.json`.
3. Verificar as conexões dos componentes.
4. Iniciar a simulação.
5. Conferir o envio das mensagens MQTT.
6. Acompanhar os registros no Node-RED e no dashboard.

## 14. Testes e validação

Os testes do projeto são organizados para verificar o funcionamento individual dos componentes e a integração entre eles.

### Testes previstos

| Componente | Cenário |
|---|---|
| ESP32 | Envio de telemetria |
| MQTT | Recebimento das mensagens pelo Node-RED |
| Node-RED | Validação e processamento dos dados |
| MySQL | Armazenamento das medições |
| Machine Learning | Classificação das quatro categorias |
| IA Generativa | Geração de explicações a partir do resultado do ML |
| Dashboard | Exibição das medições e mensagens |
| Contingência | Tratamento da indisponibilidade de serviços |

### Métricas de Machine Learning

Para avaliar o modelo, devem ser utilizadas:

- Accuracy.
- Precision.
- Recall.
- F1-score.
- Matriz de confusão.

Os resultados quantitativos devem ser registrados após a execução dos testes com uma base de validação adequada.

### Resultados e situação atual

Durante o desenvolvimento, foram confirmados:

- Comunicação entre o ESP32 simulado e o broker MQTT.
- Funcionamento do fluxo importado no Node-RED.
- Exibição da mensagem de contingência no dashboard.
- Inicialização da API de Machine Learning.
- Inicialização da API de IA Generativa.
- Resposta saudável dos endpoints de verificação dos serviços.
- Correção do envio do corpo JSON em UTF-8 nos testes da API de IA Generativa.

**Pendências de validação:**

- Execução e registro dos testes para as quatro classes de risco.
- Cálculo das métricas quantitativas do modelo.
- Validação com dados históricos reais.
- Confirmação da geração efetiva de mensagens pela OpenAI após regularização da cota ou faturamento.
- Registro das evidências finais de integração.

## 15. Limitações e melhorias futuras

### Limitações atuais

- O modelo de Machine Learning utiliza dados sintéticos.
- Os sensores estão sendo utilizados em simulação.
- A intensidade da chuva é representada por um potenciômetro.
- A comunicação MQTT utiliza um broker público.
- A geração de mensagens depende da disponibilidade da API externa da OpenAI.
- O protótipo não possui validação operacional em áreas reais de enchente.
- Os resultados do modelo ainda precisam ser avaliados com dados históricos reais.

### Melhorias futuras

- Integrar sensores físicos apropriados para ambientes externos.
- Utilizar bases históricas reais de enchentes e alagamentos.
- Treinar e validar novamente o modelo com dados representativos.
- Implementar autenticação e comunicação segura entre os componentes.
- Adicionar novos pontos de monitoramento.
- Melhorar a disponibilidade e a persistência dos serviços.
- Integrar fontes oficiais de informações meteorológicas e alertas públicos.
- Realizar testes em ambiente controlado com especialistas e órgãos competentes.

## 16. Segurança e responsabilidade

- Não publicar chaves de API, senhas ou credenciais no GitHub.
- Utilizar variáveis de ambiente para informações sensíveis.
- Não considerar as classificações do protótipo como alertas oficiais.
- Não utilizar o modelo atual como única fonte para decisões de evacuação.
- Orientar os cidadãos a acompanhar os canais oficiais da Defesa Civil.
- Em uma implantação real, adotar autenticação, criptografia e controles de acesso adequados.

## 17. Repositório

**GitHub:** https://github.com/Group-GS/GS-IoT

## 18. Integrantes

| Integrante | RM |
|---|---|
| Lucas Rafael Solimene | 565194 |
| Samyr Couto Oliveira | 565562 |
| Henrique Teixeira Cesar | 563088 |