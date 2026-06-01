CREATE DATABASE iot;

USE iot;

CREATE TABLE sensores (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperatura FLOAT,
    umidade FLOAT,
    pressao FLOAT,
    data_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);