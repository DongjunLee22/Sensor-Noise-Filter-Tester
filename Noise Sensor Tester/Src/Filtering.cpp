// Filtering.cpp
#include "Filtering.h"
#include <numeric> // std::accumulate를 사용하기 위해 포함

// 필터링 함수 구현
std::vector<double> filtering(const std::vector<double>& samples, const std::vector<double>& taps) {
    int N = taps.size();
    std::vector<double> out(samples.size(), 0.0);

    if (samples.size() <= static_cast<size_t>(N)) {
        return out;
    }

    std::vector<double> buffer(samples.size() + N, samples[0]);
    std::copy(samples.begin(), samples.end(), buffer.begin() + N);

    for (size_t i = 0; i < samples.size(); ++i) {
        out[i] = std::accumulate(buffer.begin() + i, buffer.begin() + i + N, 0.0,
            [&taps, &buffer, i](double acc, const double& b) {
                return acc + b * taps[&b - &buffer[0] - i];
            });
    }

    return std::vector<double>(out.begin() + N / 2, out.end());
}
