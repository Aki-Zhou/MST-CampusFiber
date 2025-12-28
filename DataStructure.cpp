#include "DataStructure.h"

MinHeap::MinHeap():size(0){}

// 初始化堆，将大小置为0
void MinHeap::init()
{
    size = 0;
}

// 判断堆是否为空
bool MinHeap::isEmpty()
{
    return size == 0;
}

// 将边插入堆中，并执行上浮操作以维护最小堆性质
void MinHeap::Push(Edge e)
{
    if (size >= MAX_Edges)
        return;
    int i = size++;
    while (i > 0)
    {
        int p = (i-1) / 2; // 父节点索引
        if (data[p].weight <= e.weight)
            break;
        data[i] = data[p]; // 父节点下移
        i = p;
    }
    data[i] = e;
}

// 下沉操作：当堆顶元素被移除或替换后，将新的根节点向下移动到合适位置
void MinHeap::siftDown(int index)
{
    int current = index;
    Edge temp = data[current]; // 保存当前需要下沉的元素

    while (current * 2 + 1 < size)
    {
        int child = 2 * current + 1; // 左孩子

        // 如果有右孩子，且右孩子更小，则选择右孩子
        if (child + 1 < size && data[child + 1].weight < data[child].weight)
            child++;
        // 如果较小的孩子节点不比当前元素小，满足最小堆性质，停止下沉
        if (data[child].weight >= temp.weight)
            break;
        // 否则，孩子节点上移
        data[current] = data[child];
        current = child;
    }
    data[current] = temp; // 将元素放入最终位置
}

// 弹出堆顶元素（最小权重的边），并将最后一个元素移到堆顶后下沉
Edge MinHeap::Pop()
{
    Edge ret = data[0]; // 取出堆顶（最小值）
    data[0] = data[--size]; // 将末尾元素移到堆顶
    siftDown(0); // 执行下沉操作
    return ret;
}

// 初始化并查集，每个节点的父节点指向自己
void UnionFind::init(int n)
{
    for (int i=0;i<n;i++)
        parent[i] = i;
}

// 查找节点x的根节点，并进行路径压缩
int UnionFind::find(int x)
{
    if (parent[x] == x)
        return x;
    return parent[x] = find(parent[x]); // 路径压缩
}

// 合并x和y所在的集合。如果已经在同一集合返回false，否则合并并返回true
bool UnionFind::unite(int x,int y)
{
    int rootx = find(x);
    int rooty = find(y);
    if (rootx != rooty)
    {
        parent[rootx] = rooty; // 将x的根挂在y的根下
        return true;
    }
    else
    {
        return false; // 已连通，形成环
    }
}
