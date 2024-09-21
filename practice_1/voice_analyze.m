% 1. Считать аудиофайл
[audioData, sampleRate] = audioread('MyVoice.wav');

downsample_factor = 10;
y1 = downsample(y, downsample_factor);
zvuk = audioplayer(y1, Fs / downsample_factor); 
play(zvuk);
figure;  % Создаем новую фигуру
plot(y1);
xlabel('Отсчеты');  % Подпись оси X
ylabel('Амплитуда'); % Подпись оси Y
title('Прореженный сигнал');

% 2. Прослушать аудиофайл
% sound(audioData, sampleRate);

% 3. Показать информацию о размере аудиофайла
disp(['Размер аудиофайла (всеми каналами): ', num2str(size(audioData))]);
disp(['Частота дискретизации: ', num2str(sampleRate), ' Гц']);

% 4. Построить график волновой формы
t = (0:length(audioData)-1) / sampleRate; % Время
figure;
plot(t, audioData);
xlabel('Время (с)');
ylabel('Амплитуда');
title('Волновая форма аудиозаписи');

% 5. Анализ спектра (используя быстрый преобразование Fourier)
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