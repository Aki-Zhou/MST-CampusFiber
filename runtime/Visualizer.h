#ifndef MYPROJECT_VISUALIZER_H
#define MYPROJECT_VISUALIZER_H
#pragma once
#include "DataStructure.h"
#include <SFML/Graphics.hpp>
#include <vector>

//可视化类，负责处理所有图形绘制工作
class Visualizer
{
    sf::RenderWindow *window; //指向SFML渲染窗口的指针
    sf::Font font;            //字体对象
public:
    Visualizer(sf::RenderWindow *win);

    //加载字体文件
    bool loadFont(const char* filename);

    //绘制整个场景，包括节点、边、状态栏和日志
    //nodeCount: 节点数量
    //nodes: 节点数组
    //totalCost: 当前最小生成树总花费
    //edges: 边集合（包含状态信息）
    //logs: 日志信息列表
    //isCompleted: 是否完成标志
    void drawScene(int nodeCount, Node* nodes,int totalCost,std::vector<Edge>& edges, const std::vector<std::string>& logs, bool isCompleted = false) const;
};
#endif
