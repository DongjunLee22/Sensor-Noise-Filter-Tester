#pragma once
// MovingAverageFilter.h
#ifndef MOVINGAVERAGEFILTER_H
#define MOVINGAVERAGEFILTER_H

#include <deque>
#include <numeric>

class MovingAverageFilter {
public:
    MovingAverageFilter(int windowSize);
    float apply(float newValue);

private:
    std::deque<float> samples;
    int windowSize;
};

#endif // MOVINGAVERAGEFILTER_H

