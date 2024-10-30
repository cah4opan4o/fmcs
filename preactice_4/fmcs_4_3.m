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

% Генерация последовательности Голда
gold_sequence = generate_gold_sequence(x, y, taps_x, taps_y, sequence_length);
gold_sequence_2 = generate_gold_sequence(x2, y2, taps_x, taps_y, sequence_length);

% Вывод результата
fprintf('Последовательность Голда 1: ');
disp(gold_sequence);

% Таблица циклических сдвигов и нормализованной автокорреляции
table_data = [];
autocorrelation_values = [];

% Рассчитаем значение автокорреляции при сдвиге 0 для нормализации
shifted_sequence = circshift(gold_sequence, 0);
autocorr_value_0 = mean(gold_sequence .* shifted_sequence);

for shift = 0:sequence_length-1
    shifted_sequence = circshift(gold_sequence, shift);
    autocorr_value = 2 * sum(gold_sequence == shifted_sequence) / sequence_length - 1;
 % Нормализация
    
    % Сохранение данных для таблицы
    table_data = [table_data; shift, shifted_sequence, autocorr_value];
    autocorrelation_values = [autocorrelation_values, autocorr_value];
end

% Отображение таблицы
disp('Циклический сдвиг | Последовательность | Нормализованная автокорреляция');
disp(table_data);

fprintf('Последовательность Голда 2: ');
disp(gold_sequence_2);

% Вычисление взаимной корреляции исходной и новой последовательности
cross_correlation = (2 * sum(gold_sequence == shifted_sequence) / sequence_length - 1) / 3;
 % Нормализация
disp(['Взаимная корреляция исходной и новой последовательностей: ', num2str(cross_correlation)]);

% Построение графика функции нормализованной автокорреляции
lags = 0:sequence_length-1;
figure;
stem(lags, autocorrelation_values, 'filled');
title('Функция нормализованной автокорреляции в зависимости от величины задержки');
xlabel('Задержка (lag)');
ylabel('Нормализованная автокорреляция');
grid on;
