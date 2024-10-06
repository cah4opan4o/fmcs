import numpy as np
import time
import numpy.fft as fft

# Ваша функция для вычисления ДПФ (дискретное преобразование Фурье)
def discrete_furie_tr(N, samples, x):
    summ = 0.0
    for n in range(N):
        summ += samples[n] * np.exp(-2j * np.pi * (x * (n / N)))
    return summ

# Функция для сравнения времени работы
def compare_performance(N):
    # Генерация случайных выборок (samples)
    samples = np.random.rand(N)

    # Измерение времени работы вашей функции
    start_time = time.time()
    result_custom = [discrete_furie_tr(N, samples, x) for x in range(N)]
    custom_time = time.time() - start_time

    # Измерение времени работы встроенной функции БПФ
    start_time = time.time()
    result_fft = fft.fft(samples)
    fft_time = time.time() - start_time

    # Вывод времени работы
    print(f"Время работы вашей функции ДПФ: {custom_time:.6f} секунд")
    print(f"Время работы встроенной функции БПФ: {fft_time:.6f} секунд")

    # Возвращаем результаты для сравнения (если нужно)
    return result_custom, result_fft

# Пример вызова функции для сравнения с N=1024
compare_performance(4096)
