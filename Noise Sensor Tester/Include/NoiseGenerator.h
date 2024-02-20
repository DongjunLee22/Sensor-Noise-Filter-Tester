// NoiseGenerator.h
#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

class NoiseGenerator {
public:
    // ����þ� ������ ���� �Լ�
    static float generateGaussianNoise(float mean, float stddev);

    // ������ ������ ���� �Լ�
    static float generateSineWaveNoise(float amplitude, float frequency, float time);
};

#endif // NOISE_GENERATOR_H
