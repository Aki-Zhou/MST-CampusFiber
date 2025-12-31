#include "GraphSystem.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread> // sleep
GraphSystem::GraphSystem(): nodeCount(0), totalCost(0), isCompleted(false) {}

// 重绘函数，调用Visualizer绘制当前状态
void GraphSystem::redraw(Visualizer& visualizer)
{
    visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, isCompleted);
}

// 等待回车键继续下一步，同时处理窗口事件
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
        sf::sleep(sf::milliseconds(10)); // 避免CPU占用过高
    }
}

// 更新边的状态（颜色），用于可视化反馈
void GraphSystem::updateEdgeState(int u, int v, int state)
{
    for (auto& e : visualEdges)
    {
        // 找到对应的边（无向图，需要检查两个方向）
        if ((e.u == u && e.v == v) || (e.u == v && e.v == u))
        {
            e.state = state;
            break;
        }
    }
}

// 随机初始化图：生成n个节点和随机边
void GraphSystem::initRandom(int n)
{
    nodeCount = n;
    srand(time(nullptr)); // 设置随机数种子
    heap.init(); // 初始化堆
    uf.init(n);  // 初始化并查集
    visualEdges.clear();
    logs.clear();
    for (int i=0;i < n;++i)//生成节点
    {
        nodes[i].id = i;
        nodes[i].x = rand() % 700 + 100; // 随机X坐标 (100-750)
        nodes[i].y = rand() % 500 + 50; // 随机Y坐标 (50-550)
        sprintf(nodes[i].name,"Node %d",i);
    }
    // 生成完全图的边（每两个节点之间都有一条边）
    for (int i=0;i < n;++i)
    {
        for (int j=i + 1;j < n;++j)
        {
            int w= rand() % 100 + 10; // 随机权重
            Edge e = {i,j,w,0};
            heap.Push(e); // 加入最小堆，自动排序
            visualEdges.push_back(e); // 加入可视化列表，用于显示
        }
    }
}

// 手动输入图数据
void GraphSystem::initManual()
{
    heap.init();
    visualEdges.clear();
    logs.clear();

    // 输入节点数量
    while (true) {
        std::cout << "请输入节点个数 (2-20): " << std::endl;
        if (std::cin >> nodeCount && nodeCount >= 2 && nodeCount <= 20) break;
        std::cout << "输入无效，请输入 2 到 20 之间的整数。" << std::endl;
        std::cin.clear(); // 清除错误标志
        std::cin.ignore(10000, '\n'); // 忽略缓冲区内容
    }

    uf.init(nodeCount);

    // 输入节点信息
    std::cout << "请依次输入节点名称 X坐标 Y坐标: " << std::endl;
    for (int i=0;i < nodeCount;++i)
    {
        nodes[i].id = i;
        std::cin >> nodes[i].name >> nodes[i].x >> nodes[i].y;
    }
    int m;
    // 输入边数量
    while (true) {
        std::cout << "请输入边的数量: " << std::endl;
        if (std::cin >> m && m >= 0) break;
        std::cout << "输入无效，请输入非负整数。" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    // 输入边信息
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

// 从文件读取图数据
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

// 绘制初始场景
void GraphSystem::drawInitialScene(Visualizer& visualizer)
{
    visualizer.drawScene(nodeCount,nodes,0,visualEdges, logs, false);

}

// Kruskal算法主流程
void GraphSystem::runKruskal(sf::RenderWindow& window, Visualizer& visualizer) {
    int edgesCount = 0; // 已选择的边数
    totalCost = 0; // 使用成员变量记录总权重
    isCompleted = false;
    logs.clear();

    // 先刷新一下初始画面，防x止白屏
    visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);

    // 循环直到堆为空或已选出 n-1 条边
    while (!heap.isEmpty() && edgesCount < nodeCount - 1) {

        // 等待用户按回车继续
        waitForInput(window, visualizer, totalCost);

        // 如果等待期间窗口关了，就退出
        if(!window.isOpen()) break;

        Edge e = heap.Pop(); // 取出权重最小的边

        // 1. 变成黄色 (扫描中)
        updateEdgeState(e.u, e.v, 1);
        visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);

        // 这里保留一个短促的自动停顿 (0.3秒)，
        // 让你能感觉到“它正在思考”的过程，而不是瞬间变绿，视觉效果更好。
        sf::sleep(sf::milliseconds(300));

        // 2. 判断逻辑：使用并查集判断是否形成环
        if (uf.unite(e.u, e.v))
        {
            // 成功：不形成环，加入MST，变绿
            edgesCount++;
            totalCost += e.weight;
            updateEdgeState(e.u, e.v, 2);

            // 添加日志：记录连接成功的边
            char buffer[100];
            sprintf(buffer, "Connect: %s - %s (w:%d)", nodes[e.u].name, nodes[e.v].name, e.weight);
            logs.push_back(std::string(buffer));
        }
        else
        {
            // 失败：形成环，丢弃，变红
            updateEdgeState(e.u, e.v, 3);

            // 添加日志：记录形成环的边
            char buffer[100];
            sprintf(buffer, "Cycle: %s - %s (w:%d)", nodes[e.u].name, nodes[e.v].name, e.weight);//可能不安全，可改
            logs.push_back(std::string(buffer));

            visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);

            // 延长红色显示时间
            sf::sleep(sf::milliseconds(800));

            updateEdgeState(e.u, e.v, 0); // 变回灰色
        }

        visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, false);
    }

    isCompleted = true; // 标记算法完成
    visualizer.drawScene(nodeCount, nodes, totalCost, visualEdges, logs, true);

    // 结束后等待关闭，保持窗口显示结果
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

