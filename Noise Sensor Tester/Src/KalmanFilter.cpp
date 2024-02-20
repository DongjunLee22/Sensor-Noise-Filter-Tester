// KalmanFilter.cpp
#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float initial_x, float initial_P)
	: x_esti(initial_x), P(initial_P) {}

void KalmanFilter::update(float z_meas, float Q, float R, float A, float H) {
	float x_pred = A * x_esti;
	float P_pred = A * P * A + Q;
	float K = P_pred * H / (H * P_pred * H + R);
	x_esti = x_pred + K * (z_meas - H * x_pred);
	P = P_pred - K * H * P_pred;
}

