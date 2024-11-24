#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

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

int main() {
    string surname, name;
    cout << "Введите фамилию: ";
    cin >> surname;
    cout << "Введите имя: ";
    cin >> name;

    string fullName = surname + " " + name; // Закодируем ещё и пробел

    vector<int> packet = encodeToBits(fullName);

    cout << "Битовое представление фамилии и имени: " << endl;
    for (size_t i = 0; i < packet.size(); ++i) {
        cout << packet[i];
        if ((i + 1) % 8 == 0) {
            cout << " ";
        }
    }
    cout << endl;

    // Декодирование обратно в строку
    string decodedString = decodeFromBits(packet);
    cout << "Восстановленная строка: " << decodedString << endl;

    return 0;
}
