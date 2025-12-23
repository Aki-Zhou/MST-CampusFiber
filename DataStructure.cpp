#include "DataStructure.h"

MinHeap::MinHeap():size(0){}

void MinHeap::init()
{
    size = 0;
}

bool MinHeap::isEmpty()
{
    return size == 0;
}

void MinHeap::Push(Edge e)
{
    if (size >= MAX_Edges)
        return;
    int i = size++;
    while (i > 0)
    {
        int p = (i-1) / 2;
        if (data[p].weight <= e.weight)
            break;
        data[i] = data[p];
        i = p;
    }
    data[i] = e;
}

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

Edge MinHeap::Pop()
{
    Edge ret = data[0]; // 取出堆顶（最小值）
    data[0] = data[--size]; // 将末尾元素移到堆顶
    siftDown(0); // 执行下沉操作
    return ret;
}
void UnionFind::init(int n)
{
    for (int i=0;i<n;i++)
        parent[i] = i;
}
int UnionFind::find(int x)
{
    if (parent[x] == x)
        return x;
    return parent[x] = find(parent[x]);
}

bool UnionFind::unite(int x,int y)
{
    int rootx = find(x);
    int rooty = find(y);
    if (rootx != rooty)
    {
        parent[rootx] = rooty;
        return true;
    }
    else
    {
        return false;
    }
}





