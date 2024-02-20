// FilterAnalysis.cpp

#include "FilterAnalysis.h"
#include <cmath>

// 평균 제곱 오차(MSE)를 계산하는 함수 구현
float calculateMSE(const std::vector<std::tuple<float, float, float>>& data) {
    float mse = 0.0f;
    size_t count = data.size();

    for (const auto& entry : data) {
        float original, filtered;
        std::tie(std::ignore, original, filtered) = entry;
        mse += std::pow(filtered - original, 2);
    }

    if (count > 0) mse /= count;
    return mse;
}
