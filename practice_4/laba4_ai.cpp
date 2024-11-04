#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

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

// Вывод таблицы автокорреляции
void print_autocorrelation_table(const vector<int>& sequence) {
    int n = sequence.size();
    cout << "Сдвиг\tПоследовательность\tАвтокорреляция\n";
    for (int shift = 0; shift < n; shift++) {
        vector<int> shifted_seq = cyclic_shift(sequence, shift);
        double corr = calculateCorrelation(sequence, shifted_seq);

        cout << shift << "\t";
        for (int bit : shifted_seq) {
            cout << bit << " ";
        }
        cout << "\t" << corr << "\n";
    }
}

int main() {
    int length_sequence_gold = 31;
    vector<int> x = {0, 0, 1, 0, 0}; // x = 4 в 2СС
    vector<int> y = {0, 1, 1, 0, 1}; // y = x + 7 в 2СС

    vector<int> gold_sequence_1 = generate_sequence_gold(x, y, length_sequence_gold);

    cout << "Первая последовательность Голда: ";
    for (int bit : gold_sequence_1) {
        cout << bit;
    }
    cout << endl;

    print_autocorrelation_table(gold_sequence_1);

    vector<int> x2 = {0, 0, 1, 0, 1}; // x + 1 = 5 в 2СС
    vector<int> y2 = {0, 1, 0, 0, 0}; // y - 5 = 8 в 2СС
    vector<int> gold_sequence_2 = generate_sequence_gold(x2, y2, length_sequence_gold);

    cout << "Вторая последовательность Голда: ";
    for (int bit : gold_sequence_2) {
        cout << bit;
    }
    cout << endl;

    double cross_corr = calculateCorrelation(gold_sequence_1, gold_sequence_2);
    cout << "Взаимная корреляция между двумя последовательностями Голда: " << cross_corr << endl;

    return 0;
}
