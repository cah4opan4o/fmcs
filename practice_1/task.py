import numpy as np
import matplotlib.pyplot as plt

# Параметры сигнала
f = 40           # Частота в Гц
A = 3          # Амплитуда
phi = np.pi / 7 # Фаза

# Временные параметры
t = np.arange(0, 1, 0.001)

# Параметры дискретизации
fs = 80
t_duration = 1
N = int(fs * t_duration) 

# Генерация сигнала
y = A * np.sin(2 * np.pi * f * t + phi)

# Преобразование Фурье
Y = np.fft.fft(y)
N = len(Y) # Количество точек
Y_magnitude = np.abs(Y) / N
frequencies = np.fft.fftfreq(N, d=0.001)

# Генерация временных точек с частотой дискретизации 80 Гц
t_samples = np.linspace(0, t_duration, N, endpoint=False)  
y_samples = A * np.sin(2 * np.pi * f * t_samples + phi)

plt.figure("task 1 and 2")
plt.subplot(3,1,1)
plt.plot(t, y)
plt.title('Непрерывный сигнал y(t) = 3sin(2πft + π/7)')
plt.xlabel('Время (с)')
plt.ylabel('Амплитуда')
plt.grid()
plt.axis('tight')

# Визуализация спектра
plt.subplot(3,1,3)
plt.plot(frequencies[:N // 2], Y_magnitude[:N // 2]) 
plt.title('Спектр сигнала')
plt.xlabel('Частота (Гц)')
plt.ylabel('Амплитуда')
plt.grid()
plt.xlim(0, 80)

print("Временные точки (с):", t_samples)
print("Оцифрованные значения сигнала:", y_samples)
plt.figure("task 4",figsize=(10, 5))
plt.plot(t_samples, y_samples, marker='o', linestyle='-')
plt.title('Оцифрованный сигнал (40 Гц)')
plt.xlabel('Время (с)')
plt.ylabel('Амплитуда')
plt.grid()
plt.show()