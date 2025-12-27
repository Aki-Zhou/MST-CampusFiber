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
    // 等待用户输入并处理窗口事件
    void waitForInput(sf::RenderWindow& window, Visualizer& visualizer, int totalCost);
    int totalCost; // 存储当前的最小生成树总开销
    std::vector<std::string> logs;
    bool isCompleted;
public:
    GraphSystem();
    void initRandom(int n);
    void initManual();
    bool initFromFile(const char* file);
    // Kruskal算法主流程
    void runKruskal(sf::RenderWindow& window,Visualizer& visualizer);
    //初始画面数据绘制
    void drawInitialScene(Visualizer& visualizer);
    // 重绘当前场景（用于窗口调整大小等情况）
    void redraw(Visualizer& visualizer);
};

#endif
