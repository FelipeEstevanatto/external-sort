import matplotlib.pyplot as plt
import numpy as np

# Dados
tasks = ['T1', 'T2', 'T3', 'T4']
windows_normal = [0.017, 0.1156666667, 1.368, 25.76033333]
windows_ofast = [0.017, 0.11066666666666666666666666666667, 1.208, 24.76033333]
linux_normal = [0.003098, 0.03236433333, 0.4745103333, 20.379703]
linux_ofast = [0.0029808, 0.0301248, 0.3774798, 11.8613798]

x = np.arange(len(tasks))  # Localização dos rótulos
width = 0.2  # Largura das barras

# Gráfico para Windows
fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, windows_normal, width, label='Windows Normal')
rects2 = ax.bar(x + width/2, windows_ofast, width, label='Windows Ofast')

# Adicionar rótulos, título e legenda
ax.set_xlabel('Tarefas')
ax.set_ylabel('Tempo (segundos)')
ax.set_yscale('log')
ax.set_title('Tempos de execução no Windows')
ax.set_xticks(x)
ax.set_xticklabels(tasks)
ax.legend()

# Mostrar gráfico
plt.show()

# Gráfico para Linux
fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, linux_normal, width, label='Linux Normal')
rects2 = ax.bar(x + width/2, linux_ofast, width, label='Linux Ofast')

# Adicionar rótulos, título e legenda
ax.set_xlabel('Tarefas')
ax.set_ylabel('Tempo (segundos)')
ax.set_yscale('log')
ax.set_title('Tempos de execução no Linux')
ax.set_xticks(x)
ax.set_xticklabels(tasks)
ax.legend()

# Mostrar gráfico
plt.show()