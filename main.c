#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xlsxio_read.h>
#include <stdint.h>
#include <basetsd.h>
#include <ctype.h>
#include <unistd.h>

#define PAPER_LIMIT 400
#define LONG_STRING_LEN 8192
#define MAX_PAPER_LEN 2048
#define MAX_NAME_LEN 64
#define ORCID_LEN 24
#define DATABASE_SIZE 4096

#define _INF write(1, "\x1b[0mINFO: ", 11);
#define _ERR write(1, "\x1b[31mERROR: ", 13);
#define _WRN write(1, "\x1b[33mWARNING: ", 15);
#define _DBG write(1, "\x1b[36mDEBUG: ", 13);
#define _INP write(1, "\x1b[95mINPUT: ", 13);

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

// structure to represent a node in the adjacency list
struct Node
{
    uint16_t node_id;
    struct Node* next;
};

// structure to represent the graph
struct Graph
{
    int vertice_number;
    struct Node** list_of_adjacency_lists;
    int is_directed;
};



void RouteInfoToNecessaryFunctions(struct Author* data_list, char* orcid, int mc_pos, char* mc_name, char* npc_names, char* paper, struct Graph* main_graph);
uint16_t FindFirstEmptyPaperSlot(struct Paper* papers);
uint32_t FindFirstEmptyAuthorSlot(struct Author* data_list);
int GraphPreparation(struct Author* data_list, struct Author new_recruit, int big_boss_node_id, struct Graph* main_graph);
int FindCountOfChar(char *value, char identifier);
char* PickString(char* long_string, int target_index, char encaser, char separator, char use_safe_mode);
int ExcelKatledici(struct Author* data_list, struct Graph* main_graph);
int ExcelYaglayici();
struct Node* createNode(int v);
struct Graph* createGraph(int vertices, char is_directed);
void Edge(struct Graph* graph, int src, int dest);
void PrintGraph(struct Graph* graph);
int CountUsedIndexes(struct Graph* graph);
void FreeGraph(struct Graph* graph);
int Register5(struct Graph* main_graph, struct Author* data_list, int index);
void PrintNodeInfo(int node_id, struct Author* data_list, int which_one);
int Register6(struct Graph* main_graph, struct Author* data_list);

int main()
{
    struct Graph* test_graph = createGraph(DATABASE_SIZE, 0);
    struct Author* test_data_list = (struct Author*)calloc(DATABASE_SIZE, sizeof(struct Author));
    memset(test_data_list,0,sizeof(*test_data_list));
    ExcelKatledici(test_data_list, test_graph);
    int actual_used = CountUsedIndexes(test_graph);
    actual_used += 10;
    _DBG printf("\n%d\n",actual_used);
    FreeGraph(test_graph);
    free(test_data_list);

    struct Graph* main_graph = createGraph(actual_used, 0);
    struct Author* data_list = (struct Author*)calloc(DATABASE_SIZE, sizeof(struct Author));
    memset(data_list,0,sizeof(*data_list));
    ExcelKatledici(data_list, main_graph);



    for(int i=0; i<DATABASE_SIZE ;i++)
    {
        _INF
        write(1, data_list[i].orcid, strlen(data_list[i].orcid));
        write(1, " , ", 3);
        write(1, data_list[i].name, strlen(data_list[i].name));
        write(1, "\n", 1);
    }
    PrintGraph(main_graph);
    int choice=0;
    int register_parameter_1;
    while(1)
    {
        _INP printf("Please select a register to be shown from below:"
                    "\n1-) INCOMPLETE"
                    "\n2-) INCOMPLETE"
                    "\n3-) INCOMPLETE"
                    "\n4-) INCOMPLETE"
                    "\n5-) Display all cooperations of an author"
                    "\n6-) Display the author with most cooperations"
                    "\n7-) INCOMPLETE"
                    "\n8-) Force exit program\n");
        _INP scanf("%d",&choice);
        switch(choice)
        {
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
        case 5:
            {
                int coop_no = 0;
                _INP printf("Enter author ID: ");
                scanf("%d",&register_parameter_1);
                coop_no = Register5(main_graph, data_list, register_parameter_1);
                _INF printf("Author has done a total of %d cooperations.\n",coop_no);
                break;
            }
        case 6:
            {
                int max_coops_id=0;
                max_coops_id = Register6(main_graph, data_list);
                _INF printf("The author with the most cooperations is \""); PrintNodeInfo(max_coops_id, data_list, 0); printf("\".\n");
            }
        case 7: break;
        case 8: exit(0); break;
        default: _WRN printf("Please pick one of the options presented\n"); break;
        }
    }
}

// function to create a new node
struct Node* createNode(int v)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->node_id = v;
    newNode->next = NULL;
    return newNode;
}

// function to create a graph
struct Graph* createGraph(int vertices, char is_directed)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->vertice_number = vertices;
    graph->is_directed = is_directed;
// create an array of adjacency lists
    graph->list_of_adjacency_lists = malloc(vertices * sizeof(struct Node*));
// empties the adjacency lists
    for (int i = 0; i < vertices; i++) graph->list_of_adjacency_lists[i] = NULL;
    return graph;
}

// function to add an edge to the graph
void Edge(struct Graph* graph, int src, int dest)
{
// add edge from src to dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->list_of_adjacency_lists[src];
    graph->list_of_adjacency_lists[src] = newNode;

// if the graph is undirected, add an edge from dest to src as well
    if (!graph->is_directed)
    {
        newNode = createNode(src);
        newNode->next = graph->list_of_adjacency_lists[dest];
        graph->list_of_adjacency_lists[dest] = newNode;
    }
}

// function to print the adjacency list representation of the graph
void PrintGraph(struct Graph* graph)
{
    _INF printf("Adjacency List\n");
    for (int v = 0; v < graph->vertice_number; v++)
    {
        struct Node* temp = graph->list_of_adjacency_lists[v];
        _INF printf("%d --->", v);
        while (temp)
        {
            printf(" %d ->", temp->node_id);
            temp = temp->next;
        }
        printf(" NULL\n");
    }
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

int ExcelKatledici(struct Author* data_list, struct Graph* main_graph)
{
// opens the file
    xlsxioreader excelci;
    if ((excelci = xlsxioread_open("PROLAB 3 - DATASET.xlsx")) == NULL)
    {
        _ERR fprintf(stderr, "File could not be opened!\n");
        return 1;
    }
    else
    {
        _INF printf("File was successfully opened!\n");
    }
// lists available sheets (we only have 1)
    xlsxioreadersheetlist sheetlist;
    const char* sheetname;
    _INF printf("Available sheets:\n");
    if ((sheetlist = xlsxioread_sheetlist_open(excelci)) != NULL)
    {
        while ((sheetname = xlsxioread_sheetlist_next(sheetlist)) != NULL)
        {
            _INF printf(" - %s\n", sheetname);
        }
        xlsxioread_sheetlist_close(sheetlist);
    }
//read values from first sheet
    int row_no = 0; char cell_no = 0;
    char *orcid = malloc(ORCID_LEN); memset(orcid, 0, ORCID_LEN);
    char *author_name = malloc(MAX_NAME_LEN); memset(author_name, 0, MAX_NAME_LEN);
    char author_pos = 0;
    char *npc_list = malloc(LONG_STRING_LEN); memset(npc_list, 0, LONG_STRING_LEN);
    char *paper = malloc(LONG_STRING_LEN); memset(npc_list, 0, LONG_STRING_LEN);
    char *value;
    //_INF printf("Contents of first sheet:\n");
    xlsxioreadersheet sheet = xlsxioread_sheet_open(excelci, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    xlsxioread_sheet_next_row(sheet);
    while (xlsxioread_sheet_next_row(sheet))
    {
        if(row_no%100==0) //77860
        {
            {_INF write(1,"At row no: ",11); printf("%d",row_no); write(1,"\n",1);}
        }
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
        {
            cell_no++;
            switch(cell_no)
            {
                case 1: strcpy(orcid,value); break;
                case 2: continue;
                case 3: author_pos = atoi(value); break;
                case 4: strcpy(author_name,value); break;
                case 5:
                    {
                        if(strlen(value)>=LONG_STRING_LEN){_WRN write(1, "Overflow warning: author list too long for LONG_STRING_LEN, consider increasing the limit.\n", 91);}
                        else{strcpy(npc_list,value); break;}
                    }
                case 6:
                    {
                        if(strlen(value)>=MAX_PAPER_LEN){_WRN write(1, "Overflow warning: paper name too long for MAX_PAPER_LEN, consider increasing the limit.\n", 90);}
                        else{strcpy(paper,value); break;}
                    }
                default: continue;
            }
            free(value);
        }
        RouteInfoToNecessaryFunctions(data_list, orcid, author_pos, author_name, npc_list, paper, main_graph);
// here it is here you know what to put here put it here please do it
        cell_no=0;
        row_no++;
    }
    xlsxioread_sheet_close(sheet);
//clean up
    xlsxioread_close(excelci);
    _INF printf("Finished scanning xlsx file.\n");
    return author_pos;
}

// tells you how many times a given char has appeared in the string
int FindCountOfChar(char *value, char identifier)
{
    int identifier_count = 0;
    for(int i=0; i<strlen(value) ;i++)
    {
        if(value[i]==identifier) identifier_count++;
    }
    ///_DBG printf("FindCountOfChar has returned: %d",identifier_count+1);
    return (identifier_count+1);
}

// PickStrings the names out of long string, can be customized to take escape character into account
char* PickString(char* long_string, int target_index, char encaser, char separator, char use_safe_mode)
{
    char* short_string = malloc(MAX_NAME_LEN);
    memset(short_string,0,MAX_NAME_LEN);
// go until end of the string (we will likely break out of this loop)
    int i=0, start=0, end=0;
    for(int index=0; index<target_index ;index++)
    {
        while(long_string[i]!=separator && long_string[i]!=0)i++;
        i++;
    }
// below is to give warning if lick function takes bad input, theres many of it
    if(long_string[i]==0){_ERR printf("Overlick error! PickString function likely went out of bounds!\n");return NULL;}

    while(long_string[i]!=encaser && long_string[i]!=0)i++;
    if(long_string[i]==0){_ERR printf("Overlick error! PickString function likely went out of bounds!\n");return NULL;}
    ///_DBG printf("PickString i: %d\n",i);
    start=i;
    i++;
    SAFE:
    if(long_string[i]==0){_ERR printf("Overlick error! PickString function likely went out of bounds!\n");return NULL;}
    while(long_string[i]!=encaser && long_string[i]!=0)i++;
    ///_DBG printf("PickString i-1: %c\n",long_string[i]);
    if(use_safe_mode==1 && !(long_string[i+1]==',' || long_string[i+1]==']')){i++;goto SAFE;}
    if(long_string[i]==0){_ERR printf("Overlick error! PickString function likely went out of bounds!\n");return NULL;}
    ///_DBG printf("PickString i: %d\n",i);
    end=i;
    for(int s=start+1, j=0; s<end ;s++, j++)
    {
        short_string[j]=long_string[s];
    }
    ///_DBG printf("PickString has returned \"%s\"\n",short_string);
    return short_string;
}

// you give information, it prepares it and sends it to the big function (ravish) which then lists it
void RouteInfoToNecessaryFunctions(struct Author* data_list, char* orcid, int mc_pos, char* mc_name, char* npc_names, char* paper, struct Graph* main_graph)
{
    struct Author carrier;
    strcpy(carrier.orcid, orcid);
    strcpy(carrier.name, mc_name);
    strcpy(carrier.papers[0].name, paper);
    ///_DBG printf("Sending to ravish: name:\"%s\" orcid:\"%s\" paper:\"%s\" \n", carrier.name, carrier.orcid, carrier.papers[0].name);
    int big_boss_node_id = GraphPreparation(data_list, carrier, -1, main_graph);
    memset(carrier.orcid, 0, sizeof(carrier.orcid));
    int npc_count = FindCountOfChar(npc_names, ',');
    for(int i=0; i<npc_count ;i++)
    {
        if(i==mc_pos-1) continue;
        strcpy(carrier.name, PickString(npc_names, i, '\'', ',', 1));
        ///_DBG printf("Sending to ravish: name:\"%s\" orcid:\"%s\" paper:\"%s\" \n", carrier.name, carrier.orcid, carrier.papers[0].name);
        GraphPreparation(data_list, carrier, big_boss_node_id, main_graph);
    }
    return;
};

int GraphPreparation(struct Author* data_list, struct Author new_recruit, int big_boss_node_id, struct Graph* main_graph)
{
    ///_DBG write(1, "Before FindFirstEmptyAuthorSlot.\n", 15);
    int first_empty_index = FindFirstEmptyAuthorSlot(data_list);
    ///_DBG write(1, "Past FindFirstEmptyAuthorSlot.\n", 13);
    int i=0;
    for(i=0; i<first_empty_index ;i++)
    {
        if(strcmp(new_recruit.name, data_list[i].name)==0) goto FOUNDHIM;
    }
    ///_DBG write(1, "Past for loop.\n", 15);
    strcpy(data_list[first_empty_index].name, new_recruit.name);
    strcpy(data_list[first_empty_index].papers[FindFirstEmptyPaperSlot(data_list[first_empty_index].papers)].name, new_recruit.papers[0].name);
    ///_DBG write(1, "Past copying.\n", 14);
    if(new_recruit.orcid[0]!='0')
    {
        Edge(main_graph, big_boss_node_id, first_empty_index);
        return -1;
    }
    else
    {
        strcpy(data_list[first_empty_index].orcid, new_recruit.orcid);
        return first_empty_index;
    }
    return -1;
    FOUNDHIM:
    strcpy(data_list[i].papers[FindFirstEmptyPaperSlot(data_list[i].papers)].name, new_recruit.papers[0].name);
    if(big_boss_node_id == -1) return i;
    return -1;
}

// finds and returns the index of first empty name space on array
uint32_t FindFirstEmptyAuthorSlot(struct Author* data_list)
{
    for(uint32_t i=0; i<DATABASE_SIZE ;i++)
    {
        ///_DBG printf("%d\n",i);
        if(data_list[i].name[0] == 0)
        {
            ///_DBG printf("FindFirstEmptyAuthorSlot has returned %d.\n",i);
            return i;
        }
    }
    _ERR write(1, "Overflow error: Main storage array has ran out of space, code will not work!\n", 77);
    return -1;
}

// same as FindFirstEmptyAuthorSlot except it does so on papers list
uint16_t FindFirstEmptyPaperSlot(struct Paper* papers)
{
    for(uint16_t i=0; i<PAPER_LIMIT ;i++)
    {
        if(papers[i].name[0] == 0)
        {
            ///_DBG printf("FindFirstEmptyPaperSlot has returned %d.\n",i);
            return i;
        }
    }
    _WRN write(1, "Overwrite warning: Paper limit was exceeded, last index will be overwritten.\n", 77);
    return PAPER_LIMIT-1;
}

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







