# AlertaCidadao - API de Machine Learning

API demonstrativa para integrar o Node-RED ao modelo de classificacao de risco.

## Requisitos
- Python 3.11 ou 3.12
- pip

## Instalar no Windows (PowerShell)

```powershell
cd caminho\para\AlertaCidadao_ML_API
py -3.12 -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
pip install -r requirements.txt
```

Se `py -3.12` não estiver disponível, use `python -m venv .venv`.

## Iniciar a API

```powershell
uvicorn main:app --host 0.0.0.0 --port 8000 --reload
```

O endereço `0.0.0.0` permite que o serviço escute nas interfaces da máquina.
No Node-RED executado na mesma máquina, use `http://127.0.0.1:8000/predict`.

## Testar
- Saúde: http://127.0.0.1:8000/health
- Documentação interativa: http://127.0.0.1:8000/docs
- Métricas demonstrativas: http://127.0.0.1:8000/metrics

Exemplo de POST para `/predict`:

```json
{
  "ponto_monitoramento": "alertacidadao_01",
  "nivel_agua_cm": 85.5,
  "chuva_mm": 12.4,
  "elevacao_cm_h": 3.2,
  "data_hora": "2026-11-06T08:00:00Z"
}
```

## Atenção sobre os dados
O modelo é treinado na inicialização com dados sintéticos gerados no próprio código.
As faixas são didáticas e não representam limites oficiais ou uma avaliação real
de risco de enchente. As métricas em `/metrics` medem somente o desempenho sobre
um conjunto de teste sintético derivado do mesmo gerador.

Antes de qualquer uso operacional:
1. Obter dados históricos confiáveis de sensores e ocorrências.
2. Definir rótulos com apoio de especialistas/órgãos competentes.
3. Treinar e avaliar com separação temporal e dados representativos.
4. Medir precision, recall e F1 por classe, especialmente ALERTA e EMERGENCIA.
5. Validar limites, comportamento em dados ausentes e mudanças de sensor.

## Contrato de integração Node-RED
Entrada: JSON com `ponto_monitoramento`, `nivel_agua_cm`, `chuva_mm`,
`elevacao_cm_h` e opcionalmente `data_hora`.

Saída: JSON com `classificacao`, `probabilidade`, `fatores`, dados de entrada,
nome do modelo e observação.
