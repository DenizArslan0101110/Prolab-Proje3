#include "Tree.h"

// Function to create a new BSDNode

struct BSDNode* CreateBSDNode(int value)
{
    struct BSDNode* new_node = (struct BSDNode*)malloc(sizeof(struct BSDNode));
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Function to insert a value into the BST
struct BSDNode* Insert(struct BSDNode* root, int value)
{
    if (root == NULL) return CreateBSDNode(value);
    if (value < root->value) root->left = Insert(root->left, value);
    else root->right = Insert(root->right, value);
    return root;
}

// Function to find the minimum value node in the tree (used in deletion)
struct BSDNode* FindMin(struct BSDNode* root)
{
    while (root && root->left != NULL) root = root->left;
    return root;
}

// Function to remove a value from the BST
struct BSDNode* Remove(struct BSDNode* root, int value)
{
    if (root == NULL) return root;
    if (value < root->value) root->left = Remove(root->left, value);
    else if (value > root->value) root->right = Remove(root->right, value);
    else
    {
// Node with the value found, now remove it
        if (root->left == NULL)
        {
            struct BSDNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct BSDNode* temp = root->left;
            free(root);
            return temp;
        }
// Node with two children: get the inorder successor (smallest in the right subtree)
        struct BSDNode* temp = FindMin(root->right);
// Replace root's value with the inorder successor's value
        root->value = temp->value;
// Delete the inorder successor
        root->right = Remove(root->right, temp->value);
    }
    return root;
}

// Function to print the tree in an inorder traversal
void PrintInOrder(struct BSDNode* root)
{
    if (root != NULL)
    {
        PrintInOrder(root->left);

        printf("%d ", root->value);

        PrintInOrder(root->right);
    }
}
