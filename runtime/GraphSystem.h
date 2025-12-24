#ifndef MYPROJECT_GRAPHSYSTEM_H
#define MYPROJECT_GRAPHSYSTEM_H
#pragma once
#include "Visualizer.h"
#include "DataStructure.h"
#include <vector>
class GraphSystem
{
private:
    Node nodes[MAX_Nodes];
    int nodeCount;
    MinHeap heap;
    UnionFind uf;
    std::vector<Edge>  visualEdges;
    void updateEdgeState(int u,int v,int state);
public:
    GraphSystem();
    void initRandom(int n);
    void initManual();
    bool initFromFile(const char* file);
    // Kruskal算法主流程
    void runKruskal(sf::RenderWindow& window,Visualizer& visualizer);
    //初始画面数据绘制
    void drawInitialScene(Visualizer& visualizer);

};

#endif
