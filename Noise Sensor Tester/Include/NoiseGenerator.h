// NoiseGenerator.h
#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

class NoiseGenerator {
public:
    // 가우시안 노이즈 생성 함수
    static float generateGaussianNoise(float mean, float stddev);

    // 사인파 노이즈 생성 함수
    static float generateSineWaveNoise(float amplitude, float frequency, float time);
};

#endif // NOISE_GENERATOR_H
