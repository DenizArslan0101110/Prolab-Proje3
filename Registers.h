#ifndef REGISTERS_H_INCLUDED
#define REGISTERS_H_INCLUDED

#include "rlgl.h"
#include "raymath.h"
#include "Parse.h"
#include <stdio.h>


int Register1(struct Graph* main_graph, struct Author* data_list, int start, int finish, int is_simple,struct Position* position);
void Register2(struct Graph* main_graph, struct Author* data_list, int index,struct Position* position);
void Register3(struct Graph* main_graph, struct Author* data_list);
void Register4(struct Graph* main_graph, struct Author* data_list, int index,struct Position* position);
int Register5(struct Graph* main_graph, struct Author* data_list, int index);
int Register6(struct Graph* main_graph, struct Author* data_list);
int Register7(struct Graph* main_graph, struct Author* data_list, int start,struct Position* position);
int GetInput(Font myFont);
void DrawBSTInOrder(struct BSDNode* root,int LeftOrRight,int previusX,int previusY,struct Author* data_list,Font myFont);

#endif // REGISTERS_H_INCLUDED
