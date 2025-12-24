#ifndef MYPROJECT_DATASTRUCTURE_H
#define MYPROJECT_DATASTRUCTURE_H
#pragma once

constexpr int MAX_Nodes = 100;
constexpr int MAX_Edges = 100;

struct Node
{
    int id;
    float x,y;
    char name[50];
};

struct Edge
{
    int u,v;
    int weight;
    int state;
};
class  MinHeap
{
    Edge data[MAX_Edges];
    int size;
    void siftDown(int index); // 下浮操作
public:
    MinHeap();
    void init();
    void Push(Edge e);
    Edge Pop();
    bool isEmpty();
};

class UnionFind
{
private:
    int parent[MAX_Nodes];
public:
    void init(int n);
    int find(int x);
    bool unite(int x, int y);
};
#endif //MYPROJECT_DATASTRUCTURE_H