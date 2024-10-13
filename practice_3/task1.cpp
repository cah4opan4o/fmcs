#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double calculateCorrelation(const vector<double>& x, const vector<double>& y) {
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

double calculateNormalizedCorrelation(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.empty()) {
        throw invalid_argument("Vectors must be of the same size and non-empty");
    }

    double sumXY = 0.0;
    double sumX2 = 0.0;
    double sumY2 = 0.0;

    for (size_t i = 0; i < x.size(); i++) {
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
        sumY2 += y[i] * y[i];
    }
    // Вычисление нормированной корреляции
    double correlation = sumXY / (sqrt(sumX2) * sqrt(sumY2));

    return correlation;
}

int main() {
    vector<double> a = { 6, 2, 8, -2, -4, -4, 1, 3 };
    vector<double> b = { 3, 6, 7, 0, -5, -4, 2, 5 };
    vector<double> c = { -1, -1, 3, -9, 2, -8, 4, -1 };
    
    try {
        double correlationAB = calculateCorrelation(a, b);
        double correlationAC = calculateCorrelation(a, c);
        double correlationBC = calculateCorrelation(b, c);
        
        cout << "Матрица корреляций" << endl;
        cout << "\\ | a        | b        | c        |" << endl;
        cout << "a | -        | " << correlationAB << " | "<< correlationAC << " |"<< endl;
        cout << "b | " << correlationAB << " | -        | "<< correlationBC << " |"<<endl;
        cout << "c | " << correlationAC << " | " << correlationBC << " | -        |" << "\n" << endl;

// \| a | b | c
// a | - | 11 | 23
// b | 11 | - | 4
// c | 23 | 4 | -

        double normalizedCorrelation_AB = calculateNormalizedCorrelation(a, b);
        double normalizedCorrelation_AC = calculateNormalizedCorrelation(a, c);
        double normalizedCorrelation_BC = calculateNormalizedCorrelation(b, c);
        
        cout << "Матрица Нормализованной корреляции" << endl;
        cout << "\\ | a        | b        | c        |" << endl;
        cout << "a | -        | " << normalizedCorrelation_AB << " | "<< normalizedCorrelation_AC << " |"<< endl;
        cout << "b | " << normalizedCorrelation_AB << " | -        | "<< normalizedCorrelation_BC << " |"<<endl;
        cout << "c | " << normalizedCorrelation_AC << " | " << normalizedCorrelation_BC << " | -        |" << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
