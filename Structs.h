#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

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

#endif // STRUCTS_H_INCLUDED
