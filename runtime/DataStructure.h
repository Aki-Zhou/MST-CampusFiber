#ifndef MYPROJECT_DATASTRUCTURE_H
#define MYPROJECT_DATASTRUCTURE_H
#pragma once
#include "Common.h"
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