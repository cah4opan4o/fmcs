import matplotlib.pyplot as plt

# ДОДЕЛАЙ отрисовку для FLOAT (double) значений

# Список файлов и их соответствующие заголовки для графиков
files_and_titles = [
    ("name_bits.txt", "График битовой последовательности"),
    ("gold_name_crc_bits.txt", "График gold + data + crc последовательности"),
    ("N_gold_name_crc_bits.txt", "График N * (gold + data + crc) последовательности")
]

float_files_and_titles = [
    ("noise_bits.txt", "График noise последовательности"),
    ("noise_data_bits.txt","График noise + data")
]

# Цикл по файлам
for filename, title in files_and_titles:
    try:
        # Чтение данных из файла
        with open(filename, "r") as file:
            bit_data = file.read().strip()  # Считываем содержимое и удаляем лишние пробелы/переносы строк
    except FileNotFoundError:
        print(f"Файл '{filename}' не найден.")
        continue  # Переход к следующему файлу

    # Преобразование битовой строки в числовые значения
    bit_values = [int(bit) for bit in bit_data]

    # Построение графика
    plt.figure(figsize=(10, 4))
    plt.step(range(len(bit_values)), bit_values, where="post", label="Битовые данные")
    plt.title(title)
    plt.xlabel("Индекс бита")
    plt.ylabel("Значение бита (0 или 1)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

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
    plt.plot(range(len(float_values)), float_values, label="Float данные")
    plt.title(title)
    plt.xlabel("Индекс")
    plt.ylabel("Значение (float)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()


# Показ всех графиков
plt.show()
