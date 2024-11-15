#include <iostream>
#include <vector>
#include <ctime>
#include <utility>

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
    int variant = 24; // 20 + номер в журнале (4)
    vector<int> g_sequence = {1,1,1,1,0,1,1,1};
    vector<int> packet = generate_packet(variant);
    vector<int> crc = calculateCRC(packet, g_sequence);

    cout << "CRC: ";
    for (int bit : crc) cout << bit;
    cout << endl;

    cout << "packet: ";
    for (int bit : packet) cout << bit;
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

    // Проверка с длиной пакета 250 бит
    variant = 250;
    packet = generate_packet(variant);
    crc = calculateCRC(packet, g_sequence);
    packetWithCRC = appendCRC(packet, crc);

    if (checkPacket(packetWithCRC, g_sequence)) {
        cout << "Ошибок не обнаружено в принятом пакете длиной 250 + crc length бит." << endl;
    } else {
        cout << "Ошибка обнаружена в принятом пакете длиной 250 + crc length бит." << endl;
    }

    // Цикл искажения битов
    int errorsDetected = 0;
    int errorsNotDetected = 0;
    int totalLength = packetWithCRC.size();

    for (int i = 0; i < packet.size(); ++i) {
        // Искажение бита
        packetWithCRC[i] ^= 1; // Инвертируем i-ый бит

        // Проверка на приемной стороне
        if (checkPacket(packetWithCRC, g_sequence)) {
            errorsNotDetected++;
        } else {
            errorsDetected++;
        }
    }

    cout << "Количество обнаруженных ошибок: " << errorsDetected << endl;
    cout << "Количество необнаруженных ошибок: " << errorsNotDetected << endl;

}