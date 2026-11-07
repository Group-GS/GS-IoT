from datetime import datetime
from typing import List, Optional

import numpy as np
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, Field
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score

app = FastAPI(
    title="AlertaCidadao - API de Machine Learning",
    description=(
        "API demonstrativa para classificar risco de enchente "
        "a partir de medições ambientais."
    ),
    version="1.0.0",
)

CLASSES = ["NORMAL", "ATENCAO", "ALERTA", "EMERGENCIA"]
FEATURES = [
    "nivel_agua_cm",
    "chuva_mm",
    "elevacao_cm_h",
]

# IMPORTANTE:
# Estes dados são sintéticos e servem apenas para validar a integração.
# Substitua-os por dados históricos reais e rotulados antes de qualquer
# uso operacional. As faixas abaixo são regras didáticas, não limites oficiais.
def gerar_dados_sinteticos(n=6000, seed=42):
    rng = np.random.default_rng(seed)

    nivel = rng.uniform(0, 200, n)
    chuva = rng.uniform(0, 100, n)
    elevacao = rng.uniform(-20, 40, n)

    # Rótulos artificiais para demonstração da API e do pipeline.
    # Não representam critérios oficiais de Defesa Civil.
    score = (
        (nivel / 200.0) * 0.45
        + (chuva / 100.0) * 0.20
        + (np.maximum(elevacao, 0) / 40.0) * 0.35
    )

    labels = np.select(
        [
            score < 0.22,
            score < 0.42,
            score < 0.65,
        ],
        [
            "NORMAL",
            "ATENCAO",
            "ALERTA",
        ],
        default="EMERGENCIA",
    )

    X = np.column_stack([nivel, chuva, elevacao])
    return X, labels


X, y = gerar_dados_sinteticos()
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

modelo = RandomForestClassifier(
    n_estimators=150,
    max_depth=10,
    min_samples_leaf=2,
    random_state=42,
    class_weight="balanced",
)
modelo.fit(X_train, y_train)

y_pred = modelo.predict(X_test)
METRICAS = {
    "accuracy_teste_sintetico": round(float(accuracy_score(y_test, y_pred)), 4),
    "relatorio_teste_sintetico": classification_report(
        y_test, y_pred, output_dict=True, zero_division=0
    ),
    "tipo_dados": "sinteticos - demonstracao",
}


class Medicao(BaseModel):
    ponto_monitoramento: str = Field(min_length=1, max_length=50)
    nivel_agua_cm: float = Field(ge=0, le=1000)
    chuva_mm: float = Field(default=0, ge=0, le=1000)
    elevacao_cm_h: float = Field(default=0, ge=-1000, le=1000)
    data_hora: Optional[datetime] = None


class Predicao(BaseModel):
    ponto_monitoramento: str
    nivel_agua_cm: float
    chuva_mm: float
    elevacao_cm_h: float
    classificacao: str
    probabilidade: float
    fatores: List[str]
    modelo: str
    observacao: str


@app.get("/")
def raiz():
    return {
        "servico": "AlertaCidadao ML API",
        "status": "online",
        "endpoint_predicao": "/predict",
        "documentacao": "/docs",
        "tipo_modelo": "RandomForestClassifier",
        "dados_treinamento": "sinteticos - demonstracao",
    }


@app.get("/health")
def health():
    return {"status": "healthy", "modelo_carregado": modelo is not None}


@app.get("/metrics")
def metrics():
    return METRICAS


@app.post("/predict", response_model=Predicao)
def predict(medicao: Medicao):
    valores = np.array([[
        medicao.nivel_agua_cm,
        medicao.chuva_mm,
        medicao.elevacao_cm_h,
    ]])

    classificacao = str(modelo.predict(valores)[0])
    probabilidades = modelo.predict_proba(valores)[0]
    indice = list(modelo.classes_).index(classificacao)
    probabilidade = float(probabilidades[indice])

    fatores = []
    if medicao.nivel_agua_cm >= 100:
        fatores.append("nivel de agua elevado na escala demonstrativa")
    if medicao.chuva_mm >= 50:
        fatores.append("chuva simulada elevada")
    if medicao.elevacao_cm_h >= 10:
        fatores.append("elevacao rapida da agua")
    if not fatores:
        fatores.append("nenhum fator isolado atingiu as faixas demonstrativas")

    return Predicao(
        ponto_monitoramento=medicao.ponto_monitoramento,
        nivel_agua_cm=medicao.nivel_agua_cm,
        chuva_mm=medicao.chuva_mm,
        elevacao_cm_h=medicao.elevacao_cm_h,
        classificacao=classificacao,
        probabilidade=round(probabilidade, 4),
        fatores=fatores,
        modelo="RandomForestClassifier",
        observacao=(
            "Classificacao experimental baseada em dados sinteticos. "
            "Nao usar como alerta oficial ou decisao de evacuacao."
        ),
    )
