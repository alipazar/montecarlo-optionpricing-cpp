#include <iostream>
#include <cmath>
#include <random>
#include <vector>

// Rastgele say� 
std::mt19937 generator(std::random_device{}());
std::normal_distribution<double> distribution(0.0, 1.0);

// Normal da��l�ma sahip rastgele say�lar �reten fonksiyon
double generateGaussianNoise() {
    return distribution(generator);
}

// Avrupa tipi al�m opsiyonunun getirisini hesaplayan fonksiyon
inline double callOptionPayoff(double S, double K) {
    return std::max(S - K, 0.0);
}

// Avrupa tipi sat�m opsiyonunun getirisini hesaplayan fonksiyon
inline double putOptionPayoff(double S, double K) {
    return std::max(K - S, 0.0);
}

// Avrupa opsiyon fiyatlamas� i�in Monte Carlo Sim�lasyonu
double monteCarloOptionPricing(const double S0, const double K, const double r,
    const double sigma, const double T,
    const int numSimulations, const bool isCallOption) {
    double payoffSum = 0.0;

    // Sabitleri �nceden hesaplama
    const double drift = (r - 0.5 * sigma * sigma) * T;
    const double diffusion = sigma * std::sqrt(T);
    const double discountFactor = std::exp(-r * T);

    for (int i = 0; i < numSimulations; ++i) {
        // Rastgele bir fiyat path olu�tur
        double ST = S0 * std::exp(drift + diffusion * generateGaussianNoise());

        // Bu path i�in getiri hesapla
        double payoff = isCallOption ? callOptionPayoff(ST, K) : putOptionPayoff(ST, K);

		// Getiriyi topla
        payoffSum += payoff;
    }

    // Ortalama getiriyi hesapla ve bug�nk� de�ere indirge
    return discountFactor * (payoffSum / static_cast<double>(numSimulations));
}

int main() {
    // Option parameters
    const double S0 = 100.0;   // Initial stock price
    const double K = 100.0;    // Strike price
    const double r = 0.05;     // Risk-free rate
    const double sigma = 0.2;  // Volatility
    const double T = 1;        // Time to maturity (1 year)
    const int numSimulations = 100000; // Number of simulations

    // Hesapla option prices
    double callPrice = monteCarloOptionPricing(S0, K, r, sigma, T, numSimulations, true);
    double putPrice = monteCarloOptionPricing(S0, K, r, sigma, T, numSimulations, false);

    // Output the results
    std::cout << "European Call Option Price: " << callPrice << std::endl;
    std::cout << "European Put Option Price: " << putPrice << std::endl;

    return 0;
}
