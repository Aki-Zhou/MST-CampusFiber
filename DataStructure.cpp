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

// 上浮操作：将新插入的节点向上移动到合适位置
// 维护最小堆性质：父节点的权重必须小于等于子节点的权重
void MinHeap::siftUp(int index)
{
    int i = index;
    Edge temp = data[i]; // 保存当前需要上浮的元素
    while (i > 0)
    {
        int p = (i-1) / 2; // 计算父节点索引

        // 如果父节点的权重小于等于当前元素的权重，说明满足最小堆性质，停止上浮
        if (data[p].weight <= temp.weight)
            break;

        // 否则，父节点下移（实际上是将父节点的值赋给当前位置）
        data[i] = data[p];
        i = p; // 当前索引指向父节点位置，继续向上比较
    }
    data[i] = temp; // 将元素放入最终找到的合适位置
}

// 将边插入堆中，并执行上浮操作以维护最小堆性质
void MinHeap::Push(Edge e)
{
    // 检查堆是否已满
    if (size >= MAX_Edges)
        return;

    data[size] = e; // 将新元素放入堆的末尾
    siftUp(size);   // 对新加入的元素执行上浮操作，调整其位置
    size++;         // 堆的大小加1
}

// 下沉操作：当堆顶元素被移除或替换后，将新的根节点向下移动到合适位置
void MinHeap::siftDown(int index)
{
    int i = index;
    Edge temp = data[i]; // 保存当前需要下沉的元素

    while (i * 2 + 1 < size)
    {
        int child = 2 * i + 1; // 左孩子

        // 如果有右孩子，且右孩子更小，则选择右孩子
        if (child + 1 < size && data[child + 1].weight < data[child].weight)
            child++;
        // 如果较小的孩子节点不比当前元素小，满足最小堆性质，停止下沉
        if (data[child].weight >= temp.weight)
            break;
        // 否则，孩子节点上移
        data[i] = data[child];
        i = child;
    }
    data[i] = temp; // 将元素放入最终位置
}

// 弹出堆顶元素最小权重的边，并将最后一个元素移到堆顶后下沉
Edge MinHeap::Pop()
{
    Edge ret = data[0]; // 取出堆顶最小值
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
