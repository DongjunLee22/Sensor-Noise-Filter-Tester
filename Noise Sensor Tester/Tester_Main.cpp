#include <SFML/Graphics.hpp>
#include "OscopeCtrl.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"
#include "PassFilters.h"
#include "Filtering.h"

#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;
using namespace std::chrono;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

enum FilterType {
    None,
    MovingAverage,
    LowPass,
    HighPass,
    BandPass,
    Kalman
};

// 센서 데이터 저장 함수
void saveDataToFile(const std::string& filename, const std::vector<std::tuple<float, float, float>>& data) {
    std::ofstream fileout(filename);

    if (!fileout.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    fileout << "time(ms)\tOriginalData\tFilteredData\n";

    for (const auto& entry : data) {
        float time, original, filtered;
        std::tie(time, original, filtered) = entry;
        fileout << time << "\t" << original << "\t" << filtered << "\n";
    }
}


int main() {
    printf("-------------------- 24.02.17 Made by LDJ --------------------\n");

    printf("This is a test code that receives sensor data containing random noise.\n");
    printf("This code was created to test various filters for noise removal.\n");
    printf("Currently, the Moving average filter has been applied to this code.\n\n");
    printf("WindowSize is 50\n\n");

    printf("The filtered graph is created after collecting 100 pieces of original data\n\n");

    printf("-------------------- Keyboard input guide --------------------\n");

    printf("D : Sensor data Input Start \n");
    printf("A : Sensor data Input Stop\n\n");

    printf("W : Sensor data amplitude increase ( +0.5 )\n");
    printf("S : Sensor data amplitude decrease ( -0.5 )\n");
    printf("E : Sensor data amplitude make zero \n\n");

    printf("I : Noise amplitude increase ( +3 )\n");
    printf("K : Noise amplitude decrease ( -3) \n");
    printf("O : Noise amplitude make zero\n\n");

    printf("F : Program Shut Down!!!\n\n");

    printf("----------------------- Filter setting -----------------------\n");
    printf("0 : None apply the filter\n");
    printf("1 : Apply the Moving Average Filter\n");
    printf("2 : Apply the Low-Pass Filter\n\n");
    printf("3 : Apply the High-Pass Filter\n\n");
    printf("4 : Apply the Band-Pass Filter\n\n");
    printf("5 : Apply the Kalman Filter\n\n");

    printf("--------------------------------------------------------------\n");



    sf::RenderWindow window(sf::VideoMode(1500, 800), "Real-time Graph with Noise Filter");
    window.setFramerateLimit(5000);

    OscopeCtrl oscilloscope(1500, 800); // 그래프의 너비와 높이 설정

    system_clock::time_point ts, t_start;
    chrono::nanoseconds t_duration_ns, t_stamp_ns;
    double t_duration_ns_double = 0.0;
    float t_stamp_ns_float = 0.0;
    
    // 데이터 저장 컨테이너
    std::vector<std::tuple<float, float, float>> dataStorage;

    // 필터 관련 변수
    std::vector<sf::Vertex> rawPoints;  // 원본 데이터의 그래프를 위한 벡터
    std::vector<sf::Vertex> points;     // 필터링 된 데이터 그래프를 위한 벡터

    std::deque<float> filterSamples;    // 이동 평균 필터를 위한 샘플 저장
    int filterWindowSize = 50;          // 이동 평균 필터의 윈도우 크기
    
    FilterType filtertype = None;       // 필터 타입

    
    MovingAverageFilter filter(filterWindowSize);   // 윈도우 사이즈만큼 moving average filter를 적용하는 함수
    
    std::vector<double> filterTaps;
    int N = 0;                                      // 필터 탭의 길이
    double fl = 0.1, fh = 0.3, fs = 1.0;            // Pass 필터의 파라미터
    double db = 5.0;

    // 센서 신호 관련 변수
    float time = 0.0f;
    float amplitude = 0.0f;             // 초기 진폭 설정
    float noise_amplitude = 4.0f;       // 초기 노이즈 크기
    bool isCollecting = false;          // 데이터 수집 시작 여부
    bool filterEnabled = false;         // 필터링 활성화 여부  

    srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())); // 노이즈 생성 초기화

    auto lastTimePoint = high_resolution_clock::now();  // 마지막 데이터 포인트 생성 시간
    double dataPointsPerSecond = 0.0;   // 초당 데이터 포인트 수


    while (window.isOpen())
    {
        ts = system_clock::now();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle key input
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {
                    isCollecting = true;                // 센서 데이터 입력 시작
                    t_start = system_clock::now();
                }
                else if (event.key.code == sf::Keyboard::A) {
                    isCollecting = false;               // 센서 데이터 입력 중지
                }

                else if (event.key.code == sf::Keyboard::W) {
                    amplitude += 0.5;                   // 데이터 진폭 증가
                }
                else if (event.key.code == sf::Keyboard::S && amplitude > 0) {
                    amplitude -= 0.5;                   // 데이터 진폭 감소
                }
                else if (event.key.code == sf::Keyboard::E) {
                    amplitude = 0.0;
                }

                else if (event.key.code == sf::Keyboard::F) {
                    window.close();                     // 프로그램 종료
                }

                else if (event.key.code == sf::Keyboard::I) {
                    noise_amplitude += 1.0f;            // 노이즈 증가
                }
                else if (event.key.code == sf::Keyboard::K) {
                    if (noise_amplitude > 0.0f) {
                        noise_amplitude -= 1.0f;        // 노이즈 감소
                    }
                    noise_amplitude = std::max(0.0f, noise_amplitude);
                }
                else if (event.key.code == sf::Keyboard::O) {
                    noise_amplitude = 0.0f;             // 원본 데이터의 노이즈 강제 제거 (0으로 변환)
                }

                if (event.key.code == sf::Keyboard::Num0) {
                    filtertype = None;  // 필터 적용 X
                }
                else if (event.key.code == sf::Keyboard::Num1) {
                    filtertype = MovingAverage; // Moving Average Filter  
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    filtertype = LowPass;    // Low-Pass Filter
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    filtertype = HighPass;    // High-Pass Filter
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    filtertype = BandPass;    // Band-Pass Filter
                    N = estimateFilterLen(fl, fh, fs, db);
                    filterTaps = BandPassFilter::generateTaps(fl, fh, fs, N);
                }
                else if (event.key.code == sf::Keyboard::Num5) {
                    filtertype = Kalman;    // Kalman Filter
                }                  
            }
        }
               
        if (isCollecting)
        {            
            auto currentTime = high_resolution_clock::now();
            auto timeDiff = duration_cast<microseconds>(currentTime - lastTimePoint).count(); // 마이크로초 단위로 시간 차이 계산
            lastTimePoint = currentTime; // 마지막 데이터 포인트 생성 시간 업데이트

            // 시간 차이가 0보다 클 때만 계산
            if (timeDiff > 0)
            {
                dataPointsPerSecond = 1e6 / static_cast<double>(timeDiff); // 초당 데이터 포인트 수 계산 (1e6은 마이크로초를 초로 변환하는 계수)
                std::cout << "Data Points Per Second: " << dataPointsPerSecond << " Hz" << std::endl; // 콘솔에 출력
            }

            // 시간 측정
            t_stamp_ns = system_clock::now() - t_start;
            t_stamp_ns_float = float(t_stamp_ns.count());
            float t_stamp_ms_float = t_stamp_ns_float / 1000000.0;

            // 센서 신호
            float noise = (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * noise_amplitude;
            float originalValue = amplitude * std::sin(2 * M_PI * time) + noise;
            float filteredValue;

            switch (filtertype) {
            case None: {
                filteredValue = 0;
                break;
            }
            case MovingAverage: {
                filteredValue = filter.apply(originalValue);
                break;
            }
            case LowPass: {
                filteredValue = 0;
                break;
            }
            case HighPass: {
                filteredValue = 0;
                break;
            }
            case BandPass: {
                std::vector<double> singleSampleAsDouble = { static_cast<double>(originalValue) };
                //std::vector<double> filteredSamples = filtering(singleSampleAsDouble, filterTaps);

                filteredValue = 0;
                break;
            }
            case Kalman: {
                KalmanFilter kf(5.0, 10.0); // 초기 추정값과 오차 공분산 설정
                kf.update(originalValue, 0.01, 0.1); // 측정값, 프로세스 노이즈, 측정 노이즈 업데이트
                filteredValue = kf.x_esti;
                break;
            }
            default: {
                filteredValue = 0;
                break;
            }
            }

            oscilloscope.addDataPoint(originalValue); // OscopeCtrl에 데이터 포인트 추가
            oscilloscope.addFilteredDataPoint(filteredValue);
            
            // 센서 데이터 저장
            dataStorage.push_back(make_tuple(t_stamp_ms_float, originalValue, filteredValue));

            time += 0.001;
        }

        window.clear();
        oscilloscope.draw(window); // OscopeCtrl을 사용하여 그래프 그리기
        window.display();



        //////////////////////////////////////////////////////////////////////////
        ////while문 속도 제어+++++++++++++++++++++++++++++++++++++++++++++++++++++
        //////////////////////////////////////////////////////////////////////////

        t_duration_ns_double = 0.0;
        while (t_duration_ns_double < 1000000)
        {
            t_duration_ns = system_clock::now() - ts;
            t_duration_ns_double = double(t_duration_ns.count());
        }
        //////////////////////////////////////////////////////////////////////////
     
    }

    // 데이터 저장
    saveDataToFile("data_rec_Ccode.txt", dataStorage);


    return 0;
}
