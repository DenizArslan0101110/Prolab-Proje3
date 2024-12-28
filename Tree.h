#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Structs.h"

#define _WRN write(1, "\x1b[33mWARNING: ", 15);

struct BSDNode* CreateBSDNode(int value);
struct BSDNode* Insert(struct BSDNode* root, int value);
struct BSDNode* FindMin(struct BSDNode* root);
struct BSDNode* Remove(struct BSDNode* root, int value);
void PrintInOrder(struct BSDNode* root);
void InOrderTraversal(struct BSDNode* root, int* arr, int* index);
struct BSDNode* BuildBalancedBSD(int* arr, int start, int end);
struct BSDNode* BalanceBSD(struct BSDNode* root);


#endif // TREE_H_INCLUDED
