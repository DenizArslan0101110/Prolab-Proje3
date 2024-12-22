#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Queue.h"
#include "Parse.h"
#include <stdint.h>
#include <stdlib.h>
#include <basetsd.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define _INF write(1, "\x1b[0mINFO: ", 11);
#define _ERR write(1, "\x1b[31mERROR: ", 13);
#define _WRN write(1, "\x1b[33mWARNING: ", 15);
#define _DBG write(1, "\x1b[36mDEBUG: ", 13);
#define _INP write(1, "\x1b[95mINPUT: ", 13);

#define PAPER_LIMIT 400
#define LONG_STRING_LEN 8192
#define MAX_PAPER_LEN 2048
#define MAX_NAME_LEN 64
#define ORCID_LEN 24
#define DATABASE_SIZE 4096

#include "Structs.h"

struct Node* CreateNode(int v, int weight);
struct Graph* CreateGraph(int vertices, char is_directed);
void Edge(struct Graph* graph, int src, int dest, int weight);
void PrintGraph(struct Graph* graph);
void FreeGraph(struct Graph* graph);
void UpdateEdgeWeightUndirected(struct Graph* graph, int src, int dest, int newWeight);
void UpdateEdgeWeight(struct Graph* graph, int src, int dest, int newWeight);
int CountUsedIndexes(struct Graph* graph);
void PrintNodeInfo(int node_id, struct Author* data_list, int which_one);
void AssignEdgesToEveryone(struct Graph* main_graph, struct Author* data_list, int actual_used);
struct Queue* Dijkstra(struct Graph* graph, int source, int* dist, int* prev, int is_simple);
void LongestPath(struct Graph* graph, int source, int* dist, int* prev);
void PrintLongestPath(int* dist, int* prev, int num_vertices, int source);

#endif // GRAPH_H_INCLUDED
