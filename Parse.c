#include "Parse.h"

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
        Edge(main_graph, big_boss_node_id, first_empty_index, 0);
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
