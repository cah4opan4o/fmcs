% Считать аудиофайл
[audioData, sampleRate] = audioread('MyVoice.wav');

downsample_factor = 10;
y1 = downsample(y, downsample_factor);
zvuk = audioplayer(y1, Fs / downsample_factor); 
play(zvuk);
% График прореженного сигнала
figure;  
plot(y1);
xlabel('Отсчеты');  % Подпись оси X
ylabel('Амплитуда'); % Подпись оси Y
title('Прореженный сигнал');

% Прямое дискретное преобразование Фурье оригинального сигнала
Y = fft(y);
N = length(Y); % Длина сигнала

% Прямое дискретное преобразование Фурье прореженного сигнала
Y1 = fft(y1);
N1 = length(Y1); % Длина прореженного сигнала

% Вычисляем амплитуды
ampY = abs(Y/N); % Нормировка
ampY1 = abs(Y1/N1); % Нормировка

% Частоты для графиков
f = Fs*(0:(N/2))/N; % Частоты для оригинального сигнала
f1 = (Fs/downsample_factor)*(0:(N1/2))/N1; % Частоты для прореженного сигнала

% График амплитудного спектра оригинального сигнала
figure;
subplot(2,1,1);
plot(f, ampY(1:N/2+1));
xlabel('Частота (Гц)');  % Подпись оси X
ylabel('Амплитуда'); % Подпись оси Y
title('Амплитудный спектр оригинального сигнала');
xlim([0 2000]); % Установить пределы оси X для визуализации

% График амплитудного спектра прореженного сигнала
subplot(2,1,2);
plot(f1, ampY1(1:N1/2+1));
xlabel('Частота (Гц)');  % Подпись оси X
ylabel('Амплитуда'); % Подпись оси Y
title('Амплитудный спектр прореженного сигнала');
xlim([0 2000]); % Установить пределы оси X для визуализации

% Определяем ширину спектра
width_original = find(ampY > 0.01 * max(ampY));
width_downsampled = find(ampY1 > 0.01 * max(ampY1));

disp(['Ширина спектра оригинального сигнала: ' num2str(width_original(end) * Fs / N) ' Гц']);
disp(['Ширина спектра прореженного сигнала: ' num2str(width_downsampled(end) * (Fs/downsample_factor) / N1) ' Гц']);

% Прослушать оригинальный аудиофайл
% sound(audioData, sampleRate);

% Показать информацию о размере аудиофайла
disp(['Размер аудиофайла (всеми каналами): ', num2str(size(audioData))]);
disp(['Частота дискретизации: ', num2str(sampleRate), ' Гц']);

% Построить график волновой формы
t = (0:length(audioData)-1) / sampleRate; % Время
figure;
plot(t, audioData);
xlabel('Время (с)');
ylabel('Амплитуда');
title('Волновая форма аудиозаписи');

% Анализ спектра (используя быстрый преобразование Fourier)
N = length(audioData);
frequencies = (0:N-1) * (sampleRate/N);
spectrum = abs(fft(audioData));

figure;
plot(frequencies, [spectrum; zeros(size(frequencies) - length(spectrum))]);
xlim([0 sampleRate/2]); % Показываем только положительные частоты
xlabel('Частота (Гц)');
ylabel('Амплитуда');
title('Спектр аудиозаписи');

output_filename = 'MyVoice_downsampled.wav';
audiowrite(output_filename, y1, Fs / downsample_factor);