import numpy as np
import matplotlib.pyplot as plt

# Параметры сигнала
f = 40           # Частота в Гц
A = 3          # Амплитуда
phi = np.pi / 7 # Фаза

# Временные параметры
t = np.arange(0, 1, 0.001)

# Генерация сигнала
y = A * np.sin(2 * np.pi * f * t + phi)

plt.figure()
plt.plot(t, y)
plt.title('Непрерывный сигнал y(t) = 3sin(2πft + π/7)')
plt.xlabel('Время (с)')
plt.ylabel('Амплитуда')
plt.grid()
plt.axis('tight')
plt.show()
