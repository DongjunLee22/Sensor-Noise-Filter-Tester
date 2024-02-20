// FilterAnalysis.h

#ifndef FILTER_ANALYSIS_H
#define FILTER_ANALYSIS_H

#include <vector>
#include <tuple>

// 필터 성능 분석을 위한 함수 프로토타입 선언
float calculateMSE(const std::vector<std::tuple<float, float, float>>& data);

#endif // FILTER_ANALYSIS_H
