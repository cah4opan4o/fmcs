#include <iostream>
#include <vector>

// Функция сдвига с линейной обратной связью для одного регистра
int lfsr_shift(std::vector<int>& reg, const std::vector<int>& taps) {
    int feedback = 0;
    for (int tap : taps) {
        feedback ^= reg[tap];
    }
    int output = reg.back();
    reg.pop_back();
    reg.insert(reg.begin(), feedback);
    return output;
}

// Функция генерации последовательности Голда
std::vector<int> generate_gold_sequence(std::vector<int> x, std::vector<int> y, int length) {
    std::vector<int> gold_sequence;
    // Задаем полиномы обратной связи
    std::vector<int> taps_x = {0, 2}; // Позиции обратной связи для x
    std::vector<int> taps_y = {0, 1, 2, 4}; // Позиции обратной связи для y

    for (int i = 0; i < length; ++i) {
        int bit_x = lfsr_shift(x, taps_x);
        int bit_y = lfsr_shift(y, taps_y);
        gold_sequence.push_back(bit_x ^ bit_y); // XOR выходов двух LFSR
    }

    return gold_sequence;
}

// Функция для циклического сдвига вектора вправо на 1 позицию
std::vector<int> cyclic_shift(const std::vector<int>& sequence) {
    std::vector<int> shifted(sequence.size());
    shifted[0] = sequence.back();
    for (size_t i = 1; i < sequence.size(); ++i) {
        shifted[i] = sequence[i - 1];
    }
    return shifted;
}

// Функция для расчета автокорреляции между двумя последовательностями
double calculate_correlation(const std::vector<int>& seq1, const std::vector<int>& seq2) {
    int match = 0;
    for (size_t i = 0; i < seq1.size(); ++i) {
        if (seq1[i] == seq2[i]) {
            match++;
        }
    }
    return 2.0 * match / seq1.size() - 1;
}

// Функция для расчета автокорреляции на всех сдвигах
void autocorrelation_table(const std::vector<int>& sequence) {
    std::vector<int> shifted_sequence = sequence;
    std::cout << "Сдвиг\tПоследовательность\tАвтокорреляция\n";
    for (size_t shift = 0; shift < sequence.size(); ++shift) {
        double correlation = calculate_correlation(sequence, shifted_sequence);
        std::cout << shift << "\t";
        for (int bit : shifted_sequence) {
            std::cout << bit << " ";
        }
        std::cout << "\t" << correlation << "\n";
        shifted_sequence = cyclic_shift(shifted_sequence);
    }
}

int main() {
    // Инициализация регистров с начальным состоянием для x = 4 (00100) и y = 9 (01001)
    std::vector<int> x = {0, 0, 1, 0, 0}; // x = 4
    std::vector<int> y = {0, 1, 0, 1, 1}; // y = 11
    int length = 31; // Длина последовательности

    // Генерация последовательности Голда
    std::vector<int> gold_sequence = generate_gold_sequence(x, y, length);

    // Вывод последовательности
    std::cout << "Последовательность Голда 1: ";
    for (int bit : gold_sequence) {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Таблица автокорреляции:\n";
    autocorrelation_table(gold_sequence);

    std::vector<int> x2 = {0, 0, 1, 0, 1}; // x = 5
    std::vector<int> y2 = {0, 0, 1, 1, 0}; // y = 11 - 5 = 6

    std::cout << std::endl;
    std::vector<int> second_gold_sequence = generate_gold_sequence(x2,y2,length);
        std::cout << "Последовательность Голда 2: ";
    for (int bit : second_gold_sequence) {
        std::cout << bit << " ";
    }
    std::cout << std::endl;
    // Расчет взаимной корреляции между исходной и новой последовательностью
    double mutual_correlation = calculate_correlation(gold_sequence, second_gold_sequence);
    std::cout << "Взаимная корреляция исходной и новой последовательностей: " << mutual_correlation << "\n";


    return 0;
}
