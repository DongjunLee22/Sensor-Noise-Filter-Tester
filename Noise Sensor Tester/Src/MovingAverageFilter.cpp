// MovingAverageFilter.cpp
// 위상차에 대한 보상 코드 미적용 - 24.02.19
#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(int windowSize) : windowSize(windowSize) {}

float MovingAverageFilter::apply(float newValue) {
    if (samples.size() == windowSize) {
        samples.pop_front();
    }
    samples.push_back(newValue);

    float sum = std::accumulate(samples.begin(), samples.end(), 0.0f);
    return sum / static_cast<float>(samples.size());
}
