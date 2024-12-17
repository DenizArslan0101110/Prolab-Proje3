#include "Graph.h"

int global_not_found_counter=0;

struct Node* CreateNode(int v, int weight)
{
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->node_id = v;
    new_node->weight = weight;
    new_node->next = NULL;
    return new_node;
}

struct Graph* CreateGraph(int vertices, char is_directed)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->vertice_number = vertices;
    graph->is_directed = is_directed;
// create an array of adjacency lists
    graph->list_of_adjacency_lists = malloc(vertices * sizeof(struct Node*));
// initialize all adjacency lists as empty (NULL)
    for (int i = 0; i < vertices; i++)
    {
        graph->list_of_adjacency_lists[i] = NULL;
    }
    return graph;
}

void Edge(struct Graph* graph, int src, int dest, int weight)
{
// add edge from src to dest with a given weight
    struct Node* new_node = CreateNode(dest, weight);
    new_node->next = graph->list_of_adjacency_lists[src];
    graph->list_of_adjacency_lists[src] = new_node;
// if the graph is undirected, add an edge from dest to src with the same weight
    if (!graph->is_directed)
    {
        new_node = CreateNode(src, weight);
        new_node->next = graph->list_of_adjacency_lists[dest];
        graph->list_of_adjacency_lists[dest] = new_node;
    }
}

void PrintGraph(struct Graph* graph)
{
    _INF printf("Adjacency List\n");
    for (int i = 0; i < graph->vertice_number; i++)
    {
        struct Node* temp = graph->list_of_adjacency_lists[i];
        _INF printf("%d --->", i);
        while (temp)
        {
            printf(" %d (w: %d) ", temp->node_id, temp->weight);
            temp = temp->next;
        }
        write(1," NULL\n",6);
    }
}

void UpdateEdgeWeight(struct Graph* graph, int src, int dest, int newWeight)
{
    struct Node* temp = graph->list_of_adjacency_lists[src];
// Traverse the adjacency list of the source node
    while (temp != NULL)
    {
// If the destination node is found, update its weight
        if (temp->node_id == dest)
        {
            temp->weight = newWeight;
            ///_DBG write(1,"Updated weight of edge from ",28); printf("%d --> %d to %d.\n",src,dest,newWeight);
            return;
        }
        temp = temp->next;
    }
// If the destination node was not found in the adjacency list
    _WRN write(1,"Not found warning: Specified edge could not be found.",54); char wrncnt[20]; int len=snprintf(wrncnt,sizeof(wrncnt),"%d",global_not_found_counter++); write(1, wrncnt, len); write(1,"\r",1);
    ///_DBG printf("source: %d, destination: %d\n",src,dest);
}

// Function to update the weight in an undirected graph (both directions)
void UpdateEdgeWeightUndirected(struct Graph* graph, int src, int dest, int newWeight)
{
    UpdateEdgeWeight(graph, src, dest, newWeight);
    UpdateEdgeWeight(graph, dest, src, newWeight);
}

void FreeGraph(struct Graph* graph)
{
// Free each adjacency list (list of nodes) in the graph
    for (int i = 0; i < graph->vertice_number; i++)
    {
        struct Node* current = graph->list_of_adjacency_lists[i];
// Free each node in the adjacency list
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
// Free the array of adjacency lists
    free(graph->list_of_adjacency_lists);
// Free the graph itself
    free(graph);
}

// Function to count the number of used indices in the adjacency list
int CountUsedIndexes(struct Graph* graph)
{
    int count = 0;
// Iterate over all vertices
    for (int i = 0; i < graph->vertice_number; i++)
    {
// Check if the adjacency list for vertex i is non-empty
        if (graph->list_of_adjacency_lists[i] != NULL)count++;
    }
    return count;
}

// you give it authors id and an integer signifying what you want, it then proceeds to print it
void PrintNodeInfo(int node_id, struct Author* data_list, int which_one)
{
    switch(which_one)
    {
        case 0: printf("%s",data_list[node_id].name); break;
        case 1: printf("%s",data_list[node_id].orcid); break;
        case 2:
            {
                int i=0;
                printf("Full list of author's papers:\n");
                while(data_list[node_id].papers[i].name[0]!=0)
                {
                    _INF printf("\t%s\n",data_list[node_id].papers[i].name);
                    i++;
                }
                break;
            }
    }
}

void AssignEdgesToEveryone(struct Graph* main_graph, struct Author* data_list, int actual_used)
{
    for(int i=0; i<actual_used ;i++)
    {
        for(int j=i+1; j<actual_used ;j++)
        {
            int teamwork_amount = 0;
            int main_paper_limit = FindFirstEmptyPaperSlot(data_list[i].papers);
            int secondary_paper_limit = FindFirstEmptyPaperSlot(data_list[j].papers);
            for(int k=0; k<main_paper_limit ;k++)
            {
                for(int l=0; l<secondary_paper_limit ;l++)
                {
                    if(strcmp(data_list[j].papers[l].name,data_list[i].papers[k].name)==0) teamwork_amount++;
                }
            }
            if(teamwork_amount>0)UpdateEdgeWeightUndirected(main_graph, i, j, teamwork_amount);
        }
    }
}
