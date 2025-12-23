#ifndef MYPROJECT_COMMON_H
#define MYPROJECT_COMMON_H
#pragma once

const int MAX_Nodes = 100;
const int MAX_Edges = 100;

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

#endif //MYPROJECT_COMMON_H