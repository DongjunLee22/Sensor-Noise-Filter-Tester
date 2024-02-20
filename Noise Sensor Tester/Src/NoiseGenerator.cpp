// NoiseGenerator.cpp
#include "NoiseGenerator.h"
#include <cmath>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float NoiseGenerator::generateGaussianNoise(float mean, float stddev) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mean, stddev);

    return dist(gen);
}

float NoiseGenerator::generateSineWaveNoise(float amplitude, float frequency, float time) {
    return amplitude * std::sin(2 * M_PI * frequency * time);
}
