#include "Parse.h"
#include "Graph.h"
#include "Registers.h"

int main()
{
    struct Graph* test_graph = CreateGraph(DATABASE_SIZE, 0);
    struct Author* test_data_list = (struct Author*)calloc(DATABASE_SIZE, sizeof(struct Author));
    memset(test_data_list,0,sizeof(*test_data_list));
    ExcelKatledici(test_data_list, test_graph);
    int actual_used = CountUsedIndexes(test_graph);
    actual_used += 10;
    _DBG printf("\n%d\n",actual_used);
    FreeGraph(test_graph);
    free(test_data_list);

    struct Graph* main_graph = CreateGraph(actual_used, 0);
    struct Author* data_list = (struct Author*)calloc(DATABASE_SIZE, sizeof(struct Author));
    memset(data_list,0,sizeof(*data_list));
    ExcelKatledici(data_list, main_graph);
    AssignEdgesToEveryone(main_graph, data_list, actual_used);

    for(int i=0; i<DATABASE_SIZE ;i++)
    {
        _INF
        printf("%d ",i);
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
