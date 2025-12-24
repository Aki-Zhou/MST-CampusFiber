#include <iostream>
#include <SFML/Graphics.hpp>
#include "GraphSystem.h"
#include "Visualizer.h"
using namespace std;
int main()
{
    GraphSystem System;
    cout << "=== 最小生成树的校园光缆铺设 ===" << endl;
    cout << "1. 随机生成图" << endl;
    cout << "2. 手动输入图" << endl;
    cout << "3. 从文件加载图" << endl;
    cout << "请选择初始化方式: " << endl;
    int choice;
    cin >> choice;
    if (choice == 1)
        System.initRandom(7);
    else if (choice == 2)
        System.initManual();
    else if (choice == 3)
    {
        if (!System.initFromFile("data.txt")) {
            std::cout << "File error!\n";
            system("pause");
            return 0;
        }
    }
    //创建窗口
    sf::RenderWindow window(sf::VideoMode(800,600),"Kruskal最小生成树算法可视化");
    window.setFramerateLimit(60);
    //实例化
    Visualizer visualizer(&window);
    if (!visualizer.loadFont("font.ttf"))
    {
        cout << "字体加载失败!" << endl;
        system("pause");
        return -1;
    }
    //绘制初始场景
    System.drawInitialScene(visualizer);
    sf::sleep(sf::seconds(1));
    System.runKruskal(window,visualizer);
    //保持窗口
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
            {
                // 1. 获取新的窗口大小
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);

                // 2. 更新 View 以匹配新的窗口尺寸
                window.setView(sf::View(visibleArea));
            }
        }
    }
    return 0;
}