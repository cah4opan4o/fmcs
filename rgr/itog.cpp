#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <cmath>
#include <utility>

using namespace std;

// vector<int> generate_BitArray(int array_length){
//     vector<int> array_sequence;
//     for (int i = 0; i < array_length; i++){
//         array_sequence.push_back(rand() % 2);
//     }
//     return array_sequence;
// }

// Функция для выполнения сдвига и возвращения сдвинутого бита
pair<vector<int>, int> shift_sequence(vector<int>& x, int element_1, int element_2) {
    int shift_bit = x[element_1] ^ x[element_2];
    int last_bit = x.back();
    x.pop_back();
    x.insert(x.begin(), shift_bit);
    return make_pair(x, last_bit);
}

// Генерация последовательности Голда
vector<int> generate_sequence_gold(vector<int>& x, vector<int>& y, int length_sequence) {
    vector<int> gold_sequence;
    for (int i = 0; i < length_sequence; i++) {
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
vector<int> cyclic_shift(const vector<int>& seq, int shift) {
    int n = seq.size();
    vector<int> shifted_seq(n);
    for (int i = 0; i < n; i++) {
        shifted_seq[i] = seq[(i + shift) % n];
    }
    return shifted_seq;
}

double calculateCorrelation(const vector<int>& x, const vector<int>& y) {
    if (x.size() != y.size() || x.size() == 0) {
        throw invalid_argument("Arrays must be of the same size and non-empty.");
    }
    
    double meanX = 0.0, meanY = 0.0;
    
    for (size_t i = 0; i < x.size(); ++i) {
        meanX += x[i];
        meanY += y[i];
    }
    meanX /= x.size();
    meanY /= y.size();

    double numerator = 0.0, denominatorX = 0.0, denominatorY = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        numerator += (x[i] - meanX) * (y[i] - meanY);
        denominatorX += (x[i] - meanX) * (x[i] - meanX);
        denominatorY += (y[i] - meanY) * (y[i] - meanY);
    }

    if (denominatorX == 0 || denominatorY == 0) {
        throw runtime_error("Denominator for correlation calculation is zero.");
    }
    
    return numerator / sqrt(denominatorX * denominatorY);
}

// Функция деления с использованием XOR для вычисления CRC
vector<int> calculateCRC(const vector<int>& packet, const vector<int>& generator) {
    vector<int> temp(packet);  // Копируем пакет
    int generatorSize = generator.size();

    // Добавление нулей в конец для расчета CRC
    temp.resize(packet.size() + generatorSize - 1, 0);

    for (int i = 0; i <= temp.size() - generatorSize; ++i) {
        if (temp[i] == 1) {
            for (int j = 0; j < generatorSize; ++j) {
                temp[i + j] ^= generator[j];
            }
        }
    }

    // Возвращаем остаток (CRC), последние биты после выполнения XOR
    return vector<int>(temp.end() - (generatorSize - 1), temp.end());
}

// Функция добавления CRC к пакету
vector<int> appendCRC(const vector<int>& packet, const vector<int>& crc) {
    vector<int> result(packet);
    result.insert(result.end(), crc.begin(), crc.end());
    return result;
}

// Функция проверки принятого пакета (пакет + CRC)
bool checkPacket(const vector<int>& packetWithCRC, const vector<int>& generator) {
    vector<int> remainder = calculateCRC(packetWithCRC, generator);

    // Если остаток весь нули, то ошибок нет
    for (int bit : remainder) {
        if (bit != 0) return false; // Ошибка обнаружена
    }
    return true; // Ошибок нет
}

// Функция кодера: Преобразует строку в вектор битов
vector<int> encodeToBits(const string& input) {
    vector<int> packet;
    for (char c : input) {
        bitset<8> bits(static_cast<unsigned char>(c)); // Преобразование символа в биты
        for (int i = 7; i >= 0; --i) {
            packet.push_back(bits[i]);
        }
    }
    return packet;
}

// Функция декодера: Преобразует вектор битов обратно в строку
string decodeFromBits(const vector<int>& packet) {
    string result;
    if (packet.size() % 8 != 0) {
        throw invalid_argument("Размер вектора битов некорректен (не кратен 8).");
    }

    for (size_t i = 0; i < packet.size(); i += 8) {
        bitset<8> bits;
        for (int j = 0; j < 8; ++j) {
            bits[7 - j] = packet[i + j]; // Заполняем биты с младшего к старшему
        }
        result += static_cast<char>(bits.to_ulong());
    }
    return result;
}

vector<int> append_gold_sequnce(const vector<int>& packetWithCRC, const vector<int>& gold_sequence){
    vector<int> output_signal;
    output_signal.reserve(gold_sequence.size() + packetWithCRC.size());
    output_signal.insert(output_signal.end(), gold_sequence.begin(), gold_sequence.end());
    output_signal.insert(output_signal.end(), packetWithCRC.begin(), packetWithCRC.end());

    return output_signal;
}

vector<int> repeat_elements(const vector<int>& input_vector, int N) {
    vector<int> output_vector;
    output_vector.reserve(input_vector.size() * N); // Резервируем память для выходного вектора

    for (int element : input_vector) {
        for (int i = 0; i < N; ++i) {
            output_vector.push_back(element); // Добавляем элемент N раз
        }
    }

    return output_vector;
}

// vector<int> insert_array_at_position(vector<int>& target_vector, const vector<int>& array_to_insert, int position) {
//     vector<int> output;
//     // Вставляем элементы из array_to_insert в target_vector начиная с указанной позиции
//     output = target_vector.insert(target_vector.begin() + position, array_to_insert.begin(), array_to_insert.end());
//     return output;
// }

int main(){
    // Ввод фамилии и имени
    string surname, name;
    cout << "Введите фамилию: ";
    cin >> surname;
    cout << "Введите имя: ";
    cin >> name;

    // Объединение фамилии и имени для обработки
    string fullName = surname + name; // Если добавить + " " + то будут биты пробела, а это не нужно

    // Строка для хранения битового представления
    vector<int> packet;

    // Преобразование каждого символа в ASCII-код и затем в биты
    for (char c : fullName) {
        bitset<8> bits(static_cast<unsigned char>(c)); // Преобразование символа в биты
        for (int i = 7; i >= 0; --i) {
            packet.push_back(bits[i]);
        }
    }

    cout << "Битовое представление фамилии и имени: " << endl;
    for (size_t i = 0; i < packet.size(); ++i) {
        cout << packet[i];
        if ((i + 1) % 8 == 0) {
            cout << " "; // Добавляем пробел после каждых 8 битов
        }
    }
    cout << endl;

    vector<int> g_sequence = {1,1,1,1,0,1,1,1};
    vector<int> crc = calculateCRC(packet, g_sequence);

    cout << "CRC: ";
    for (int bit : crc) cout << bit;
    cout << endl;

    vector<int> packetWithCRC = appendCRC(packet, crc);
    cout << "Пакет с добавленным CRC: ";
    for (int bit : packetWithCRC) cout << bit;
    cout << endl;

    // Проверка на приемной стороне
    // if (checkPacket(packetWithCRC, g_sequence)) {
    //     cout << "Ошибок не обнаружено в принятом пакете." << endl;
    // } else {
    //     cout << "Ошибка обнаружена в принятом пакете." << endl;
    // }

    // генерация последовательности Голда
    int length_sequence_gold = 31;
    vector<int> x = {0, 0, 1, 0, 0}; // x = 4 в 2СС
    vector<int> y = {0, 1, 1, 0, 1}; // y = x + 7 в 2СС

    vector<int> gold_sequence = generate_sequence_gold(x, y, length_sequence_gold);

    cout << "Первая последовательность Голда: ";
    for (int bit : gold_sequence) {
        cout << bit;
    }
    cout << endl;

    // L - длина битов кодирующих ФИО = 96
    // M - длина CRC = 7
    // G - длина последовательности Голда = 31
    // N - sample rate = 4
    // 1001 0110 0111 1100 0110 1110 1010 000 - golds

    int L = size(packet);
    int M = size(crc);
    int G = size(gold_sequence);
    int N = 4;
    vector<int> array_data(N * (L + M + G),0);
    array_data = append_gold_sequnce(packetWithCRC,gold_sequence);
    array_data = repeat_elements(array_data, N);
    vector<int> array_out(2 * N * (L + M + G), 0);

    int position = 0;
    bool flag = true;
    // Пока нету нужного значения принимаем новые
    while(flag){
        cin >> position;
        if (position >= 0 && position <= N * (L + M + G) ){
            cout << "Значение лежит в этом диапазоне" << endl;
            break;
        }
        cout << "Значение вне диапазона" << endl;
    }
    // array_out = insert_array_at_position(array_data, array_out, position);
    // cout << "p9itb: ";
    // for(int i : array_out){
    //     cout << i;
    // }
    // cout << endl;


}