#include "Parse.h"
#include "Graph.h"
#include "Registers.h"
#include "Tree.h"

struct Queue* queue = NULL;

int Register1(struct Graph* main_graph, struct Author* data_list, int start, int finish, int is_simple)
{
    int* dist = (int*)malloc(main_graph->vertice_number * sizeof(int));
    int* prev = (int*)malloc(main_graph->vertice_number * sizeof(int));
    queue = Dijkstra(main_graph, start, dist, prev, is_simple);
    if(!is_simple)
    {
        for(int i=0; i<main_graph->vertice_number ;i++)
        {
            write(1,"\x1b[0m",4);
            if(i==start) {write(1, "\x1b[44mSTRT: ", 11); goto PRINTING;}
            else if(i==finish) {write(1, "\x1b[44mFNSH: ", 11);goto PRINTING;}
            else if(dist[i]<INT_MAX) {_INF goto PRINTING;}
            else goto NOTPRINTING;
            PRINTING:
                write(1,"node_id: ",9);
                char iii[10]; int leng=snprintf(iii,sizeof(iii),"%d",i); write(1, iii, leng); snprintf(iii, 10-leng, "          "); write(1,iii,10-leng); write(1,"previous_node: ",15);
                char prevus[10]; int lengg=snprintf(prevus,sizeof(prevus),"%d",prev[i]); write(1, prevus, lengg); snprintf(prevus, 10-lengg, "          "); write(1,prevus,10-lengg); write(1,"distance: ",10);
                char distace[10]; int lenggg=snprintf(distace,sizeof(distace),"%d",dist[i]); write(1, distace, lenggg); snprintf(distace, 10-lenggg, "          "); write(1,distace,10-lenggg);
                write(1,"\x1b[0m\n",5);
            NOTPRINTING:
        }
    }
    if(prev[finish]==-1 && !is_simple) {_WRN printf("Shortest path between %d and %d could not be found, nodes are not connected.\n",start,finish);}
    if(prev[finish]!=-1)
    {
        _INF printf("Shortest path between %d and %d has a distance of %d, and is as follows: \n", start, finish, dist[finish]);
        int current = finish;
        printf("%d", finish);
        while(prev[current]!=-1)
        {
            printf(" ---> %d",prev[current]);
            current = prev[current];
        }
        printf("\n");
    }
    return 1;
}

void Register2(struct Graph* main_graph, struct Author* data_list, int index)
{
    struct Node* current = main_graph->list_of_adjacency_lists[index];
    int coauthor_no = 0;
    while (current != NULL)
    {
        coauthor_no++;
        current = current->next;
    }
    ///_DBG printf("Does the function at least start!?\n");
    struct Author* small_data_list = malloc((coauthor_no+3)*sizeof(struct Author));
    ///_DBG printf("Succes at making the struct.\n");
    small_data_list[0] = data_list[index];
    ///_DBG printf("Succes at getting temp value.\n");
    struct Node* temp = main_graph->list_of_adjacency_lists[index];
    int i=1;
    ///_DBG printf("Right before while loop\n");
    while(temp != NULL)
    {
        small_data_list[i] = data_list[temp->node_id];
        temp = temp->next; i++;
    }
// right at this point we made a small_data_list from all connected authors, this will be useful down below
    int paper_count_holder[coauthor_no+1];
    i=0;
    ///_DBG printf("Before finding papers.\n");
    while(small_data_list[i].name[0] != 0)
    {
        paper_count_holder[i] = FindFirstEmptyPaperSlot(small_data_list[i].papers);
        i++;
    }
    _INF printf("Sit tight, this can take a long time. (unlikely, but possible)\n");
    for(int aaa=0; aaa<FindFirstEmptyAuthorSlot(small_data_list)-1 ;aaa++)
    {
        AGAIN:
        if(paper_count_holder[aaa] < paper_count_holder[aaa+1])
        {
            int temporary = paper_count_holder[aaa+1];
            paper_count_holder[aaa+1] = paper_count_holder[aaa];
            paper_count_holder[aaa] = temporary;
            struct Author temporarius = small_data_list[aaa+1];
            small_data_list[aaa+1] = small_data_list[aaa];
            small_data_list[aaa] = temporarius;
            if(aaa>0) aaa--;
            goto AGAIN;
        }
    }
    ///_DBG printf("Before printing\n");
    for(int aaa=0; aaa<FindFirstEmptyAuthorSlot(small_data_list) ;aaa++)
    {
        _DBG printf("\"%s\" with %d papers was added to the queue.\n",small_data_list[aaa].name, paper_count_holder[aaa]);
    }

}

void Register3(struct Graph* main_graph, struct Author* data_list)
{
    if(queue == NULL){ _WRN printf("Queue does not exists, try Register 1 first.");}
    int list[512]; memset(list,-1,512*sizeof(int));
    int element = -1; int i=0;
    while((element = Dequeue(queue)) != -1 && i<512)
    {
        list[i] = element;
        i++;
    }
// code chunk below destroys any duplicates in the list and sorts them into order (This is the worst code I have ever written but it works)
    for(int aaa=0; aaa<512 ;aaa++)
    {
        int spy = 0;
        if(list[aaa]!=-1) spy = list[aaa];
        for(int bbb=0; bbb<512 ;bbb++)
        {
            if(list[bbb]==spy && aaa!=bbb) list[bbb] = -1;
        }
    }
    for(int aaa=0; aaa<512 ;aaa++)
    {
        if(list[aaa]==-1 && list[aaa+1]!=-1)
        {
            list[aaa] = list[aaa+1];
            list[aaa+1] = -1;
            if(aaa>0) aaa--;
        }
    }
    i--;
    struct BSDNode* binary_search_tree = CreateBSDNode(list[i]);
    i--;
    for(; i>0 ;i--)
    {
        Insert(binary_search_tree, list[i]);
    }
    _INF printf("Binary Search tree of the queue: \n");
    PrintInOrder(binary_search_tree);
    _INP printf("Which index would you like to delete off of the binary search tree?\n");
    int input = -1;
    scanf("%d", &input);
    Remove(binary_search_tree, input);
    _INF printf("Binary Search tree of the queue after deletion: \n");
    PrintInOrder(binary_search_tree);
}

void Register4(struct Graph* main_graph, struct Author* data_list, int index)
{
    if(data_list[index].orcid[0]!=0)
    {
        _WRN printf("Given author has no connections of depth \"2\" and as such, cant be calculated.\n");
        goto FUCKTHISIMOUT;
    }
    int to_visit[512]; memset(to_visit, -1, 512*sizeof(int));
    struct Node* current = main_graph->list_of_adjacency_lists[index];
    struct Node* current2 = NULL;
    int i=0;
    while (current != NULL)
    {
        current2 = main_graph->list_of_adjacency_lists[current->node_id];
            while (current2 != NULL)
            {
                to_visit[i] = current2->node_id; i++;
                current2 = current2->next;
            }
        current = current->next;
    }
    for(i=0; to_visit[i] != -1 ;i++)
    {
        Register1(main_graph, data_list, index, to_visit[i], 1);
    }
    FUCKTHISIMOUT:
}

int Register5(struct Graph* main_graph, struct Author* data_list, int index)
{
    struct Node* current = main_graph->list_of_adjacency_lists[index];
    int coauthor_no = 0;
    while (current != NULL)
    {
        coauthor_no++;
        current = current->next;
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

int Register7(struct Graph* main_graph, struct Author* data_list, int start)
{
    int* dist = (int*)malloc(main_graph->vertice_number * sizeof(int));
    int* prev = (int*)malloc(main_graph->vertice_number * sizeof(int));
    Dijkstra(main_graph, start, dist, prev, 1);
    for(int i=0; i<main_graph->vertice_number ;i++)
    {
        write(1,"\x1b[0m",4);
        if(i==start) {write(1, "\x1b[44mSTRT: ", 11); goto PRINTING;}
        else if(dist[i]<INT_MAX) {_INF goto PRINTING;}
        else goto NOTPRINTING;
        PRINTING:
            write(1,"node_id: ",9);
            char iii[10]; int leng=snprintf(iii,sizeof(iii),"%d",i); write(1, iii, leng); snprintf(iii, 10-leng, "          "); write(1,iii,10-leng); write(1,"previous_node: ",15);
            char prevus[10]; int lengg=snprintf(prevus,sizeof(prevus),"%d",prev[i]); write(1, prevus, lengg); snprintf(prevus, 10-lengg, "          "); write(1,prevus,10-lengg); write(1,"distance: ",10);
            char distace[10]; int lenggg=snprintf(distace,sizeof(distace),"%d",dist[i]); write(1, distace, lenggg); snprintf(distace, 10-lenggg, "          "); write(1,distace,10-lenggg);
            write(1,"\x1b[0m\n",5);
        NOTPRINTING:
    }
    ///if(prev[finish]==-1) {_WRN printf("Shortest path between %d and %d could not be found, nodes are not connected.\n",start,finish);}
    PrintLongestPath(dist, prev, main_graph->vertice_number, start);
    return 1;
}
