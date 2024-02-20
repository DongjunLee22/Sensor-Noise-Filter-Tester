// DataStorage.cpp

#include "DataStorage.h"
#include <fstream>
#include <iostream>

void DataStorage::saveFilteredData(const std::string& filename, const std::vector<std::tuple<float, float, float>>& dataStorage) {
    if (dataStorage.size() > 100) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filename << std::endl;
            return;
        }
        file << "time(ms)\tOriginalData\tFilteredData\n";
        for (const auto& entry : dataStorage) {
            float time, original, filtered;
            std::tie(time, original, filtered) = entry;
            file << time << "\t" << original << "\t" << filtered << "\n";
        }
    }
}
