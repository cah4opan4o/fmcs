#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <utility>
#include <random>
#include <fstream>
#include <algorithm>

using namespace std;

// Функция для выполнения сдвига и возвращения сдвинутого бита
pair<vector<int>, int> shift_sequence(vector<int> &x, int element_1, int element_2)
{
    int shift_bit = x[element_1] ^ x[element_2];
    int last_bit = x.back();
    x.pop_back();
    x.insert(x.begin(), shift_bit);
    return make_pair(x, last_bit);
}

// Генерация последовательности Голда
vector<int> generate_sequence_gold(vector<int> &x, vector<int> &y, int length_sequence)
{
    vector<int> gold_sequence;
    for (int i = 0; i < length_sequence; i++)
    {
        auto result_x = shift_sequence(x, 2, 4);
        auto result_y = shift_sequence(y, 2, 4);

        x = result_x.first;
        y = result_y.first;

        int shift_bit_x = result_x.second;
        int shift_bit_y = result_y.second;

        gold_sequence.push_back(shift_bit_x ^ shift_bit_y);
    }
    return gold_sequence;
}

// Функция для циклического сдвига последовательности
vector<int> cyclic_shift(const vector<int> &seq, int shift)
{
    int n = seq.size();
    vector<int> shifted_seq(n);
    for (int i = 0; i < n; i++)
    {
        shifted_seq[i] = seq[(i + shift) % n];
    }
    return shifted_seq;
}

double calculateCorrelation(const vector<double> &x, const vector<double> &y)
{
    if (x.size() != y.size() || x.size() == 0)
    {
        throw invalid_argument("Векторы имеют разную длину или пустые!");
    }

    double meanX = 0.0, meanY = 0.0;

    for (size_t i = 0; i < x.size(); ++i)
    {
        meanX += x[i];
        meanY += y[i];
    }
    meanX /= x.size();
    meanY /= y.size();

    double numerator = 0.0, denominatorX = 0.0, denominatorY = 0.0;

    for (size_t i = 0; i < x.size(); ++i)
    {
        numerator += (x[i] - meanX) * (y[i] - meanY);
        denominatorX += (x[i] - meanX) * (x[i] - meanX);
        denominatorY += (y[i] - meanY) * (y[i] - meanY);
    }

    if (denominatorX == 0 || denominatorY == 0)
    {
        throw runtime_error("Denominator for correlation calculation is zero.");
    }

    return numerator / sqrt(denominatorX * denominatorY);
}

// Функция деления с использованием XOR для вычисления CRC
vector<int> calculateCRC(const vector<int> &packet, const vector<int> &generator)
{
    vector<int> temp(packet); // Копируем пакет
    int generatorSize = generator.size();

    // Добавление нулей в конец для расчета CRC
    temp.resize(packet.size() + generatorSize - 1, 0);

    for (int i = 0; i <= temp.size() - generatorSize; ++i)
    {
        if (temp[i] == 1)
        {
            for (int j = 0; j < generatorSize; ++j)
            {
                temp[i + j] ^= generator[j];
            }
        }
    }

    // Возвращаем остаток (CRC), последние биты после выполнения XOR
    return vector<int>(temp.end() - (generatorSize - 1), temp.end());
}

// Функция добавления CRC к пакету
vector<int> appendCRC(const vector<int> &packet, const vector<int> &crc)
{
    vector<int> result(packet);
    result.insert(result.end(), crc.begin(), crc.end());
    return result;
}

// Функция проверки принятого пакета (пакет + CRC)
bool checkPacket(const vector<int> &packetWithCRC, const vector<int> &generator)
{
    vector<int> remainder = calculateCRC(packetWithCRC, generator);

    // Если остаток весь нули, то ошибок нет
    for (int bit : remainder)
    {
        if (bit != 0)
            return false; // Ошибка обнаружена
    }
    return true; // Ошибок нет
}

// Функция кодера: Преобразует строку в вектор битов
vector<int> encodeToBits(const string &input)
{
    vector<int> packet;
    for (char c : input)
    {
        bitset<8> bits(static_cast<unsigned char>(c)); // Преобразование символа в биты
        for (int i = 7; i >= 0; --i)
        {
            packet.push_back(bits[i]);
        }
    }
    return packet;
}

// Функция декодера: Преобразует вектор битов обратно в строку
string decodeFromBits(const vector<int> &packet)
{
    string result;
    if (packet.size() % 8 != 0)
    {
        throw invalid_argument("Размер вектора битов некорректен (не кратен 8).");
    }

    for (size_t i = 0; i < packet.size(); i += 8)
    {
        bitset<8> bits;
        for (int j = 0; j < 8; ++j)
        {
            bits[7 - j] = packet[i + j]; // Заполняем биты с младшего к старшему
        }
        result += static_cast<char>(bits.to_ulong());
    }
    return result;
}

vector<int> append_gold_sequnce(const vector<int> &packetWithCRC, const vector<int> &gold_sequence)
{
    vector<int> output_signal;
    output_signal.reserve(gold_sequence.size() + packetWithCRC.size());
    output_signal.insert(output_signal.end(), gold_sequence.begin(), gold_sequence.end());
    output_signal.insert(output_signal.end(), packetWithCRC.begin(), packetWithCRC.end());

    return output_signal;
}

vector<int> repeat_elements(const vector<int> &input_vector, int N)
{
    vector<int> output_vector;
    output_vector.reserve(input_vector.size() * N); // Резервируем память для выходного вектора

    for (int element : input_vector)
    {
        for (int i = 0; i < N; ++i)
        {
            output_vector.push_back(element); // Добавляем элемент N раз
        }
    }

    return output_vector;
}

vector<double> repeat_elements_double(const vector<double> &input_vector, int N)
{
    vector<double> output_vector;
    output_vector.reserve(input_vector.size() * N); // Резервируем память для выходного вектора

    for (int element : input_vector)
    {
        for (int i = 0; i < N; ++i)
        {
            output_vector.push_back(element); // Добавляем элемент N раз
        }
    }

    return output_vector;
}

// Вставляем на position в массиве весь массив данных
vector<int> insert_array_at_position(vector<int> &target_vector, const vector<int> &array_to_insert, int position)
{
    // Проверяем, что позиция корректна
    if (position < 0 || position >= target_vector.size())
    {
        cout << "Invalid position!" << endl;
        return target_vector;
    }

    // Проверяем, что длина вставляемого массива не превышает доступного места в target_vector
    if (position + array_to_insert.size() > target_vector.size())
    {
        cout << "Not enough space in target vector to insert the array!" << endl;
        return target_vector;
    }

    // Заменяем элементы target_vector начиная с позиции position
    for (size_t i = 0; i < array_to_insert.size(); ++i)
    {
        target_vector[position + i] = array_to_insert[i];
    }

    return target_vector;
}

// Функция для генерации шума с нормальным распределением
void generate_noise(vector<double> &noise, int size, double mu, double sigma)
{
    // Создаём генератор случайных чисел
    random_device rd;
    mt19937 gen(rd()); // Генератор случайных чисел

    // Создаём нормальное распределение с параметрами mu (среднее) и sigma (стандартное отклонение)
    normal_distribution<> dist(mu, sigma);

    // Заполняем вектор случайными значениями из нормального распределения
    for (int i = 0; i < size; ++i)
    {
        noise[i] = dist(gen); // Генерируем случайное число и присваиваем его вектору
    }
}

vector<double> convert_int_to_double(const vector<int> &int_vector)
{
    vector<double> double_vector;

    // Преобразование каждого элемента
    for (int value : int_vector)
    {
        double_vector.push_back(static_cast<double>(value)); // Приведение int к double
    }

    return double_vector;
}

vector<double> add_vectors(const vector<double> &vec1, const vector<double> &vec2)
{
    // Проверяем, что размеры векторов совпадают
    if (vec1.size() != vec2.size())
    {
        throw runtime_error("Размерность массивов не равны!");
    }

    vector<double> result(vec1.size());
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        result[i] = vec1[i] + vec2[i];
    }

    return result;
}

int findSyncSequenceStart(const vector<double> &array_with_hastle,
                          const vector<double> &gold_sequence_double,
                          int N, int L, int M, int G)
{
    double result = 0;
    double max_correlation = 0;
    int max_index = -1;
    int G_N = G * N; // Длина синхронизирующей последовательности

    // Проверяем, чтобы длина данных была достаточной
    if (array_with_hastle.size() < G_N)
    {
        cout << "Длина массива слишком мала для поиска синхронизирующей последовательности." << endl;
        return -1;
    }

    // Проходим по массиву и ищем синхронизирующую последовательность
    for (int i = 0; i <= N * (L + M + G) - G_N; ++i)
    {
        // Извлекаем подмассив длиной G * N начиная с позиции i
        vector<double> synchro_sequence(array_with_hastle.begin() + i,
                                        array_with_hastle.begin() + i + G_N);
        // Вычисляем корреляцию
        result = calculateCorrelation(synchro_sequence, gold_sequence_double);

        // Обновляем максимальную корреляцию, если она выше текущей
        if (abs(result) > abs(max_correlation))
        {
            max_correlation = result;
            max_index = i;
        }

        // Проверяем корреляцию
        if (abs(result) == 1)
        {
            cout << "Найден вход в синхроимпульс на индексе: " << i << endl;
            return i; // Возвращаем индекс начала
        }
    }

    // Если синхроимпульс не найден, возвращаем индекс с максимальной корреляцией
    if (max_index != -1)
    {
        cout << "Синхроимпульс не найден. Максимальная корреляция: "
             << max_correlation << " на индексе: " << max_index << endl;
    }
    else
    {
        cout << "Синхроимпульс не найден и корреляций не найдено." << endl;
    }
    return max_index;
}

vector<double> decode_signal(const vector<double> &signal, int N, double P)
{

    // Результирующий вектор для хранения декодированных битов
    vector<double> decoded_bits;

    // Длина сигнала
    size_t signal_length = signal.size();

    // Обработка сигналов группами длиной N
    for (size_t i = 0; i + N <= signal_length; i += N)
    {
        // Суммируем N отсчетов
        double sum = 0.0;
        for (size_t j = 0; j < N; ++j)
        {
            sum += signal[i + j];
        }

        // Среднее значение текущей группы
        double average = sum / N;

        // cout << average << endl;

        // Принимаем решение: 0 или 1
        if (average >= P)
        {
            decoded_bits.push_back(1); // Если среднее выше порога, это 1
        }
        else
        {
            decoded_bits.push_back(0); // Иначе 0
        }
    }

    // Возвращаем массив декодированных битов
    return decoded_bits;
}

vector<int> convertVectorToInt(const vector<double> &input)
{
    vector<int> result;
    result.reserve(input.size()); // Резервируем память для оптимизации

    for (double value : input)
    {
        result.push_back(static_cast<int>(round(value))); // Округление
    }

    return result;
}

int main()
{
    // srand(time(0));
    // Ввод фамилии и имени
    string surname, name;
    // cout << "Введите фамилию: ";
    // cin >> surname;
    // cout << "Введите имя: ";
    // cin >> name;
    surname = "volodin";
    name = "sasha";

    // Объединение фамилии и имени для обработки
    string fullName = surname + "_" + name; // Если добавить + " " + то будут биты пробела

    // Строка для хранения битового представления
    vector<int> packet;

    // Преобразование каждого символа в ASCII-код и затем в биты
    for (char c : fullName)
    {
        bitset<8> bits(static_cast<unsigned char>(c)); // Преобразование символа в биты
        for (int i = 7; i >= 0; --i)
        {
            packet.push_back(bits[i]);
        }
    }

    vector<int> g_sequence = {1, 1, 1, 1, 0, 1, 1, 1};
    vector<int> crc = calculateCRC(packet, g_sequence);

    vector<int> packetWithCRC = appendCRC(packet, crc);

    // генерация последовательности Голда
    int length_sequence_gold = 31;
    vector<int> x = {0, 0, 1, 0, 0}; // x = 4 в 2СС
    vector<int> y = {0, 1, 1, 0, 1}; // y = x + 7 в 2СС

    vector<int> gold_sequence = generate_sequence_gold(x, y, length_sequence_gold);

    int L = size(packet);
    int M = size(crc);
    int G = size(gold_sequence);
    int N = 4;
    vector<int> array_data(N * (L + M + G), 0);
    array_data = append_gold_sequnce(packetWithCRC, gold_sequence);

    vector<int> array_data_1 = array_data;
    array_data = repeat_elements(array_data, N);

    vector<int> array_out(2 * N * (L + M + G), 0);

    // Хардкод
    int position = 83;
    bool flag = true;

    array_out = insert_array_at_position(array_out, array_data, position);
    double P = 0.30;
    // Перебор значений порога
    vector<double> probability_error;
    for (double sigma = 0.0; sigma <= 1 + 1e-9; sigma += 0.05)
    { // Перебор порога интерпретации
        // Итоговый вектор для хранения результата
        cout << "Пробуем P = " << P << endl;
        vector<double> resultVector;
        // for (double sigma = 0.0; sigma < 1; sigma += 0.05)
        // { // Перебо шума
            cout << "   Пробуем sigma = " << sigma << endl;
            vector<int> probability;
            for (int i = 0; i < 100; i++)
            { // Итерации для среднего значений

                float mu = 0.0;

                // задаём пустой массив
                vector<double> array_with_hastle(2 * N * (L + M + G));
                // создаём шум
                generate_noise(array_with_hastle, array_with_hastle.size(), mu, sigma);

                // cout << "Noise:"<<endl;
                // for (double bit : array_with_hastle) {
                //     cout << bit << " ";
                // }
                // cout << endl;

                // перевёл
                vector<double> array_out_double = convert_int_to_double(array_out);

                vector<double> signal_out = add_vectors(array_out_double, array_with_hastle);

                vector<double> gold_sequence_double = convert_int_to_double(gold_sequence);

                gold_sequence_double = repeat_elements_double(gold_sequence_double, N);

                int sync_index = findSyncSequenceStart(signal_out, gold_sequence_double, N, L, M, G);

                signal_out.erase(signal_out.begin(), signal_out.begin() + sync_index + 1);

                vector<double> signal_after_decoding = decode_signal(signal_out, N, P);

                signal_after_decoding.erase(signal_after_decoding.begin(), signal_after_decoding.begin() + G);

                signal_after_decoding.erase(signal_after_decoding.begin() + L + M, signal_after_decoding.end()); // ПРОБЛЕМА в декодирование битов

                vector<int> signal_after_decoding_int = convertVectorToInt(signal_after_decoding);

                // Проверка на приемной стороне
                if (checkPacket(signal_after_decoding_int, g_sequence))
                {
                    cout << "net problem Ошибок не обнаружено в принятом пакете." << endl;
                    probability.push_back(1);
                    continue;
                }
                else
                {
                    cout << "problem Ошибка обнаружена в принятом пакете." << endl;
                    probability.push_back(0);
                    continue;
                }

                for (int i = 0; i < signal_after_decoding_int.size(); i++)
                {
                    if (packetWithCRC[i] != signal_after_decoding_int[i])
                    {
                        cout << "ERROR" << endl;
                        cout << "Error element position " << i << endl;
                        break;
                    }
                }

                // обрезаю CRC
                signal_after_decoding_int.erase(signal_after_decoding_int.begin() + L, signal_after_decoding_int.end());

                cout << "FINISH" << endl;

                string bukava = decodeFromBits(signal_after_decoding_int);
                cout << bukava << endl;
            }
            int zeroCount = count(probability.begin(), probability.end(), 0);

            // Проверка длины вектора (на случай пустого вектора)
            if (probability.empty())
            {
                cerr << "Вектор пустой!" << endl;
                return 1;
            }

            // Вычисление отношения количества нулей к длине вектора
            double zeroRatio = static_cast<double>(zeroCount) / probability.size();

            probability.clear();

            resultVector.push_back(zeroRatio);
        // }
        cout << resultVector.size() << endl;
        cout << "ResultVector: " << endl;
        for (double bit : resultVector)
        {
            cout << bit << " ";
        }
        cout << endl;
        // porog += 0.05;
        probability_error.push_back(resultVector[0]);
    }
    cout << "probability_error: ";
        for (double bit : probability_error)
        {
            cout << bit << " ";
        }
        cout << endl;
    // Запись данных в файл "probability_error.txt"
    ofstream outFile("probability_error.txt");
    if (outFile.is_open()) {
        for (double bit : probability_error) {
            outFile << bit << " ";
        }
        outFile.close();
        cout << "Данные успешно записаны в файл \"probability_error.txt\"." << endl;
    } else {
        cerr << "Не удалось открыть файл для записи." << endl;
    }
    // Запись данных в файл "probability_error.txt"
}