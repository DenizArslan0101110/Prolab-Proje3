#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xlsxio_read.h>
#include <stdint.h>
#include <basetsd.h>

#define MAX_NAME_LEN 64

#define _INF printf("\x1b[0mINFO: ");
#define _ERR printf("\x1b[31mERROR: ");
#define _WRN printf("\x1b[33mWARNING: ");

// Structure to represent a node in the adjacency list
struct Node
{
    int node_id;
    struct Node* next;
};

// Structure to represent the graph
struct Graph
{
    int numVertices;
    struct Node** list_of_adjacency_lists;
    int isDirected;
};

// Function to create a new node
struct Node* createNode(int v)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->node_id = v;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph
struct Graph* createGraph(int vertices, char isDirected)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->isDirected = isDirected;

// Create an array of adjacency lists
    graph->list_of_adjacency_lists = malloc(vertices * sizeof(struct Node*));

// Empties the adjacency lists
    for (int i = 0; i < vertices; i++) graph->list_of_adjacency_lists[i] = NULL;
    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int src, int dest)
{
// Add edge from src to dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->list_of_adjacency_lists[src];
    graph->list_of_adjacency_lists[src] = newNode;

// If the graph is undirected, add an edge from dest to src as well
    if (!graph->isDirected)
    {
        newNode = createNode(src);
        newNode->next = graph->list_of_adjacency_lists[dest];
        graph->list_of_adjacency_lists[dest] = newNode;
    }
}

// Function to print the adjacency list representation of the graph
void printGraph(struct Graph* graph)
{
    printf("Vertex:  Adjacency List\n");
    for (int v = 0; v < graph->numVertices; v++)
    {
        struct Node* temp = graph->list_of_adjacency_lists[v];
        printf("%d --->", v);
        while (temp)
        {
            printf(" %d ->", temp->node_id);
            temp = temp->next;
        }
        printf(" NULL\n");
    }
}

struct Writer
{
    char name[MAX_NAME_LEN];
    char orcid[20];
};

int ExcelKatledici(struct Writer *place_holder)
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
    int row_no = 0;
    char cell_no = 0;
    char *orcid = calloc(1,20);
    char *writer_name = calloc(1,MAX_NAME_LEN);
    int writer_pos=0; // dont get confused, this is only used to place writers on the list and doesnt indicate much
    char* value;
    _INF printf("Contents of first sheet:\n");
    xlsxioreadersheet sheet = xlsxioread_sheet_open(excelci, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    xlsxioread_sheet_next_row(sheet);
    while (xlsxioread_sheet_next_row(sheet))
    {
        if(row_no%10000==0)
        {
            _INF printf("At row no: %d\r",row_no);
        }
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
        {
            cell_no++;
            switch(cell_no)
            {
                case 1: strcpy(orcid,value); break;
                case 2: continue;
                case 4: strcpy(writer_name,value); break;
                default: continue;
            }
            ///_INF printf("Switch case success.\n");

            if(strncmp(orcid,place_holder[writer_pos-1].orcid,20)!=0 || writer_pos==0)
            {
                ///_INF printf("Inside the if statement.\n");
                strncpy(place_holder[writer_pos].orcid, orcid, 20);
                ///_INF printf("Here is resulting orcid %s\n",orcid);
                strncpy(place_holder[writer_pos].name, writer_name, MAX_NAME_LEN);
                if(cell_no==4)writer_pos++;
            }
            ///_INF printf("If statement success.\n");
            free(value);
        }
        cell_no=0;
        row_no++;
    }
    xlsxioread_sheet_close(sheet);
//clean up
    xlsxioread_close(excelci);
    writer_pos--;
    _INF printf("Finished scanning xlsx file.\n");
    _INF printf("Found %d unique authors.\n", writer_pos);
    return writer_pos;
}

int main()
{
    int writer_count = 0;
    struct Writer *writer_list = (struct Writer*)malloc(MAXUINT32);
    memset(writer_list,0,sizeof(*writer_list));
    writer_count = ExcelKatledici(writer_list);
    /*for(int i=0; i<writer_count ;i++)
    {
        _INF printf("%s , %s\n", writer_list[i].orcid, writer_list[i].name);
    }*/
}




