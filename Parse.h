#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include "Graph.h"
#include <xlsxio_read.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "Structs.h"

int FindCountOfChar(char *value, char identifier);
uint16_t FindFirstEmptyPaperSlot(struct Paper* papers);
uint32_t FindFirstEmptyAuthorSlot(struct Author* data_list);
char* PickString(char* long_string, int target_index, char encaser, char separator, char use_safe_mode);
void RouteInfoToNecessaryFunctions(struct Author* data_list, char* orcid, int mc_pos, char* mc_name, char* npc_names, char* paper, struct Graph* main_graph);
int GraphPreparation(struct Author* data_list, struct Author new_recruit, int big_boss_node_id, struct Graph* main_graph);
int ExcelKatledici(struct Author* data_list, struct Graph* main_graph);

#endif // PARSE_H_INCLUDED
