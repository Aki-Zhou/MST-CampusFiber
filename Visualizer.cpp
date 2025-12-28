#include <Visualizer.h>
#include <string>
#include <cmath> // 引入cmath用于计算角度

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp" // 旋转矩形自定义lines

Visualizer::Visualizer(sf::RenderWindow* win)
{
    this->window = win;
}

// 加载字体，用于显示文本
bool Visualizer::loadFont(const char* filename)
{
    return font.loadFromFile(filename);
}

// 核心绘制函数：每一帧都会调用此函数来更新画面
void Visualizer::drawScene(int nodeCount, Node* nodes, int totalCost, std::vector<Edge>& edges, const std::vector<std::string>& logs, bool isCompleted) const
{
    window->clear(sf::Color(30, 30, 30)); // 清除上一帧内容，使用深灰色背景

    // 1. 绘制状态栏（左上角）
    sf::Text status;
    status.setFont(font);
    status.setCharacterSize(20);
    status.setFillColor(sf::Color::Green); // 绿色
    status.setPosition(15,15);
    status.setString("Total Cost: " + std::to_string(totalCost) );
    window->draw(status);

    int logY = 45; // 日志起始Y坐标

    // 如果算法完成，显示完成提示
    if (isCompleted) {
        sf::Text completedText;
        completedText.setFont(font);
        completedText.setCharacterSize(20);
        completedText.setFillColor(sf::Color::Green);
        completedText.setPosition(15, logY);
        completedText.setString("MST Completed");
        window->draw(completedText);
        logY += 30;
    }

    // 绘制操作日志
    for (const auto& log : logs) {
        sf::Text logText;
        logText.setFont(font);
        logText.setCharacterSize(16);

        // 如果是环路检测日志，显示为红色
        if (log.find("Cycle:") == 0) {
             logText.setFillColor(sf::Color(255, 100, 100)); // Light Red
        }
        else
        {
             logText.setFillColor(sf::Color::White);
        }

        logText.setPosition(15, logY);
        logText.setString(log);
        window->draw(logText);
        logY += 20;
    }

    // 2. 绘制边
    for ( auto& e : edges)
    {
        sf::Color color = sf::Color(100, 100, 100); // 默认深灰色（未处理）
        float thickness = 2.0;

        if (e.state == 1) // 正在扫描（黄色）
        {
            color = sf::Color(255, 215, 0);
            thickness = 4.0;
        }
        else if (e.state == 2) // 已加入MST（绿色）
        {
            color = sf::Color(50, 205, 50);
            thickness = 4.0;
        }
        else if (e.state == 3) // 形成环路，被丢弃（红色）
        {
            color = sf::Color::Red;
            thickness = 4.0;
        }

        sf::Vector2f p1(nodes[e.u].x, nodes[e.u].y);
        sf::Vector2f p2(nodes[e.v].x, nodes[e.v].y);

        // 计算线的长度和角度，以便使用矩形绘制带宽度的线
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float length = std::sqrt(dx*dx + dy*dy);
        float angle = std::atan2(dy, dx) * 180 / 3.14159265f;

        sf::RectangleShape line(sf::Vector2f(length, thickness));
        line.setOrigin(0, thickness / 2); // 设置原点为左侧中心，方便旋转
        line.setPosition(p1);
        line.setRotation(angle);
        line.setFillColor(color);

        window->draw(line);

        // 绘制边的权重文字，显示在边的中点
        sf::Text weightText;
        weightText.setFont(font);
        weightText.setString(std::to_string(e.weight));
        weightText.setCharacterSize(14);
        weightText.setFillColor(sf::Color::White);
        weightText.setOutlineColor(sf::Color::Black);
        weightText.setOutlineThickness(1);
        weightText.setPosition((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
        window->draw(weightText);
    }
    // 3. 绘制节点
    for (int i=0;i < nodeCount;++i)
    {
        sf::CircleShape c(20); // 半径20
        c.setOrigin(20,20);    // 中心点
        c.setPosition(nodes[i].x,nodes[i].y);
        c.setFillColor(sf::Color(70,130,180)); // 钢蓝色
        c.setOutlineThickness(2);
        c.setOutlineColor(sf::Color::White); // 白色边框
        window->draw(c);

        // 绘制节点名称
        sf::Text name;
        name.setFont(font);
        name.setString(nodes[i].name);
        name.setCharacterSize(16);
        name.setFillColor(sf::Color::White); // 白色文字

        // 居中显示文字
        sf::FloatRect textRect = name.getLocalBounds();
        name.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
        name.setPosition(nodes[i].x, nodes[i].y - 35); // 放在圆圈上方


        window->draw(name);

        // 在圆圈中间显示节点ID
        sf::Text idText;
        idText.setFont(font);
        idText.setString(std::to_string(nodes[i].id));
        idText.setCharacterSize(18);
        idText.setFillColor(sf::Color::White);
        sf::FloatRect idRect = idText.getLocalBounds();
        idText.setOrigin(idRect.left + idRect.width/2.0f, idRect.top  + idRect.height/2.0f);
        idText.setPosition(nodes[i].x, nodes[i].y);
        window->draw(idText);
    }
    window->display(); // 刷新窗口显示
}
