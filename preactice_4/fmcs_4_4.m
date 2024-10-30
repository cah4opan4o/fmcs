% Функция сдвига регистра с обратной связью
function [reg, output] = lfsr_shift(reg, feedback_positions)
    feedback = 0;
    for pos = feedback_positions
        feedback = xor(feedback, reg(pos));
    end
    output = reg(end);  % Выходной бит - последний бит регистра
    reg = [feedback, reg(1:end-1)];  % Сдвиг регистра с новой обратной связью
end

% Функция генерации последовательности Голда
function gold_sequence = generate_gold_sequence(x, y, taps_x, taps_y, length)
    gold_sequence = zeros(1, length);
    for i = 1:length
        % Выполняем сдвиг для регистров x и y
        [x, bit_x] = lfsr_shift(x, taps_x);
        [y, bit_y] = lfsr_shift(y, taps_y);
        % Результат - побитовая операция XOR между выходными битами x и y
        gold_sequence(i) = xor(bit_x, bit_y);
    end
end

% Параметры для генерации последовательности Голда
x = [0, 0, 1, 0, 0];  % Инициализация регистра x для x = 4
y = [0, 1, 0, 1, 1];  % Инициализация регистра y для y = 11
taps_x = [1, 3];  % Обратные связи для регистра x (0 и 2)
taps_y = [1, 2, 3, 5];  % Обратные связи для регистра y (0, 1, 2 и 4)

x2 = [0, 0, 1, 0, 1];  % Инициализация регистра x для x = 5
y2 = [0, 0, 1, 1, 0];  % Инициализация регистра y для y = 6

% Длина последовательности Голда
sequence_length = 31;

% Генерация последовательностей Голда
gold_sequence = generate_gold_sequence(x, y, taps_x, taps_y, sequence_length);
gold_sequence_2 = generate_gold_sequence(x2, y2, taps_x, taps_y, sequence_length);

% Вывод результата
fprintf('Последовательность Голда 1: ');
disp(gold_sequence);

% Нормализованная автокорреляция
[autocorrelation_values, lags] = xcorr(gold_sequence, 'coeff');

% Таблица сдвигов и значений автокорреляции
table_data = [lags', autocorrelation_values'];
disp('Циклический сдвиг | Нормализованная автокорреляция');
disp(table_data);

fprintf('Последовательность Голда 2: ');
disp(gold_sequence_2);

% Вычисление взаимной корреляции между первой и второй последовательностями
[cross_correlation_values, cross_lags] = xcorr(gold_sequence, gold_sequence_2, 'coeff');
disp(['Взаимная корреляция исходной и новой последовательностей: ', num2str(max(cross_correlation_values))]);

% Построение графика функции нормализованной автокорреляции
figure;
stem(lags, autocorrelation_values, 'filled');
title('Функция нормализованной автокорреляции в зависимости от величины задержки');
xlabel('Задержка (lag)');
ylabel('Нормализованная автокорреляция');
grid on;