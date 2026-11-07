CREATE DATABASE IF NOT EXISTS alertacidadao;

USE alertacidadao;

CREATE TABLE IF NOT EXISTS leituras_enchente (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ponto_monitoramento VARCHAR(50) NOT NULL,
    nivel_agua_cm DECIMAL(10,2) NOT NULL,
    chuva_mm DECIMAL(10,2) NOT NULL DEFAULT 0,
    elevacao_cm_h DECIMAL(10,2) NOT NULL DEFAULT 0,
    data_hora DATETIME NOT NULL,
    data_registro TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);