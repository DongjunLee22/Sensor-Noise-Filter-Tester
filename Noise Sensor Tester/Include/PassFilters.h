#pragma once
// PassFilters.h
#ifndef PASSFILTERS_H
#define PASSFILTERS_H

#include <vector>
#include <cmath>
#include <numeric>
#include "EstimateFilterLen.h"

class LowPassFilter {
public:
    static std::vector<double> generateTaps(double f, double fs, int N);
};

class HighPassFilter {
public:
    static std::vector<double> generateTaps(double f, double fs, int N);
};

class BandPassFilter {
public:
    static std::vector<double> generateTaps(double fl, double fh, double fs, int N);
};

#endif // FILTERS_H

