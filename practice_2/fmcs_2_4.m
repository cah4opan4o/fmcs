% Входные данные
P_tx_UE = 24;      % Мощность передатчика UE в дБм (UL)
P_tx_BS = 46;      % Мощность передатчика BS в дБм (DL)
G_tx_UE = 0;       % Усиление антенны UE в дБи (UL)
G_tx_BS = 21;      % Усиление антенны BS в дБи (DL и UL)
G_rx_UE = 0;       % Усиление антенны UE в дБи (DL)
G_rx_BS = 21;      % Усиление антенны BS в дБи (UL)
L_pen = 15;        % Потери на проникновение через стены в дБ
L_int = 1;         % Потери на интерференцию в дБ
N_f_BS = 2.4;      % Коэффициент шума приемника BS в дБ
N_f_UE = 6;        % Коэффициент шума приемника UE в дБ
SINR_UL = 4;       % Требуемое SINR для UL в дБ
SINR_DL = 2;       % Требуемое SINR для DL в дБ
BW_UL = 10e6;      % Полоса частот в UL (10 МГц)
BW_DL = 20e6;      % Полоса частот в DL (20 МГц)
PL_constant = 46.3;  % Константа для модели распространения сигнала (COST231)
B = 33.9;          % Коэффициент модели распространения сигнала

% Заданная площадь покрытия
S_total_macro = 100;     % Площадь покрытия макросот (100 км^2)
S_micro_femto = 4;       % Площадь покрытия для микро- и фемтосот (4 км^2)

% Чувствительность приемника BS (UL)
P_rx_BS = -174 + 10 * log10(BW_UL) + N_f_BS + SINR_UL;

% Чувствительность приемника UE (DL)
P_rx_UE = -174 + 10 * log10(BW_DL) + N_f_UE + SINR_DL;

% Расчет MAPL для UL
MAPL_UL = P_tx_UE + G_tx_UE + G_rx_BS - L_pen - L_int - P_rx_BS;

% Расчет MAPL для DL
MAPL_DL = P_tx_BS + G_tx_BS + G_rx_UE - L_pen - L_int - P_rx_UE;

% Вывод результатов
fprintf('MAPL для восходящего канала (UL): %.2f дБ\n', MAPL_UL);
fprintf('MAPL для нисходящего канала (DL): %.2f дБ\n', MAPL_DL);

% Определение радиуса базовой станции (используем модель COST231 для макросот)
f = 1800;  % Частота в МГц
A_COST231 = PL_constant - 13.82 * log10(hBS);  % Поправка для высоты BS
d_UL = 10^((MAPL_UL - A_COST231 - B * log10(f)) / B);  % Радиус для UL
d_DL = 10^((MAPL_DL - A_COST231 - B * log10(f)) / B);  % Радиус для DL

% Выбор минимального радиуса
d_min = min(d_UL, d_DL);  % Минимальный радиус в км

% Расчет площади S = 1.95R^2
S_one_macro = 1.95 * d_min^2;  % Площадь одной соты (км^2)

% Расчет количества базовых станций для макросот
num_sites_macro = ceil(S_total_macro / S_one_macro);  % Количество базовых станций для макросот

% ---------------- Расчет для микро- и фемтосот с моделью UMiNLOS ----------------
L_UMiNLOS = @(d) 22.7 + 36.7 * log10(d) + 26 * log10(f);  % Потери по модели UMi NLOS

% Радиус для UMi NLOS
% MAPL для микро- и фемтосот (берем минимальное из UL и DL для безопасности):
MAPL_micro_femto = min(MAPL_UL, MAPL_DL);

% Найдем радиус по UMi NLOS: L_UMiNLOS(d) = MAPL
d_micro_femto = 10^((MAPL_micro_femto - 22.7 - 20 * log10(f)) / 20);  % Радиус в км

% Площадь покрытия одной соты для микро- или фемтосоты
S_one_micro_femto = 1.95 * d_micro_femto^2;  % Площадь одной соты для микро-/фемтосоты (км^2)

% Расчет количества микро- и фемтосот
num_sites_micro_femto = ceil(S_micro_femto / S_one_micro_femto);  % Количество микро- и фемтосот

% ------------------- Вывод результатов -------------------
fprintf('Радиус базовой станции для восходящего канала (UL): %.2f км\n', d_UL);
fprintf('Радиус базовой станции для нисходящего канала (DL): %.2f км\n', d_DL);
fprintf('Минимальный радиус базовой станции: %.2f км\n', d_min);
fprintf('Площадь одной соты для макросот: %.2f км^2\n', S_one_sector);
fprintf('Площадь покрытия одной базовой станции (макросота): %.2f км^2\n', S_one_macro);
fprintf('Требуемое количество базовых станций для макросот: %d\n', num_sites_macro);

% Вывод результатов для микро- и фемтосот по модели UMi NLOS
fprintf('Радиус базовой станции для микро- и фемтосот (UMi NLOS): %.2f км\n', d_micro_femto);
fprintf('Площадь одной соты для микро- и фемтосот: %.4f км^2\n', S_one_sector_micro_femto);
fprintf('Площадь покрытия одной базовой станции для микро-/фемтосоты: %.4f км^2\n', S_one_micro_femto);
fprintf('Требуемое количество микро- и фемтосот: %d\n', num_sites_micro_femto);