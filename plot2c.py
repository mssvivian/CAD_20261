import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress

# 1. Os valores de n que o professor pediu
n_values = np.array([64, 256, 1024, 4096, 16384])

# O intervalo é de 0 a pi, então h = (pi - 0) / n
h_values = np.pi / n_values

# ---------------------------------------------------------
# ATENÇÃO: Substitua os 5 valores abaixo pelas integrais
# que o seu programa MPI imprimiu no terminal para cada 'n'!
# ---------------------------------------------------------
I_aprox = np.array([
    1.999598388640037,  # Cole o resultado para n = 64
    1.999974900235053,  # Cole o resultado para n = 256
    1.999998431268383,  # Cole o resultado para n = 1024
    1.999999901954288,  # Cole o resultado para n = 4096
    1.999999993872146   # Cole o resultado para n = 16384
])

# 2. Calcula o Erro Absoluto
exact = 2.0
errors = np.abs(I_aprox - exact)

# 3. Calcula a inclinação (slope) fazendo uma regressão linear nos logaritmos
# log10(Erro) vs log10(h)
log_h = np.log10(h_values)
log_err = np.log10(errors)
slope, intercept, r_value, p_value, std_err = linregress(log_h, log_err)

# 4. Plota o Gráfico Log-Log
plt.figure(figsize=(8, 6))
plt.loglog(h_values, errors, marker='o', linestyle='', color='red', label='Erro Empírico')

# Plota a reta de tendência da regressão
plt.loglog(h_values, 10**(intercept + slope * log_h), linestyle='--', color='blue', 
           label=f'Reta de Ajuste (Inclinação = {slope:.3f})')

plt.title('Convergência da Regra do Trapézio (Escala Log-Log)', fontsize=14)
plt.xlabel('Tamanho do passo (h)', fontsize=12)
plt.ylabel('Erro Absoluto |I_aprox - 2|', fontsize=12)
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend(fontsize=12)

# Salva a imagem para você colocar no seu PDF
plt.savefig('grafico_convergencia.png', dpi=300, bbox_inches='tight')
print(f"Gráfico salvo como 'grafico_convergencia.png'!")
print(f"A inclinação (slope) medida foi: {slope:.4f}")