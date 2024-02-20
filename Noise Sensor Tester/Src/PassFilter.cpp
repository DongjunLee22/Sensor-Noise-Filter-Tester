// PassFilter.cpp
#include "PassFilters.h"

std::vector<double> LowPassFilter::generateTaps(double f, double fs, int N) {
    std::vector<double> taps(N, 0.0);
    double fc = f / fs;
    double omega = 2 * M_PI * fc;

    int middle = N / 2;
    for (int i = 0; i < N; ++i) {
        if (i == middle) {
            taps[i] = 2 * fc;
        }
        else {
            taps[i] = std::sin(omega * (i - middle)) / (M_PI * (i - middle));
        }
    }
    return taps;
}

std::vector<double> HighPassFilter::generateTaps(double f, double fs, int N) {
    std::vector<double> taps(N, 0.0);
    double fc = f / fs;
    double omega = 2 * M_PI * fc;

    int middle = N / 2;
    for (int i = 0; i < N; ++i) {
        if (i == middle) {
            taps[i] = 1 - 2 * fc;
        }
        else {
            taps[i] = -std::sin(omega * (i - middle)) / (M_PI * (i - middle));
        }
    }
    return taps;

}

std::vector<double> BandPassFilter::generateTaps(double fl, double fh, double fs, int N) {
    std::vector<double> taps(N, 0.0);
    double flc = fl / fs;
    double fhc = fh / fs;
    double omegal = 2 * M_PI * flc;
    double omegah = 2 * M_PI * fhc;

    int middle = N / 2;
    for (int i = 0; i < N; ++i) {
        if (i == middle) {
            taps[i] = 2 * fhc - 2 * flc;
        }
        else {
            taps[i] = std::sin(omegah * (i - middle)) / (M_PI * (i - middle)) - std::sin(omegal * (i - middle)) / (M_PI * (i - middle));
        }
    }
    return taps;
}
