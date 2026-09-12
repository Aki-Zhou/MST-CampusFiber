#include <iostream>
#include <SFML/Graphics.hpp>
#include "GraphSystem.h"
#include "Visualizer.h"
using namespace std;

//主函数：程序入口
int main()
{
    GraphSystem System;

    //显示菜单
    cout << "=== 最小生成树的校园光缆铺设 ===" << endl;
    cout << "1. 随机生成图" << endl;
    cout << "2. 手动输入图" << endl;
    cout << "3. 从文件加载图" << endl;
    cout << "请选择输入方式: " << endl;

    int choice;
    //输入验证循环
    while (true)
        {
        if (cin >> choice)
            {
            if (choice >= 1 && choice <= 3) break;
            cout << "无效选择，请输入 1-3: ";
        }
        else
        {
            cout << "输入错误，请输入数字: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    //根据选择初始化图
    if (choice == 1)
        System.initRandom(7); //随机生成7个节点
    else if (choice == 2)
        System.initManual(); //手动输入
    else if (choice == 3)
    {
        if (!System.initFromFile("data.txt")) { //从文件读取
            std::cout << "File error!\n";
            system("pause");
            return 0;
        }
    }

    //创建SFML窗口
    sf::RenderWindow window(sf::VideoMode(800,600),"KruskalMST");
    window.setFramerateLimit(60); //限制帧率

    //实例化可视化器
    Visualizer visualizer(&window);
    if (!visualizer.loadFont("font.ttf")) //加载字体
    {
        cout << "字体加载失败!" << endl;
        system("pause");
        return -1;
    }

    //绘制初始场景
    System.drawInitialScene(visualizer);
    sf::sleep(sf::seconds(1)); //停顿1秒

    //运行Kruskal算法演示
    System.runKruskal(window,visualizer);

    //保持窗口打开，直到用户关闭
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //持续重绘以保持画面更新
        System.redraw(visualizer);
    }
    return 0;
}