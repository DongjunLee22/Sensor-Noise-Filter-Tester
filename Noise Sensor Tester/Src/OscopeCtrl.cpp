// OscopeCtrl.cpp
#include "OscopeCtrl.h"

OscopeCtrl::OscopeCtrl(float width, float height) : graphWidth(width), graphHeight(height) {}

void OscopeCtrl::addDataPoint(float value) {
    if (dataPoints.size() >= maxPoints) {
        dataPoints.pop_front();
    }
    dataPoints.push_back(value);
    updateGraph();
}

void OscopeCtrl::addFilteredDataPoint(float value) {
    if (filteredDataPoints.size() >= maxPoints) {
        filteredDataPoints.pop_front();
    }
    filteredDataPoints.push_back(value);
    updateFilteredGraph();
}

void OscopeCtrl::draw(sf::RenderWindow& window) {
    if (!graph.empty()) {
        window.draw(&graph[0], graph.size(), sf::LinesStrip);
    }
    if (!filteredGraph.empty()) {
        window.draw(&filteredGraph[0], filteredGraph.size(), sf::LinesStrip, sf::BlendAlpha);
    }
}

void OscopeCtrl::updateGraph() {
    graph.clear();
    float xIncrement = graphWidth / maxPoints;
    float xOffset = graphWidth - dataPoints.size() * xIncrement;
    float centerY = graphHeight / 2;

    for (size_t i = 0; i < dataPoints.size(); ++i) {
        float x = xOffset + i * xIncrement;
        float y = centerY - dataPoints[i] * 50; // 데이터 포인트에 적용된 스케일링 팩터
        graph.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::White));
    }
}

void OscopeCtrl::updateFilteredGraph() {
    filteredGraph.clear();
    float xIncrement = graphWidth / maxPoints;
    float xOffset = graphWidth - filteredDataPoints.size() * xIncrement;
    float centerY = graphHeight / 2;

    for (size_t i = 0; i < filteredDataPoints.size(); ++i) {
        float x = xOffset + i * xIncrement;
        float y = centerY - filteredDataPoints[i] * 50; // 필터링된 데이터 포인트에 적용된 스케일링 팩터
        filteredGraph.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::Red));
    }
}
