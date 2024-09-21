import numpy as np
import matplotlib.pyplot as plt

f = 40  # Частота в Гц
A = 3   # Амплитуда
phi = np.pi / 7  # Фаза

fs = 80  # Частота дискретизации в Гц
t_duration = 1  # Длительность в секундах
N = int(fs * t_duration)  # Количество отсчетов

# Генерация временных выборок
t_samples = np.linspace(0, t_duration, N, endpoint=False)
# Генерация выборок сигнала
y_samples = A * np.sin(2 * np.pi * f * t_samples + phi)

# Выполнение БПФ
Y = np.fft.fft(y_samples)
# Вычисляем частоту сэмплирования
frequencies = np.fft.fftfreq(N, d=1/fs)

max_frequency = np.max(np.abs(Y))
Frequency_bin_width = frequencies[1] - frequencies[0]

# Используем тип float64 по умолчанию для numpy
memory_usage = y_samples.nbytes  # это в байтах
memory_usage_kb = memory_usage / 1024  # переводим в КБ

# Восстановление оригинального сигнала с помощью интерполяции
t_fine = np.linspace(0, t_duration, 1000)  
y_fine = A * np.sin(2 * np.pi * f * t_fine + phi)  

# Вывод информации в консоль
print("Дискретное преобразование Фурье (амплитуда):\n", np.abs(Y))
print("Ширина спектра (максимальная частота):", max_frequency)
print("Объем памяти, занятый массивом y_samples:", memory_usage_kb, "КБ")

# Сохранение данных в файл
with open('signal_data.txt', 'w') as f:
    for x, y in zip(t_samples, y_samples):
        f.write(f"{x:.6f} {y:.6f}\n")

# Визуализация
plt.figure(figsize=(12, 6))
plt.plot(t_fine, y_fine, label='Оригинальный сигнал', color='blue')
plt.plot(t_samples, y_samples, 'o-', label='Восстановленный сигнал (отсчеты)', color='orange')
plt.title('Сравнение оригинального и восстановленного сигнала')
plt.xlabel('Время (с)')
plt.ylabel('Амплитуда')
plt.legend()
plt.grid()
plt.xlim(0, t_duration)
plt.ylim(-5, 5)
plt.show()