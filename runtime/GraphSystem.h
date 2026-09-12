#ifndef MYPROJECT_GRAPHSYSTEM_H
#define MYPROJECT_GRAPHSYSTEM_H
#pragma once
#include "Visualizer.h"
#include "DataStructure.h"
#include <vector>

//图系统类，管理图的数据结构和算法逻辑
class GraphSystem
{
private:
    Node nodes[MAX_Nodes]; //节点数组
    int nodeCount;         //节点数量
    MinHeap heap;          //最小堆，用于存储边
    UnionFind uf;          //并查集，用于Kruskal算法
    std::vector<Edge>  visualEdges; //用于可视化的边列表

    //更新边的可视化状态
    void updateEdgeState(int u,int v,int state);

    //等待用户输入并处理窗口事件（如关闭、调整大小）
    void waitForInput(sf::RenderWindow& window, Visualizer& visualizer, int totalCost);

    int totalCost; //存储当前的最小生成树总开销
    std::vector<std::string> logs; //存储算法执行过程的日志
    bool isCompleted; //标记算法是否执行完毕

public:
    GraphSystem();

    //随机生成图数据
    void initRandom(int n);

    //手动输入图数据
    void initManual();

    //从文件加载图数据
    bool initFromFile(const char* file);

    //Kruskal算法主流程：执行最小生成树算法并驱动可视化
    void runKruskal(sf::RenderWindow& window,Visualizer& visualizer);

    //绘制初始画面
    void drawInitialScene(Visualizer& visualizer);

    //重绘当前场景（用于窗口调整大小等情况）
    void redraw(Visualizer& visualizer);
};

#endif
