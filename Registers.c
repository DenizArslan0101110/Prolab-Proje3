#include "Parse.h"
#include "Graph.h"
#include "Registers.h"


int Register5(struct Graph* main_graph, struct Author* data_list, int index)
{
    struct Node* current = main_graph->list_of_adjacency_lists[index];  // Use a temporary pointer
    int coauthor_no = 0;
    while (current != NULL)  // Traverse the adjacency list
    {
        coauthor_no++;  // Count the number of coauthors
        current = current->next;  // Move to the next node
    }
    return coauthor_no;
}

int Register6(struct Graph* main_graph, struct Author* data_list)
{
    int most = -1, coop_no_max = 0, temp=0;
    for(int i=0; i<main_graph->vertice_number ;i++)
    {
        temp = Register5(main_graph, data_list, i);
        if(temp>coop_no_max)
        {
            coop_no_max=temp;
            most=i;
        }
    }
    _DBG printf("with %d cooperations, author's id: %d\n",coop_no_max, most);
    //_DBG PrintNodeInfo(most, data_list, 2);
    return most;
}
