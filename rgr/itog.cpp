#include <iostream>
#include <vector>
#include <ctime>
#include <bitset>
#include <string>

using namespace std;

vector<int> generate_packet(int packet_length){
    vector<int> N_sequence;
    for (int i = 0; i < packet_length; i++){
        N_sequence.push_back(rand() % 2);
    }
    return N_sequence;
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

int main(){
    srand(time(0));

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

    // Вывод битового представления
    cout << "Битовое представление фамилии и имени: " << endl;
    for (size_t i = 0; i < packet.size(); ++i) {
        cout << packet[i];
        if ((i + 1) % 8 == 0) {
            std::cout << " "; // Добавляем пробел после каждых 8 битов
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
    if (checkPacket(packetWithCRC, g_sequence)) {
        cout << "Ошибок не обнаружено в принятом пакете." << endl;
    } else {
        cout << "Ошибка обнаружена в принятом пакете." << endl;
    }


}