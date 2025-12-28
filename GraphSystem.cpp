#include "GraphSystem.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread> // sleep
GraphSystem::GraphSystem(): nodeCount(0), totalCost(0), isCompleted(false) {}

void GraphSystem::redraw(Visualizer& visualizer) {
    visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, isCompleted);
}

void GraphSystem::waitForInput(sf::RenderWindow& window, Visualizer& visualizer, int currentCost) {
    bool pressed = false;
    while (window.isOpen() && !pressed)
        {
        sf::Event event;
        while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
            // 处理窗口大小调整
            if (event.type == sf::Event::Resized)
            {

                // 重绘
                visualizer.drawScene(nodeCount, nodes, currentCost, visualEdges, logs, false);
            }
            // 如果按下了键盘，并且按的是 Enter 键
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                pressed = true; // 退出等待循环
            }
        }
        sf::sleep(sf::milliseconds(10));
    }
}

void GraphSystem::updateEdgeState(int u, int v, int state)
{
    for (auto& e : visualEdges)
    {
        if ((e.u == u && e.v == v) || (e.u == v && e.v == u))
        {
            e.state = state;
            break;
        }
    }
}

void GraphSystem::initRandom(int n)
{
    nodeCount = n;
    srand(time(nullptr));
    heap.init();
    uf.init(n);
    visualEdges.clear();
    logs.clear();
    for (int i=0;i < n;++i)//生成节点
    {
        nodes[i].id = i;
        nodes[i].x = rand() % 700 +50;
        nodes[i].y = rand() % 500 + 50;
        sprintf(nodes[i].name,"Node %d",i);
    }
    for (int i=0;i < n;++i)
    {
        for (int j=i + 1;j < n;++j)
        {
            int w= rand() % 100 + 10;
            Edge e = {i,j,w,0};
            heap.Push(e);
            visualEdges.push_back(e);
        }
    }
}
void GraphSystem::initManual()
{
    heap.init();
    visualEdges.clear();
    logs.clear();

    while (true) {
        std::cout << "请输入节点个数 (2-20): " << std::endl;
        if (std::cin >> nodeCount && nodeCount >= 2 && nodeCount <= 20) break;
        std::cout << "输入无效，请输入 2 到 20 之间的整数。" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    uf.init(nodeCount);

    std::cout << "请依次输入节点名称 X坐标 Y坐标: " << std::endl;
    for (int i=0;i < nodeCount;++i)
    {
        nodes[i].id = i;
        std::cin >> nodes[i].name >> nodes[i].x >> nodes[i].y;
    }
    int m;
    while (true) {
        std::cout << "请输入边的数量: " << std::endl;
        if (std::cin >> m && m >= 0) break;
        std::cout << "输入无效，请输入非负整数。" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::cout << "请依次输入边的起点 终点 权重: " << std::endl;
    for (int i=0;i < m;++i)
    {
        int u,v,w;
        while (true) {
            std::cin >> u >> v >> w;
            if (!std::cin.fail() && u >= 0 && u < nodeCount && v >= 0 && v < nodeCount && w > 0) {
                break;
            }
            std::cout << "输入错误 (起点/终点应在 0-" << nodeCount-1 << " 之间，权重>0)，请重新输入: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        Edge e  = {u,v,w,0};
        heap.Push(e);
        visualEdges.push_back(e);
    }
}

bool GraphSystem::initFromFile(const char* file)
{
    heap.init();
    visualEdges.clear();
    logs.clear();
    FILE* fp = fopen(file,"r");
    if (fp == nullptr)
        return false;
    fscanf(fp,"%d",&nodeCount);
    uf.init(nodeCount);
    for (int i=0;i < nodeCount; ++i)
    {
        nodes[i].id = i;
        fscanf(fp,"%s %f %f",nodes[i].name,&nodes[i].x,&nodes[i].y);
    }

    int u,v,w;
    while (fscanf(fp,"%d %d %d",&u,&v,&w) != EOF)
    {
        Edge e = {u,v,w,0};
        heap.Push(e);
        visualEdges.push_back(e);
    }
    fclose(fp);
    return true;
}

void GraphSystem::drawInitialScene(Visualizer& visualizer)
{
    visualizer.drawScene(nodeCount,nodes,0,visualEdges, logs, false);

}
void GraphSystem::runKruskal(sf::RenderWindow& window, Visualizer& visualizer) {
    int edgesCount = 0;
    totalCost = 0; // 使用成员变量
    isCompleted = false;
    logs.clear();

    // 先刷新一下初始画面，防止白屏
    visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);

    while (!heap.isEmpty() && edgesCount < nodeCount - 1) {

        // 等待回车
        waitForInput(window, visualizer, totalCost);

        // 如果等待期间窗口关了，就退出
        if(!window.isOpen()) break;

        Edge e = heap.Pop();

        // 1. 变成黄色 (扫描中)
        updateEdgeState(e.u, e.v, 1);
        visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);

        // 这里保留一个短促的自动停顿 (0.3秒)，
        // 让你能感觉到“它正在思考”的过程，而不是瞬间变绿，视觉效果更好。
        sf::sleep(sf::milliseconds(300));

        // 2. 判断逻辑
        if (uf.unite(e.u, e.v))
        {
            // 成功：变绿
            edgesCount++;
            totalCost += e.weight;
            updateEdgeState(e.u, e.v, 2);

            // Add log
            char buffer[100];
            sprintf(buffer, "Connect: %s - %s (w:%d)", nodes[e.u].name, nodes[e.v].name, e.weight);
            logs.push_back(std::string(buffer));
        }
        else
        {
            // 失败：变红
            updateEdgeState(e.u, e.v, 3);

            // Add log
            char buffer[100];
            sprintf(buffer, "Cycle: %s - %s (w:%d)", nodes[e.u].name, nodes[e.v].name, e.weight);
            logs.push_back(std::string(buffer));

            visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);

            // 延长红色显示时间，让用户能看清
            sf::sleep(sf::milliseconds(800));

            updateEdgeState(e.u, e.v, 0); // 变回灰色
        }

        visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);
    }

    isCompleted = true;
    visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, true);

    // 结束后等待关闭
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}

