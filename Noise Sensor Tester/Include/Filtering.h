#pragma once
// Filtering.h
#ifndef FILTERING_H
#define FILTERING_H

#include <vector>

// 필터링 함수 원형 선언
std::vector<float> filtering(const std::vector<float>& samples, const std::vector<float>& taps);

#endif // FILTERING_H

