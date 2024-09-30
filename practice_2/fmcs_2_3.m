% Исходные данные
f = 1800;          % Частота в МГц
hBS = 30;          % Высота антенны базовой станции в метрах
hUE = 1;           % Высота антенны пользователя в метрах
d = linspace(0.1, 10, 100); % Массив расстояний в км

% Модель COST231 Hata
A_COST231 = 46.3;      % Коэффициент A для частот 1500-2000 МГц
B_COST231 = 33.9;      % Коэффициент B для частот 1500-2000 МГц
Lclutter_COST231 = 3;  % Поправка на городскую застройку в дБ
%Lclutter_COST231 = -(4.78*(log10(f/28))^2-18.33*log10(f)+35.94)

% Поправка на высоту пользователя
a_hUE_COST231 = (1.1 * log10(f) - 0.7) * hUE - (1.56 * log10(f) - 0.8);

% Модель UMi NLOS (обобщенная формула для UMi)
L_UMiNLOS = @(d) 22.7 + 36.7 * log10(d) + 26 * log10(f); % Пример зависимости для UMiNLOS

% Модель Walfish-Ikegami
L_WI = @(d) 42.6 + 26*log10(d) + 20*log10(f) + (log10(hBS) + log10(hUE) - 9); % Упрощенная формула

% Расчет потерь для COST231 Hata
PL_COST231 = A_COST231 + B_COST231 * log10(f) - 13.82 * log10(hBS) - a_hUE_COST231 + ...
             (44.9 - 6.55 * log10(hBS)) * log10(d) + Lclutter_COST231;

% Расчет потерь для UMiNLOS
PL_UMiNLOS = L_UMiNLOS(d);

% Расчет потерь для Walfish-Ikegami
PL_WI = L_WI(d);

% Построение графиков
figure;
plot(d, PL_COST231, 'r', 'LineWidth', 2); hold on;
plot(d, PL_UMiNLOS, 'g', 'LineWidth', 2);
plot(d, PL_WI, 'b', 'LineWidth', 2);

xlabel('Расстояние (км)');
ylabel('Потери сигнала (дБ)');
title('Зависимость потерь радиосигнала от расстояния');
legend('COST231 Hata', 'UMi NLOS', 'Walfish-Ikegami');
grid on;
