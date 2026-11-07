# AlertaCidadão — API de IA Generativa

API FastAPI que recebe a classificação calculada pelo serviço de Machine Learning e usa a OpenAI API para redigir uma explicação preventiva.

## Responsabilidades
- ML (`:8000`): classifica o risco.
- GenAI (`:8001`): explica a classificação recebida.
- Node-RED: integra os serviços e exibe a mensagem.

A IA generativa não recalcula o risco nem substitui comunicados oficiais da Defesa Civil. Projeto acadêmico/demonstrativo.

## Requisitos
Python 3.12 e uma chave OpenAI API com acesso e faturamento/créditos habilitados. A API é cobrada separadamente do ChatGPT.

## Instalação (PowerShell)
```powershell
py -3.12 -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
python -m pip install --only-binary=:all: -r requirements.txt
Copy-Item .env.example .env
notepad .env
```
No `.env`, substitua `sua_chave_openai_aqui` pela sua chave. Nunca compartilhe a chave nem a envie ao GitHub.

## Iniciar
```powershell
python -m uvicorn main:app --host 0.0.0.0 --port 8001 --reload
```
- Health: http://127.0.0.1:8001/health
- Swagger: http://127.0.0.1:8001/docs

## Teste (outro PowerShell)
```powershell
$body = @{
  instrucoes = "Explique o resultado para o cidadão."
  dados = @{
    classificacao = "ATENCAO"
    probabilidade = 0.82
    fatores = @("nível da água elevado")
    medicoes = @{
      ponto_monitoramento = "alertacidadao_01"
      nivel_agua_cm = 85.5
      chuva_mm = 12.4
      elevacao_cm_h = 3.2
      data_hora = "2026-11-06T08:00:00Z"
    }
  }
} | ConvertTo-Json -Depth 6

Invoke-RestMethod -Uri "http://127.0.0.1:8001/gerar-alerta" -Method Post -ContentType "application/json" -Body $body
```

## Node-RED
Endpoint: `POST http://127.0.0.1:8001/gerar-alerta`. O corpo segue o exemplo acima e a resposta contém `alerta`, compatível com o fluxo Node-RED existente.

## Segurança e limitações
- `.env` está no `.gitignore`; nunca coloque a chave no código, Node-RED, Wokwi, frontend ou repositório.
- Chamadas à OpenAI são feitas somente pelo backend.
- Confira preços e limites atuais antes dos testes.
- Não usar para alertas oficiais, evacuação ou decisões de emergência.
