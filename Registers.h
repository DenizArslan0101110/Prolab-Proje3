#ifndef REGISTERS_H_INCLUDED
#define REGISTERS_H_INCLUDED


#include "Parse.h"
#include <stdio.h>


int Register1(struct Graph* main_graph, struct Author* data_list, int start, int finish);
void Register2(struct Graph* main_graph, struct Author* data_list, int index);
int Register5(struct Graph* main_graph, struct Author* data_list, int index);
int Register6(struct Graph* main_graph, struct Author* data_list);
int Register7(struct Graph* main_graph, struct Author* data_list, int start);

#endif // REGISTERS_H_INCLUDED
