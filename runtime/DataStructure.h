#ifndef MYPROJECT_DATASTRUCTURE_H
#define MYPROJECT_DATASTRUCTURE_H
#pragma once

constexpr int MAX_Nodes = 100;
constexpr int MAX_Edges = 100;

//节点结构体，存储节点的基本信息
struct Node
{
    int id;         //节点ID
    float x,y;      //节点在屏幕上的坐标
    char name[50];  //节点名称
};

//边结构体，存储边的连接关系和权重
struct Edge
{
    int u,v;        //边的两个端点ID
    int weight;     //边的权重（成本）
    int state;      //边的状态：0=普通(灰色), 1=扫描中(黄色), 2=已选中(绿色), 3=成环(红色)
};

//最小堆类，用于Kruskal算法中按权重从小到大取出边
class  MinHeap
{
    Edge data[MAX_Edges]; //堆数组
    int size;             //当前堆大小
    void siftDown(int index); //下沉操作，维护堆性质
    void siftUp(int index);   //上浮操作
public:
    MinHeap();
    void init();          //初始化堆
    bool Push(Edge e);    //入堆，返回false表示堆已满、该边被丢弃
    Edge Pop();           //出堆（取出最小权重的边）
    bool isEmpty();       //判断堆是否为空
    bool isFull();        //判断堆是否已满
};

//并查集类，用于判断两个节点是否连通，以及合并连通分量
class UnionFind
{
private:
    int parent[MAX_Nodes]; //父节点数组
public:
    void init(int n);      //初始化并查集，每个节点自成一个集合
    int find(int x);       //查找节点所属集合的根节点
    bool unite(int x, int y); //合并两个集合，如果已经在同一集合返回false
};
#endif //MYPROJECT_DATASTRUCTURE_H