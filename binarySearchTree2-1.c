#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node{
    char key;
    struct node* left;
    struct node* right;
} Node;

typedef Node* NodePtr;

void insert(char key, NodePtr* rootPtr);
NodePtr find(char key, NodePtr root);
bool delete(char key, NodePtr* rootPtr);
void destroy(NodePtr* rootPtr); 
void print(NodePtr root);
bool isEmpty(NodePtr root);
NodePtr* getPreceding(NodePtr* rootPtr);
void printfind(NodePtr x, char key);

int main(void){
    NodePtr root = NULL; //create a new tree
    insert('k', &root);
    insert('a', &root);
    insert('q', &root);
    insert('z', &root);
    insert('y', &root);
    puts("Populated root:");
    print(root);
    puts("Elements will be removed one-by-one");
    delete('a', &root);
    print(root);
    puts("");
    delete('k', &root);
    print(root);
    puts("");
    delete('q', &root);
    print(root);
    puts("");
    delete('z', &root);
    print(root);
    puts("");
    delete('y', &root);
    print(root);
    if(isEmpty(root)){
        puts("The tree is Empty\n");
    }
    puts("REPOPULATING");
    insert('k', &root);
    insert('a', &root);
    insert('q', &root);
    insert('z', &root);
    insert('y', &root);
    puts("\nPopulated root:");
    print(root);
    NodePtr x = find('a',root);
    printfind(x, 'a');
    NodePtr y = find('c',root);
    printfind(y,'c');
    puts("\nDESTROYING\n");
    destroy(&root);
    if(isEmpty(root)){
        puts("The tree is Empty");
    }
    return 0;
}

void insert(char key, NodePtr* rootPtr){
    if(isEmpty(*rootPtr)){
        //create a new node, assign it to root
        NodePtr rootPtrNew = malloc(sizeof(Node));
        rootPtrNew->key = key;
        rootPtrNew->left = NULL;
        rootPtrNew->right = NULL;
        *rootPtr = rootPtrNew;
    }
    else{
        if(key<= (*rootPtr)->key){
            insert(key,&(*rootPtr)->left);//may need & instead of *
        }
        else if(key> (*rootPtr)->key){
            insert(key,&(*rootPtr)->right);//may need & instead of *
        }
    }
}

NodePtr find(char key, NodePtr rootPtr){
    if(isEmpty(rootPtr))
        return NULL;
    if(key == rootPtr->key)
        return rootPtr;
    if(key < rootPtr->key)
        return find(key,rootPtr->left);
    else
        return find(key,rootPtr->right);
}

bool delete(char key, NodePtr* rootPtr){
    if(isEmpty(*rootPtr)){
        return false;   
    }
   else if((*rootPtr)->key > key){
        return delete(key,&((*rootPtr)->left)); //may need & instead of * (might need to remove)
    }
    else if((*rootPtr)->key < key){
        return delete(key,&((*rootPtr)->right)); //may need & instead of * (might need to remove)
    }
    else if(isEmpty((*rootPtr)->left) && isEmpty((*rootPtr)->right)){
        free(*rootPtr);
        *rootPtr = NULL;
        return true;
    }
    else if(isEmpty((*rootPtr)->left) || isEmpty((*rootPtr)->right)){
        if(isEmpty((*rootPtr)->left)){
              NodePtr temp = *rootPtr;
              *rootPtr = (*rootPtr)->right;
              free(temp);
              return true;
        }
        if(isEmpty((*rootPtr)->right)){
              NodePtr temp = *rootPtr;
              *rootPtr = (*rootPtr)->left;  
              free(temp);
              return true;
        }
   }     
   else{//two children
        NodePtr* preceding = getPreceding(&(*rootPtr)->left);
        NodePtr temp = *preceding;
        *preceding = temp->left;
        temp->left = (*rootPtr)->left;
        temp->right = (*rootPtr)->right;
        free(*rootPtr);
        *rootPtr = temp;
        return true;
      /*  NodePtr new = malloc(sizeof(Node));
        memcpy(new,*preceding, sizeof(Node));
        new->left = (*rootPtr)->left;
        new->right = (*rootPtr)->right;
        free(*rootPtr);
        *rootPtr = new;
        return delete((*preceding)->key, preceding);*/
        //copy root's children to newRoot
     }
    return false; 
}

void destroy(NodePtr* rootPtr){
    if(isEmpty(*rootPtr)){
        return;
    }
    destroy(&(*rootPtr)->left);
    destroy(&(*rootPtr)->right);
    free(*rootPtr);
    *rootPtr = NULL;
}

void print(NodePtr root){
    static unsigned int depth = 0;
    if(isEmpty(root)){
        return;
    }
    ++depth;
    print(root->right);
    --depth;
    for(unsigned int i = 0; i<depth; ++i){
        printf("  ");
    }
    printf("%c\n", root->key);
    ++depth;
    print(root->left);
    --depth;
}

bool isEmpty(NodePtr root){
    return root == NULL;
}


NodePtr* getPreceding(NodePtr* rootPtr){ //only happens when there are 2 children
   if(!isEmpty((*rootPtr)->right))
        return getPreceding(&((*rootPtr)->right));
    return rootPtr; 
}

void printfind(NodePtr x, char key){
    printf("Searching for Node %c\n",key);
    if(x!=NULL){
        printf("\nFound Node %c\n", x->key);
        if(x->left!=NULL){
            printf("Left Child: %c\n", x->left->key);
        }
        if(x->right!=NULL){
            printf("Right Child: %c\n", x->right->key);
        }
        puts("");
    }
    else{
        printf("\nNode %c not found\n", key);
    }
}
