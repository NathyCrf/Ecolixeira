import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression

print("🗑️ Inicializando IA Preditiva da ecoLixeira...\n")

# 1. Dados simulados: Histórico de leituras do HC-SR04
# Horas de uso vs Distância medida pelo sensor (cm)
# Lixeira vazia = 40cm | Lixeira cheia = 5cm
dados = {
    'horas_uso': [0, 5, 10, 15, 20, 25, 30],
    'distancia_cm': [40, 36, 31, 24, 20, 14, 10]
}
df = pd.DataFrame(dados)

# 2. Separar as variáveis
X = df[['horas_uso']]  # Variável independente (Tempo)
y = df['distancia_cm'] # Variável dependente (Distância do lixo)

# 3. Treinar o modelo de Machine Learning
modelo = LinearRegression()
modelo.fit(X, y)

# 4. Fazer a previsão matemática
# Se a equação da reta é y = (coeficiente * x) + intercepto, 
# isolamos o 'x' (horas) para descobrir quando 'y' (distância) será 5cm.
distancia_alvo = 5
hora_limite = (distancia_alvo - modelo.intercept_) / modelo.coef_[0]

print(f"📊 Dados analisados: {len(df)} leituras do sensor ultrassônico.")
print(f"⚠️ Alerta: O modelo prevê que a lixeira atingirá sua capacidade máxima (5cm) em {hora_limite:.1f} horas de uso.")
