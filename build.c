#include <stdio.h>
#include <stdlib.h>
#include "hbt.h"

Tnode * insertNode(Tnode * head, int key);
Tnode * rebalance(Tnode * ya, Tnode * child);
void replaceNode(Tnode * n);
Tnode * deleteNode(Tnode * head, int key, int * bShift);

void printPO(Tnode * n){
    if (n == NULL) {
        return;
    }
    printf("%d\n", n -> key);
    printPO(n -> left);
    printPO(n -> right);
}

void freeTree(Tnode * head){
    if (head != NULL){
        freeTree(head -> left);
        freeTree(head -> right);
    }
    free(head);
}

void buildTree(FILE * f_in, FILE * f_out){
    int key;
    char c;
    Tnode * head = NULL;
    while (fread(&key, sizeof(int), 1, f_in) && fread(&c, sizeof(char), 1, f_in)){
        if (c == 'i'){
            printf("insert: %d\n", key);
            head = insertNode(head, key);
        }
        else if (c == 'd')
        {
            printf("delete: %d\n", key);
            int dummy;
            head = deleteNode(head, key, &dummy);

        }
        
    }
    printPO(head);
    freeTree(head);
}

Tnode * CCR(Tnode * old_root) {
    Tnode * new_root = old_root->right;
    old_root->right = new_root->left;
    new_root->left = old_root;
    return new_root;
}

Tnode * CR(Tnode * old_root) {
    Tnode * new_root = old_root->left;
    old_root->left = new_root->right;
    new_root->right = old_root;
    return new_root;
}

Tnode * insertNode(Tnode * head, int key){
    Tnode *ya = head, *curr = head; // ya: youngest ancestor with balance 1/-1
    Tnode *pya = NULL, *pcurr = NULL; // pya: parent of youngest ancestor
    Tnode *q;
    while (curr != NULL)
    {    
        if (key <= curr -> key)
        {
            q = curr->left;
        }
        else
        {
            q = curr->right;
        }
        if (q != NULL && q->balance != 0) // keep track of youngest
        {
            pya = curr; // ancestor and its parent
            ya = q;
        }
        pcurr = curr;
        curr = q;
    }
    q = malloc(sizeof(Tnode));
    if (!q){
        printf("Malloc error!");
        return NULL;
    }
    q -> left = NULL;
    q -> right = NULL;
    q -> key = key;
    q -> balance = 0;
    if (pcurr == NULL)
    {
        head = q;
            return head;
    }
    else
    {
        if (key <= pcurr->key)
        {
            pcurr->left = q;
        }
        else
        {
            pcurr->right = q;
        }
        // update the balance from youngest ancestor to
        // parent of the new node
        curr = ya;
        while (curr != q)
        {   
            if (key <= curr -> key)
            {
                curr -> balance += 1; // insert on left
                curr = curr -> left;
            }
            else
            {
                curr -> balance -= 1; // insert on right
                curr = curr->right;
            }
        }

        // check if balancing is required
        // balance of ya = -1, 0, or 1, no balancing
        if ((ya->balance < 2) && (ya->balance > -2))
        {
            return head;
        }
        // find the child into which q is inserted
        Tnode * child;
        if (key <= ya -> key)
        {
            child = ya->left;
        }
        else
        {
            child = ya->right;
        }
        // the subtree rooted at ya needs balancing
        // curr points to the new root of the subtree
        // pya has to point to curr after rebalancing

        // both ya and child are unbalanced in the same
        // direction

        if ((ya -> balance == 2) && (child -> balance == 1))
        {
            curr = CR(ya); // clockwise rotation
            ya -> balance = 0;
            child -> balance = 0;
        }
        if ((ya -> balance == -2) && (child -> balance == -1))
        {
            curr = CCR(ya); // counter-clockwise rotation
            ya -> balance = 0;
            child -> balance = 0;
        }

        // ya and child are unbalanced in opp. Directions
        if ((ya->balance == 2) && (child->balance == -1))
        {
            ya->left = CCR(child); // counter-clockwise rotation
            curr = CR(ya); // clockwise rotation
            if (curr->balance == 0) // curr is inserted node q
            {
                ya->balance = 0;
                child->balance = 0;
            }
            else
                if (curr->balance == 1) // ori. left subtree of curr
                {
                    ya->balance = -1; // contains q
                    child->balance = 0;
                }
                else // ori. right subtree of curr
                {
                    ya->balance = 0; // contains q
                    child->balance = 1;
                }
                curr->balance = 0; // new root is balanced
        }
        if ((ya->balance == -2) && (child->balance == 1))
        {    
            ya->right = CR(child); // clockwise rotation
            curr = CCR(ya); // counter-clockwise rotation
            if (curr->balance == 0) // curr is inserted node q
            {
                ya->balance = 0;
                child->balance = 0;
            }
            else
            {    
                if (curr->balance == -1) // ori. right subtree of curr
                {    
                    ya->balance = 1; // contains q
                    child->balance = 0;
                }
                else // ori. left subtree of curr
                {
                    ya->balance = 0; // contains q
                    child->balance = -1;
                }
            }
            curr->balance = 0; // new root is balanced
        }
        if (pya == NULL)
        {    
            head = curr;
        }
        else
        {
            if (key <= pya-> key)
            {
                pya->left = curr;
            }
            else
            {
                pya->right = curr;
            }
        }
        return head;
    }
}

Tnode * deleteNode(Tnode * head, int key, int * bShift){
    if (head -> key == key){ // base case where head is node to be deleted
        if (head -> left != NULL && head -> right != NULL) { // Two Child case
            replaceNode(head);
            head = deleteNode(head, key, bShift);
            return head;
        }
        else if (head -> left == NULL && head -> right == NULL){ // No Child case
            free(head);
            *bShift = -1;
            return NULL;
        }
        else //One Child Case
        {
            *bShift = -1;
            return head -> left == NULL ? head -> right : head -> left;
        }
    }

    //standard recursive case
    Tnode * child;
    //int prevBal = head -> balance;
    if (head -> key > key)
    {
        head -> left = deleteNode(head -> left, key, bShift);
        head -> balance += *bShift;
        child = head -> left;
    }
    else if (head -> key < key)
    {
        head -> right = deleteNode(head -> right, key, bShift);
        head -> balance -= *bShift;
        child = head -> right;
    }
    if (head -> balance > 1 || head -> balance < 1){
        rebalance(head, child);
    }
    if (head -> balance == 0){
        *bShift = -1;
    }
    return head;
}

void replaceNode(Tnode * n){
    Tnode * target = n -> right;
    while (n -> left != NULL){
        n = n -> left;
    }
    int temp = n -> key;
    n -> key = target -> key;
    target -> key = temp;
}

Tnode * rebalance(Tnode * ya, Tnode * child){
    // check if balancing is required
    // balance of ya = -1, 0, or 1, no balancing
    if ((ya -> balance < 2) && (ya -> balance > -2))
    {
        return ya;
    }
    // the subtree rooted at ya needs balancing
    // curr points to the new root of the subtree
    // pya has to point to curr after rebalancing

    // both ya and child are unbalanced in the same
    // direction
    Tnode * curr;
    if ((ya -> balance == 2) && (child -> balance == 1))
    {
        curr = CR(ya); // clockwise rotation
        ya -> balance = 0;
        child -> balance = 0;
    }
    if ((ya -> balance == -2) && (child -> balance == -1))
    {
        curr = CCR(ya); // counter-clockwise rotation
        ya -> balance = 0;
        child -> balance = 0;
    }

    // ya and child are unbalanced in opp. Directions
    if ((ya->balance == 2) && (child->balance == -1))
    {
        ya->left = CCR(child); // counter-clockwise rotation
        curr = CR(ya); // clockwise rotation
        if (curr->balance == 0) // curr is inserted node q
        {
            ya->balance = 0;
            child->balance = 0;
        }
        else
            if (curr->balance == 1) // ori. left subtree of curr
            {
                ya->balance = -1; // contains q
                child->balance = 0;
            }
            else // ori. right subtree of curr
            {
                ya->balance = 0; // contains q
                child->balance = 1;
            }
            curr->balance = 0; // new root is balanced
    }
    if ((ya->balance == -2) && (child->balance == 1))
    {    
        ya->right = CR(child); // clockwise rotation
        curr = CCR(ya); // counter-clockwise rotation
        if (curr->balance == 0) // curr is inserted node q
        {
            ya->balance = 0;
            child->balance = 0;
        }
        else
        {    
            if (curr->balance == -1) // ori. right subtree of curr
            {    
                ya->balance = 1; // contains q
                child->balance = 0;
            }
            else // ori. left subtree of curr
            {
                ya->balance = 0; // contains q
                child->balance = -1;
            }
        }
        curr->balance = 0; // new root is balanced
    }
    return curr;
}
