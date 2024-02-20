// FilterAnalysis.cpp

#include "FilterAnalysis.h"
#include <cmath>

// ��� ���� ����(MSE)�� ����ϴ� �Լ� ����
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
