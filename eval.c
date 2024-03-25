#include <stdio.h>
#include <stdlib.h>
#include "hbt.h"

int getNode(FILE * f, int * height, int * isBST, int * isHBT, int * isValid);

// void printPO(Tnode * node){
//     if (node == NULL){
//         return;
//     }
//     printf("%d,", node -> key);
//     printPO(node -> left);
//     printPO(node -> right);
// }

void evalTree(FILE * f){
    int isBST = 1;
    int isHBT = 1;
    int isValid = 1;
    int height;
    getNode(f, &height, &isBST, &isHBT, &isValid);
    printf("1,%d,%d\n", isBST, isHBT);
    // printPO(head);
}

int getNode(FILE * f, int * height, int * isBST, int * isHBT, int * isValid){
    int key;
    char c;
    if (!fread(&key, sizeof(int), 1, f) || !fread(&c, sizeof(char), 1, f)){
        *isValid = 0;
        return NULL;
    }
    int lHeight = 0, rHeight = 0;
    int childKey;
    if (c & 0x2) {
        childKey = getNode(f, &lHeight, isBST, isHBT, isValid);
        if (childKey > key){
            *isBST = 0;
        }
    }
    if (c & 0x1){
        childKey = getNode(f, &rHeight, isBST, isHBT, isValid);
        if (childKey < key){
            *isBST = 0;
        }
    }
    *height = lHeight > rHeight ? lHeight : rHeight;
    if (abs(lHeight - rHeight) > 2){
        *isHBT = 0;
    }
    if (key > HBT_MAX || key < HBT_MIN){
        *isValid = 0;
    }
    return key;
}
