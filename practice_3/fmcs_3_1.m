f_1 = 4;
f_2 = f_1 + 4;
f_3 = f_1 * 2 + 1;

t = (0:100-1)/100;

s_1 = cos(2*pi*f_1*t);
s_2 = cos(2*pi*f_2*t);
s_3 = cos(2*pi*f_3*t);

a = 3*s_1 + 3*s_2 + s_3;
b = s_1 + 1/2 * s_2;

%a = [0.3 0.2 -0.1 4.2 -2 1.5 0];
%b = [0.3 4 -2.2 1.6 0.1 0.1 0.2];

% Построение графиков исходных массивов
figure;
subplot(2,1,1);
stem(a, 'filled');
title('Массив a');
xlabel('Индекс');
ylabel('Значение');

subplot(2,1,2);
stem(b, 'filled');
title('Массив b');
xlabel('Индекс');
ylabel('Значение');

% Вычисление взаимной корреляции
[r, lags] = xcorr(a, b, 'coeff');

% Построение графика взаимной корреляции
figure;
plot(lags, r, '-o');
title('Взаимная корреляция a и b');
xlabel('Сдвиг');
ylabel('Коэффициент корреляции');
grid on;

% Поиск максимальной корреляции и соответствующего сдвига
[maxCorr, maxIdx] = max(r);
optimalShift = lags(maxIdx);

% Вывод результатов
fprintf('Максимальная корреляция: %.4f\n', maxCorr);
fprintf('Оптимальный сдвиг: %d\n', optimalShift);

% Сдвиг последовательности b на оптимальное значение
b_shifted = circshift(b, optimalShift);

% Построение графиков для a и сдвинутого b
figure;
subplot(2,1,1);
stem(a, 'filled');
title('Массив a');
xlabel('Индекс');
ylabel('Значение');

subplot(2,1,2);
stem(b_shifted, 'filled');
title('Массив b, сдвинутый на величину оптимальной корреляции');
xlabel('Индекс');
ylabel('Значение');