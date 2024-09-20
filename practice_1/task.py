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

# Преобразование Фурье
Y = np.fft.fft(y)
N = len(Y) # Количество точек
Y_magnitude = np.abs(Y) / N
frequencies = np.fft.fftfreq(N, d=0.001)

plt.figure()
plt.subplot(2,1,1)
plt.plot(t, y)
plt.title('Непрерывный сигнал y(t) = 3sin(2πft + π/7)')
plt.xlabel('Время (с)')
plt.ylabel('Амплитуда')
plt.grid()
plt.axis('tight')
plt.show()

# Визуализация спектра
plt.figure()
plt.subplot(2,1,2)
plt.plot(frequencies[:N // 2], Y_magnitude[:N // 2]) 
plt.title('Спектр сигнала')
plt.xlabel('Частота (Гц)')
plt.ylabel('Амплитуда')
plt.grid()
plt.xlim(0, 80)
plt.show()
