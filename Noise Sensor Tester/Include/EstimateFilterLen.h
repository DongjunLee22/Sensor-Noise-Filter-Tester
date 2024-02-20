#pragma once
// EstimateFilterLen.h
#ifndef ESTIMATEFILTERLEN_H
#define ESTIMATEFILTERLEN_H

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int estimateFilterLen(double fl, double fh, double fs, double db);

#endif // ESTIMATEFILTERLEN_H

