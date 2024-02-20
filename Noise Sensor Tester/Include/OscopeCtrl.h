// OscopeCtrl.h
#ifndef OSCOPECTRL_H
#define OSCOPECTRL_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

class OscopeCtrl {
public:
    OscopeCtrl(float width, float height);
    void addDataPoint(float value);
    void addFilteredDataPoint(float value);
    void draw(sf::RenderWindow& window);

private:
    std::deque<float> dataPoints;
    std::deque<float> filteredDataPoints;
    std::vector<sf::Vertex> graph;
    std::vector<sf::Vertex> filteredGraph;
    float graphWidth, graphHeight;
    size_t maxPoints = 300;

    void updateGraph();
    void updateFilteredGraph();
};

#endif // OSCOPECTRL_H

