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

bool Visualizer::loadFont(const char* filename)
{
    return font.loadFromFile(filename);
}

void Visualizer::drawScene(int nodeCount, Node* nodes, int totalCost, std::vector<Edge>& edges)
{
    window->clear(sf::Color(30, 30, 30)); // 改为深灰色背景

    //1绘制状态栏
    // 添加半透明背景
    sf::RectangleShape statusBg(sf::Vector2f(400, 60));
    statusBg.setFillColor(sf::Color(0, 0, 0, 150));
    statusBg.setPosition(5, 5);
    window->draw(statusBg);

    sf::Text status;
    status.setFont(font);
    status.setCharacterSize(20);
    status.setFillColor(sf::Color::White); // 改为白色文字
    status.setPosition(15,15);
    status.setString("Total Cost: " + std::to_string(totalCost) +
        "\nYellow: Checking | Green: Linked | Red: Loop");
    window->draw(status);

    //2.绘制边
    for ( auto& e : edges)
    {
        sf::Color color = sf::Color(100, 100, 100); // 默认深灰色
        float thickness = 2.0;

        if (e.state == 1) // Checking
        {
            color = sf::Color(255, 215, 0); // 金色/黄色
            thickness = 4.0;
        }
        else if (e.state == 2) // Linked
        {
            color = sf::Color(50, 205, 50); // 鲜绿色
            thickness = 4.0;
        }
        else if (e.state == 3) // Loop
        {
            color = sf::Color::Red; // 纯红色，更显眼
            thickness = 5.0;
        }

        sf::Vector2f p1(nodes[e.u].x, nodes[e.u].y);
        sf::Vector2f p2(nodes[e.v].x, nodes[e.v].y);

        // 计算线的长度和角度
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

        // 绘制边的权重文字
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
    //3.绘制节点
    for (int i=0;i < nodeCount;++i)
    {
        sf::CircleShape c(20); // 稍微加大一点
        c.setOrigin(20,20);
        c.setPosition(nodes[i].x,nodes[i].y);
        c.setFillColor(sf::Color(70,130,180)); // 钢蓝色
        c.setOutlineThickness(2);
        c.setOutlineColor(sf::Color::White); // 白色边框
        window->draw(c);

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

        // 在圆圈中间显示ID
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
    window->display();
}