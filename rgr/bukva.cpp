#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// Для вычисления CRC
// Пораждающий полином из 5 лабы: 11110111

// gold из 4 лабы: 

int main() {
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

    return 0;
}
