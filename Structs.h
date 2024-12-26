#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <stdint.h>
#include "raylib.h"

#define PAPER_LIMIT 400
#define LONG_STRING_LEN 8192
#define MAX_PAPER_LEN 2048
#define MAX_NAME_LEN 64
#define ORCID_LEN 24
#define DATABASE_SIZE 4096

// below is information holding structs

struct Paper
{
    char name[MAX_PAPER_LEN];
};
struct Author
{
    char name[MAX_NAME_LEN];
    char orcid[ORCID_LEN];
    struct Paper papers[PAPER_LIMIT];
};

// below are the graph components, these make a graph
struct Node
{
    uint16_t node_id;
    int weight;
    struct Node* next;
};
struct Graph
{
    int vertice_number;
    struct Node** list_of_adjacency_lists;
    int is_directed;
};

// below are for the queue,
struct Queue
{
    int* data;
    int front;
    int rear;
    int size;
    int capacity;
};

// I accidentally named this struct BSDNode instead of BSTNode and we are going to have to live with it (no I am not fixing it)
struct BSDNode
{
    int value;
    struct BSDNode *left;
    struct BSDNode *right;
};
struct CircleFeatures
{
    Color color;
    int radius;
    int NumberOfCoauthors;
};

struct Position
{
    int pos_x;
    int pos_y;
    struct Author author;
    struct CircleFeatures cf;
    bool IsDrawed;
};
struct Button
{
    float widgt;
    float height;
    float pos_x;
    float pos_y;
    Color color;
    bool isClicked;
};


#endif // STRUCTS_H_INCLUDED
