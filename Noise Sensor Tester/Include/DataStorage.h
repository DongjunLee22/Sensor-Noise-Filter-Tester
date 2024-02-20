// DataStorage.h

#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <vector>
#include <tuple>
#include <string>

class DataStorage {
public:
    // 데이터 스토리지 컨테이너
    std::vector<std::tuple<float, float, float>> dataStorageNone;
    std::vector<std::tuple<float, float, float>> dataStorageMovingAverage;
    std::vector<std::tuple<float, float, float>> dataStorageLowPass;
    std::vector<std::tuple<float, float, float>> dataStorageHighPass;
    std::vector<std::tuple<float, float, float>> dataStorageBandPass;
    std::vector<std::tuple<float, float, float>> dataStorageKalman;

    // 데이터 저장 함수
    void saveFilteredData(const std::string& filename, const std::vector<std::tuple<float, float, float>>& dataStorage);
};

#endif // DATA_STORAGE_H