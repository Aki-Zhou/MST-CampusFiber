#ifndef MYPROJECT_VISUALIZER_H
#define MYPROJECT_VISUALIZER_H
#pragma once
#include "DataStructure.h"
#include <SFML/Graphics.hpp>
#include <vector>
class Visualizer
{
    sf::RenderWindow *window;
    sf::Font font;
public:
    Visualizer(sf::RenderWindow *win);
    bool loadFont(const char* filename);
    void drawScene(int nodeCount, Node* nodes,int totalCost,std::vector<Edge>& edges, bool isCompleted = false) const;
};
#endif
