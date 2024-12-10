import matplotlib.pyplot as plt
import numpy as np
# ДОДЕЛАЙ отрисовку для FLOAT (double) значений

# Список файлов и их соответствующие заголовки для графиков
float_files_and_titles = [
    ("probability_error.txt","Вероятность ошибки")
]

# Цикл по файлам с float данными
for filename, title in float_files_and_titles:
    try:
        # Чтение данных из файла
        with open(filename, "r") as file:
            raw_data = file.read().strip()  # Считываем содержимое и удаляем лишние пробелы/переносы строк
    except FileNotFoundError:
        print(f"Файл '{filename}' не найден.")
        continue  # Переход к следующему файлу

    try:
        # Преобразование строки в список float значений
        float_values = [float(value) for value in raw_data.split()]
    except ValueError:
        print(f"Данные в файле '{filename}' не являются корректными float значениями.")
        continue  # Переход к следующему файлу

    # Построение графика
    plt.figure(figsize=(10, 4))
    plt.plot(np.arange(0.0,1.05,0.05), float_values, label="Float данные")
    
    plt.xticks(np.arange(0.0, 1.05, 0.05))

    plt.title(title)
    plt.xlabel("sigma")
    plt.ylabel("P")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()


# Показ всех графиков
plt.show()