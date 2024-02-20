// KalmanFilter.h
#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

class KalmanFilter {
public:
    float x_esti; // 추정값
    float P; // 오차 공분산

    KalmanFilter(float initial_x, float initial_P);
    void update(float z_meas, float Q, float R, float A = 1, float H = 1);
};

#endif // KALMAN_FILTER_H

