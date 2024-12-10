function main
    % Ввод фамилии и имени
    surname = input('Введите фамилию: ', 's');
    name = input('Введите имя: ', 's');
    
    % Объединение фамилии и имени для обработки
    fullName = strcat(surname, '_' ,name);
    
    % Преобразование строки в биты
    packet = encodeToBits(fullName);
    disp('Битовое представление фамилии и имени:');
    dispBits(packet);
    
    figure;
    plot(packet, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(packet)]); 
    ylim([min(packet) - 0.5, max(packet) + 0.5]);


    % Генератор для CRC
    g_sequence = [1, 1, 1, 1, 0, 1, 1, 1];
    crc = calculateCRC(packet, g_sequence);
    packetWithCRC = [packet, crc];
    disp("CRC:");
    disp(crc);

    % Проверка на приемной стороне
    %if checkPacket(packetWithCRC, g_sequence)
    %    disp('Ошибок не обнаружено в принятом пакете.');
    %else
    %    disp('Ошибка обнаружена в принятом пакете.');
    %end
    
    % Генерация последовательности Голда
    length_sequence_gold = 31;
    x = [0, 0, 1, 0, 0]; % x = 4 в 2СС
    y = [0, 1, 1, 0, 1]; % y = x + 7 в 2СС
    gold_sequence = generate_sequence_gold(x, y, length_sequence_gold);
    %disp('Первая последовательность Голда:');
    %disp(gold_sequence);
    
    % Параметры
    L = length(packet);
    M = length(crc);
    G = length(gold_sequence);
    N = 6; % Sample rate
    N1 = N / 2;
    N2 = N * 2;
    array_data = repmat([gold_sequence, packetWithCRC], 1);
    array_data_1 = repmat([gold_sequence, packetWithCRC], 1);
    array_data_2 = repmat([gold_sequence, packetWithCRC], 1);
    
    % стандартный sample rate
    array_out = zeros(1, 2 * N * (L + M + G));
    % уменьшин sample rate в 2 раза
    array_out_1 = zeros(1, 2 * N1 * (L + M + G));
    % увеличен sample rate в 2 раза
    array_out_2 = zeros(1, 2 * N2 * (L + M + G));

    figure;
    plot(array_data, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization GOLD + DATA + CRC', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_data)]); 
    ylim([min(array_data) - 0.5, max(array_data) + 0.5]);

    array_data = repeat_elements(array_data,N);
    array_data_1 = repeat_elements(array_data_1,N1);
    array_data_2 = repeat_elements(array_data_2,N2);

    figure;

    % Первый график
    subplot(3, 1, 1); % 3 строки, 1 столбец, 1-я позиция
    plot(array_data, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization Sample rate * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_data)]); 
    ylim([min(array_data) - 0.5, max(array_data) + 0.5]);
    
    % Второй график
    subplot(3, 1, 2); % 3 строки, 1 столбец, 2-я позиция
    plot(array_data_1, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization (Sample rate / 2) * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_data_1)]); 
    ylim([min(array_data_1) - 0.5, max(array_data_1) + 0.5]);
    
    % Третий график
    subplot(3, 1, 3); % 3 строки, 1 столбец, 3-я позиция
    plot(array_data_2, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization (Sample rate * 2) * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_data_2)]); 
    ylim([min(array_data_2) - 0.5, max(array_data_2) + 0.5]);

    % Ввод позиции
    flag = true;
    while flag
        position = input('Введите позицию вставки: ');
        if position >= 0 && position <= N * (L + M + G)
            disp('Значение лежит в диапазоне');
            flag = false;
        else
            disp('Значение вне диапазона');
        end
    end
    
    % Вставка данных
    array_out = insert_array_at_position(array_out, array_data, position);
    array_out_1 = insert_array_at_position(array_out_1, array_data_1, position);
    array_out_2 = insert_array_at_position(array_out_2, array_data_2, position);

    figure;
    % Первый график
    subplot(3, 1, 1); % 3 строки, 1 столбец, 1-я позиция
    plot(array_out, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization Sample rate * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_out)]); 
    ylim([min(array_out) - 0.5, max(array_out) + 0.5]);
    
    % Второй график
    subplot(3, 1, 2); % 3 строки, 1 столбец, 2-я позиция
    plot(array_out_1, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization (Sample rate / 2) * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_out_1)]); 
    ylim([min(array_out_1) - 0.5, max(array_out_1) + 0.5]);
    
    % Третий график
    subplot(3, 1, 3); % 3 строки, 1 столбец, 3-я позиция
    plot(array_out_2, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization (Sample rate * 2) * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(array_out_2)]); 
    ylim([min(array_out_2) - 0.5, max(array_out_2) + 0.5]);

    % Генерация шума
    sigma = input('Введите стандартное отклонение шума: ');
    array_with_hastle = generate_noise(2 * N * (L + M + G), 0, sigma);
    array_with_hastle_1 = generate_noise(2 * N1 * (L + M + G), 0, sigma);
    array_with_hastle_2 = generate_noise(2 * N2 * (L + M + G), 0, sigma);
    %disp('Шум:');

    array_out = double(array_out);
    array_out_1 = double(array_out_1);
    array_out_2 = double(array_out_2);

    result = array_out + array_with_hastle;
    result_1 = array_out_1 + array_with_hastle_1;
    result_2 = array_out_2 + array_with_hastle_2;

    %disp(result);
 figure;
    % Первый график
    subplot(3, 1, 1); % 3 строки, 1 столбец, 1-я позиция
    plot(result, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization Sample rate * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(result)]); 
    ylim([min(result) - 0.5, max(result) + 0.5]);
    
    % Второй график
    subplot(3, 1, 2); % 3 строки, 1 столбец, 2-я позиция
    plot(result_1, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization (Sample rate / 2) * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(result_1)]); 
    ylim([min(result_1) - 0.5, max(result_1) + 0.5]);
    
    % Третий график
    subplot(3, 1, 3); % 3 строки, 1 столбец, 3-я позиция
    plot(result_2, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization (Sample rate * 2) * (GOLD + DATA + CRC)', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(result_2)]); 
    ylim([min(result_2) - 0.5, max(result_2) + 0.5]);

    % Находим начало синхропоследовательности
    gold_sequence_double = double(gold_sequence);
    gold_sequence_double = repeat_elements(gold_sequence_double,N);
    [sync_index,correlation] = findSyncSequenceStart(result, gold_sequence_double, N, L, M, G);

    figure;
    plot(correlation, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Signal Visualization hastle with signal', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(correlation)]); 
    ylim([min(correlation) - 0.5, max(correlation) + 0.5]);

    disp(sync_index);
    
    % Обрезаем до синхры
    signal_out = result(sync_index :end);
    %disp(signal_out);

    figure;
    plot(signal_out, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Обрезал до синхры', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(signal_out)]); 
    ylim([min(signal_out) - 0.5, max(signal_out) + 0.5]);
    
    bits_sequence = decodeSignal(signal_out,N);

    figure;
    plot(bits_sequence, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('декодировал биты', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(bits_sequence)]); 
    ylim([min(bits_sequence) - 0.5, max(bits_sequence) + 0.5]);
    %disp(bits_sequence);
    %disp(array_data);

    bits_sequence_W_N = bits_sequence(1 : end);
    figure;
    plot(bits_sequence_W_N, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Обрезал до Голда', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(bits_sequence_W_N)]); 
    ylim([min(bits_sequence_W_N) - 0.5, max(bits_sequence_W_N) + 0.5]);
    
    bits_sequence_W_NG = bits_sequence_W_N(G+1 :end);

    figure;
    plot(bits_sequence_W_NG, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Обрезал Голда', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(bits_sequence_W_NG)]); 
    ylim([min(bits_sequence_W_NG) - 0.5, max(bits_sequence_W_NG) + 0.5]);

    % Проверка на приемной стороне
    [isValid, crcLength, endIndex] = checkPacket(bits_sequence_W_NG, g_sequence);
    disp(crcLength);
    disp(endIndex);
    if isValid
        disp('Ошибок не обнаружено в принятом пакете.');
    else
        disp('Ошибка обнаружена в принятом пакете.');
    end
    
    bits_sequence_W_NGM = bits_sequence_W_NG(1 : crcLength);
    
    figure;
    plot(bits_sequence_W_NGM, 'LineWidth', 2);
    xlabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('Signal', 'FontSize', 12, 'FontWeight', 'bold');
    title('Обрезал CRC + шум', 'FontSize', 14, 'FontWeight', 'bold');
    grid on;
    set(gca, 'FontSize', 12, 'FontWeight', 'bold');
    xlim([0 length(bits_sequence_W_NGM)]); 
    ylim([min(bits_sequence_W_NGM) - 0.5, max(bits_sequence_W_NGM) + 0.5]);

    slovo = decodeFromBits(bits_sequence_W_NGM);
    disp(slovo);
end

function packet = encodeToBits(input)
    packet = [];
    for c = input
        bits = bitget(uint8(c), 8:-1:1); % Преобразование символа в биты
        packet = [packet, bits];
    end
end

function output = decodeFromBits(packet)
    % Проверяем, что длина пакета делится на 8 (по 8 бит на символ)
    if mod(length(packet), 8) ~= 0
        error('Длина пакета должна быть кратной 8 для корректного декодирования.');
    end

    % Инициализация результирующего массива
    output = char([]); % Символы сохраняем в строку

    % Преобразуем каждые 8 бит в символ
    for i = 1:8:length(packet)
        % Берем 8 бит и преобразуем их в байт
        bits = packet(i:i+7);
        byte = uint8(bin2dec(num2str(bits))); % Преобразуем массив бит в десятичное значение
        output = [output, char(byte)]; % Преобразуем байт в символ и добавляем к результату
    end
end


function dispBits(bits)
    for i = 1:length(bits)
        fprintf('%d', bits(i));
        if mod(i, 8) == 0
            fprintf(' '); % Пробел после каждых 8 бит
        end
    end
    fprintf('\n');
end

function [crc, crcLength, endIndex] = calculateCRC(packet, generator)
    % Дополнение пакета нулями
    temp = [packet, zeros(1, length(generator) - 1)];
    crcLength = 0; % Счётчик длины CRC
    endIndex = -1; % Индекс завершения CRC
    generatorLength = length(generator);

    for i = 1:length(packet)
        if temp(i) == 1
            % Выполняем XOR с полиномом генератора
            temp(i:i + generatorLength - 1) = xor(temp(i:i + generatorLength - 1), generator);
        end
        
        % Увеличиваем счётчик длины CRC
        crcLength = crcLength + 1;

        % Проверяем, заполнен ли текущий отрезок нулями
        if all(temp(i+1:end) == 0)
            endIndex = i + generatorLength - 1; % Запоминаем индекс завершения CRC
            break;
        end
    end

    % Извлекаем CRC
    crc = temp(end - (generatorLength - 2):end);
end


function [isValid, crcLength, endIndex] = checkPacket(packetWithCRC, generator)
    % Вычисляем остаток и информацию о CRC
    [remainder, crcLength, endIndex] = calculateCRC(packetWithCRC, generator);
    
    % Проверяем, что остаток равен нулю
    isValid = all(remainder == 0);
end


function gold_sequence = generate_sequence_gold(x, y, length_sequence)
    gold_sequence = zeros(1, length_sequence);
    for i = 1:length_sequence
        [x, shift_bit_x] = shift_sequence(x, 2, 4);
        [y, shift_bit_y] = shift_sequence(y, 2, 4);
        gold_sequence(i) = xor(shift_bit_x, shift_bit_y);
    end
end

function [seq, last_bit] = shift_sequence(seq, element_1, element_2)
    shift_bit = xor(seq(element_1 + 1), seq(element_2 + 1));
    last_bit = seq(end);
    seq = [shift_bit, seq(1:end - 1)];
end

function output_vector = repeat_elements(input_vector, N)
    output_vector = [];
    for i = 1:length(input_vector)
        output_vector = [output_vector, repmat(input_vector(i), 1, N)];
    end
end

function target_vector = insert_array_at_position(target_vector, array_to_insert, position)
    % Проверка, чтобы избежать выхода за пределы массива
    if position + length(array_to_insert) - 1 > length(target_vector)
        error('Размер массива для вставки превышает размер целевого вектора.');
    end
    
    % Заменяем элементы target_vector начиная с позиции position
    target_vector(position:position+length(array_to_insert)-1) = array_to_insert;
end

function noise = generate_noise(size, mu, sigma)
    noise = mu + sigma * randn(1, size);
end

function [syncStartIndex, correlations] = findSyncSequenceStart(array_with_hastle, gold_sequence_double, N, L, M, G)
    % Инициализация переменной для результата
    syncStartIndex = -1;  % По умолчанию синхроимпульс не найден
    
    % Длина синхронизирующей последовательности
    G_N = G * N;
    
    % Проверка, что длина входного массива достаточна для поиска
    if length(array_with_hastle) < G_N
        disp('Длина массива слишком мала для поиска синхронизирующей последовательности.');
        return;
    end
    
    % Инициализация массива для хранения корреляций
    correlations = zeros(1, length(array_with_hastle) - G_N + 1);
    
    % Проход по массиву для поиска синхронизирующей последовательности
    for i = 1:(length(array_with_hastle) - G_N + 1)  % Для корректного индексации до конца массива
        % Извлекаем подмассив длиной G * N начиная с позиции i
        synchro_sequence = array_with_hastle(i:i + G_N - 1);
        
        % Вычисляем корреляцию между подмассивом и эталонной последовательностью
        result = corr(synchro_sequence', gold_sequence_double');  % Применяем транспонирование
        
        % Сохраняем корреляцию в массив
        correlations(i) = result;
        
        % Проверяем, если корреляция близка к 1 (синхроимпульс найден)
        % Захлопнуть начало синхры
        if abs(result) >= 0.76 % что-то поделать с SINR 
            disp(['Найден вход в синхроимпульс на индексе: ', num2str(i)]);
            syncStartIndex = i; % Возвращаем индекс начала синхроимпульса
            break;
        end
    end

    % Если синхроимпульс не найден
    if syncStartIndex == -1
        disp('Синхроимпульс не найден.');
    end
end

function decoded_bits = decodeSignal(signal, N)
    % Функция для декодирования сигналов группами по N отсчетов
    % signal - входной массив данных
    % N - количество отсчетов на символ

    P = 0.6; % Порог для интерпретации
    decoded_bits = []; % Результирующий массив для хранения декодированных битов

    % Длина входного сигнала
    signal_length = length(signal);

    % Обработка сигналов группами длиной N
    for i = 1:N:(signal_length - N + 1)
        % Суммируем N отсчетов
        segment = signal(i:i + N - 1); % Извлекаем сегмент длиной N
        average = mean(segment); % Среднее значение сегмента

        % Вывод среднего значения для отладки
        %disp(average);

        % Принятие решения: 0 или 1
        if average >= P
            decoded_bits = [decoded_bits, 1]; % Если среднее выше порога, добавляем 1
        else
            decoded_bits = [decoded_bits, 0]; % Иначе добавляем 0
        end
    end
end
