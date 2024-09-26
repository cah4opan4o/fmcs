import numpy as np
import matplotlib.pyplot as plt

# Входные данные
P_tx_BS = 46  # мощность передатчика BS в дБм
P_tx_UE = 24  # мощность передатчика UE в дБм
G_tx_BS = 21  # коэффициент усиления антенны BS в дБи
G_rx_BS = 21  # коэффициент усиления антенны BS в дБи (для восходящего канала)
G_rx_UE = 0   # коэффициент усиления антенны UE, можно считать как 0 дБи
L_margin_penetration = 15  # запас на проникновение сквозь стены в дБ
L_margin_interference = 1   # запас на интерференцию в дБ
frequency_UL = 10e6  # полоса частот для UL в Гц
frequency_DL = 20e6  # полоса частот для DL в Гц
SINR_UL = 4  # требуемое SINR для UL в дБ
SINR_DL = 2  # требуемое SINR для DL в дБ
Nf_BS = 2.4  # коэффициент шума приемника BS в дБ
Nf_UE = 6    # коэффициент шума приемника UE в дБ
B = 290  # температура системы в К
h_BS = 30  # высота антенны базовой станции (в м)
h_UE = 1.5  # высота антенны пользователя (в м)
C_m = 3  # поправка для городской среды

# Константы
k = 1.38e-23  # постоянная Больцмана

# Функция для расчета уровня шума (в дБ)
def calculate_noise(Bandwidth, Nf):
    # Уровень шума: 10 * log10(k * T * B) + F
    Noise_power = 10 * np.log10(k * B * Bandwidth) + Nf
    return Noise_power

# Запас мощности сигнала
L_margin = L_margin_penetration + L_margin_interference

# 1. MAPL для UL
Noise_UL = calculate_noise(frequency_UL, Nf_BS)
MAPL_UL = P_tx_UE + G_rx_BS - (Noise_UL + SINR_UL + L_margin)

# 2. MAPL для DL
Noise_DL = calculate_noise(frequency_DL, Nf_UE)
MAPL_DL = P_tx_BS + G_tx_BS - (Noise_DL + SINR_DL + L_margin)

# Функция для модели COST 231 Hata
def cost231_hata(f, d, h_BS, h_UE, C_m):
    a_h_UE = (1.1 * np.log10(f) - 0.7) * h_UE - (1.56 * np.log10(f) - 0.8)
    L = 46.3 + 33.9 * np.log10(f) - 13.82 * np.log10(h_BS) - a_h_UE + \
        (44.9 - 6.55 * np.log10(h_BS)) * np.log10(d) + C_m
    return L

# Функция для модели UMiNLOS
def uminlos(f, d):
    L = 32.4 + 20 * np.log10(d) + 20 * np.log10(f)
    return L

# 3. Зависимость потерь от расстояния для обеих моделей
distances = np.linspace(0.1, 10, 100)  # расстояние от 0.1 км до 10 км
frequency_MHz = 1800  # частота в МГц

# Потери для модели COST 231 Hata
loss_cost231 = cost231_hata(frequency_MHz, distances, h_BS, h_UE, C_m)

# Потери для модели UMiNLOS
loss_uminlos = uminlos(frequency_MHz/1000, distances)  # переводим частоту в ГГц

# Построение графика зависимости потерь от расстояния
plt.figure(figsize=(10, 6))
plt.plot(distances, loss_cost231, label="COST 231 Hata")
plt.plot(distances, loss_uminlos, label="UMiNLOS", linestyle="--")
plt.xlabel("Расстояние (км)")
plt.ylabel("Потери (дБ)")
plt.title("Зависимость потерь сигнала от расстояния")
plt.legend()
plt.grid(True)
plt.show()

# 4. Радиус базовой станции

# Функция для нахождения радиуса по модели потерь
def find_radius(MAPL, model_func, f, h_BS, h_UE, C_m):
    for d in np.linspace(0.1, 10, 10000):
        if model_func(f, d, h_BS, h_UE, C_m) > MAPL:
            return d
    return None

# Радиус базовой станции в UL и DL
radius_UL = find_radius(MAPL_UL, cost231_hata, frequency_MHz, h_BS, h_UE, C_m)
radius_DL = find_radius(MAPL_DL, cost231_hata, frequency_MHz, h_BS, h_UE, C_m)

radius_station = min(radius_UL, radius_DL)

# 5. Площадь одной базовой станции и требуемое количество станций
area_station = np.pi * radius_station**2  # площадь одной базовой станции
total_area = 100  # заданная площадь в кв.км
num_stations = total_area / area_station  # количество базовых станций

# Вывод результатов
print(f"Радиус базовой станции (км): {radius_station:.2f}")
print(f"Площадь одной базовой станции (кв.км): {area_station:.2f}")
print(f"Требуемое количество базовых станций: {np.ceil(num_stations)}")
