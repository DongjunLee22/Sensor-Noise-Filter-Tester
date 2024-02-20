// EstimateFilterLen.cpp
#include "EstimateFilterLen.h"

int estimateFilterLen(double fl, double fh, double fs, double db) {
    int N = static_cast<int>(std::round(db * fs / (22 * (fh - fl))) - 1);
    return N;
}
