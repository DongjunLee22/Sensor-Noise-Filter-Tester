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

// ���� ������ ���� �Լ�
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

    OscopeCtrl oscilloscope(1500, 800); // �׷����� �ʺ�� ���� ����

    system_clock::time_point ts, t_start;
    chrono::nanoseconds t_duration_ns, t_stamp_ns;
    double t_duration_ns_double = 0.0;
    float t_stamp_ns_float = 0.0;
    
    // ������ ���� �����̳�
    std::vector<std::tuple<float, float, float>> dataStorage;

    // ���� ���� ����
    std::vector<sf::Vertex> rawPoints;  // ���� �������� �׷����� ���� ����
    std::vector<sf::Vertex> points;     // ���͸� �� ������ �׷����� ���� ����

    std::deque<float> filterSamples;    // �̵� ��� ���͸� ���� ���� ����
    int filterWindowSize = 50;          // �̵� ��� ������ ������ ũ��
    
    FilterType filtertype = None;       // ���� Ÿ��

    
    MovingAverageFilter filter(filterWindowSize);   // ������ �����ŭ moving average filter�� �����ϴ� �Լ�
    
    std::vector<double> filterTaps;
    int N = 0;                                      // ���� ���� ����
    double fl = 0.1, fh = 0.3, fs = 1.0;            // Pass ������ �Ķ����
    double db = 5.0;

    // ���� ��ȣ ���� ����
    float time = 0.0f;
    float amplitude = 0.0f;             // �ʱ� ���� ����
    float noise_amplitude = 4.0f;       // �ʱ� ������ ũ��
    bool isCollecting = false;          // ������ ���� ���� ����
    bool filterEnabled = false;         // ���͸� Ȱ��ȭ ����  

    srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())); // ������ ���� �ʱ�ȭ

    auto lastTimePoint = high_resolution_clock::now();  // ������ ������ ����Ʈ ���� �ð�
    double dataPointsPerSecond = 0.0;   // �ʴ� ������ ����Ʈ ��


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
                    isCollecting = true;                // ���� ������ �Է� ����
                    t_start = system_clock::now();
                }
                else if (event.key.code == sf::Keyboard::A) {
                    isCollecting = false;               // ���� ������ �Է� ����
                }

                else if (event.key.code == sf::Keyboard::W) {
                    amplitude += 0.5;                   // ������ ���� ����
                }
                else if (event.key.code == sf::Keyboard::S && amplitude > 0) {
                    amplitude -= 0.5;                   // ������ ���� ����
                }
                else if (event.key.code == sf::Keyboard::E) {
                    amplitude = 0.0;
                }

                else if (event.key.code == sf::Keyboard::F) {
                    window.close();                     // ���α׷� ����
                }

                else if (event.key.code == sf::Keyboard::I) {
                    noise_amplitude += 1.0f;            // ������ ����
                }
                else if (event.key.code == sf::Keyboard::K) {
                    if (noise_amplitude > 0.0f) {
                        noise_amplitude -= 1.0f;        // ������ ����
                    }
                    noise_amplitude = std::max(0.0f, noise_amplitude);
                }
                else if (event.key.code == sf::Keyboard::O) {
                    noise_amplitude = 0.0f;             // ���� �������� ������ ���� ���� (0���� ��ȯ)
                }

                if (event.key.code == sf::Keyboard::Num0) {
                    filtertype = None;  // ���� ���� X
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
            auto timeDiff = duration_cast<microseconds>(currentTime - lastTimePoint).count(); // ����ũ���� ������ �ð� ���� ���
            lastTimePoint = currentTime; // ������ ������ ����Ʈ ���� �ð� ������Ʈ

            // �ð� ���̰� 0���� Ŭ ���� ���
            if (timeDiff > 0)
            {
                dataPointsPerSecond = 1e6 / static_cast<double>(timeDiff); // �ʴ� ������ ����Ʈ �� ��� (1e6�� ����ũ���ʸ� �ʷ� ��ȯ�ϴ� ���)
                std::cout << "Data Points Per Second: " << dataPointsPerSecond << " Hz" << std::endl; // �ֿܼ� ���
            }

            // �ð� ����
            t_stamp_ns = system_clock::now() - t_start;
            t_stamp_ns_float = float(t_stamp_ns.count());
            float t_stamp_ms_float = t_stamp_ns_float / 1000000.0;

            // ���� ��ȣ
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
                KalmanFilter kf(5.0, 10.0); // �ʱ� �������� ���� ���л� ����
                kf.update(originalValue, 0.01, 0.1); // ������, ���μ��� ������, ���� ������ ������Ʈ
                filteredValue = kf.x_esti;
                break;
            }
            default: {
                filteredValue = 0;
                break;
            }
            }

            oscilloscope.addDataPoint(originalValue); // OscopeCtrl�� ������ ����Ʈ �߰�
            oscilloscope.addFilteredDataPoint(filteredValue);
            
            // ���� ������ ����
            dataStorage.push_back(make_tuple(t_stamp_ms_float, originalValue, filteredValue));

            time += 0.001;
        }

        window.clear();
        oscilloscope.draw(window); // OscopeCtrl�� ����Ͽ� �׷��� �׸���
        window.display();



        //////////////////////////////////////////////////////////////////////////
        ////while�� �ӵ� ����+++++++++++++++++++++++++++++++++++++++++++++++++++++
        //////////////////////////////////////////////////////////////////////////

        t_duration_ns_double = 0.0;
        while (t_duration_ns_double < 1000000)
        {
            t_duration_ns = system_clock::now() - ts;
            t_duration_ns_double = double(t_duration_ns.count());
        }
        //////////////////////////////////////////////////////////////////////////
     
    }

    // ������ ����
    saveDataToFile("data_rec_Ccode.txt", dataStorage);


    return 0;
}
