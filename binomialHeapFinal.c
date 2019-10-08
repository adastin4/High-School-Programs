#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <time.h>

typedef struct node{
    int data, degree;
    struct node *child, *sibling, *parent;

} Node;

Node* createHeap();
Node* newNode(int key);

Node* mergeHeap(Node *h1, Node *h2);
Node* heapUnion(Node *h1, Node *h2);
Node* insert(Node *h, Node* val);

Node* extractMin(Node *h);
void printNodes(Node *h);

Node *findNode(Node* h, int val);
void decreaseKey(Node *h, int val1, int val2);
Node* delete(Node* h, int val);
void destroyHeap(Node *h);
Node* revert(Node *h);

void swap(Node *x, Node *y);
bool heapEmpty(Node *h);
bool evaluate(Node* x, Node* y);
bool evaluate1(Node* x, Node* y);
bool evaluate2(Node* x, Node* y);

void swapNodes(Node *x, Node* y); 
Node* getsibling(Node* x);
void printMin(Node* x);

Node* returnH = NULL;

int main(void){
    srand(42);
    puts("\n\nSTARTING TESTING \n");
    puts("--------------------------------------------------------------------");
    puts("\nTesting on random values\n"); 
    Node *pq = newNode(23);
    Node *q = newNode(34);
    Node *z = newNode(45);
    Node *y = newNode(56);
    Node *a = newNode(74);
    Node *c = newNode(87);
    Node *p = newNode(14);

    delete(returnH, 87);
    /*for(i = 0; i < 12; ++i){
        b = newNode(i);
        h = insert(h, b);
    }*/
    returnH = insert(returnH, p);
    returnH = insert(returnH, pq);
    printNodes(returnH);
    returnH = insert(returnH, q);
    returnH = insert(returnH, z);
    returnH = insert(returnH, y);
    returnH = insert(returnH, a);
    returnH = insert(returnH, c);

    printNodes(returnH);
    
    decreaseKey(returnH, 23, 4);
    printNodes(returnH);
   
    delete(returnH, 56);

    printNodes(returnH);
     
    Node* b = extractMin(returnH);
   
    printMin(b);

    printNodes(returnH);
    
    Node* t = extractMin(returnH);
    
    printMin(t);
    
    printNodes(returnH);
    
    delete(returnH, 34);
    printNodes(returnH);
   
    Node* nb = extractMin(returnH);
    
    printMin(nb);
    
    printNodes(returnH);
    delete(returnH, 74);
    printNodes(returnH);
    delete(returnH, 87);
    printNodes(returnH);
   /* delete(h, 87);
    
    printNodes(h);

    puts("--------------------------------------------------------------------");
    puts("\nTesting on values 1-34 inserted\n");
    Node *l = NULL;
    
    for(int i = 1; i < 35; ++i){
        Node *k = newNode(i+1);
        l = insert(l, k);
    }
    
    printNodes(l);
   
    l = extractMin(l);
    printNodes(l);
    decreaseKey(l, 74, 1);
    printNodes(l);

    decreaseKey(l, 34, 1);
    printNodes(l);
    */
    return 0;
}

Node* newNode(int key){
    Node *temp = malloc(sizeof(Node));
    temp->data = key;
    temp->parent = NULL;
    temp->child = NULL;
    temp->sibling = NULL;
    temp->degree = 0;
    return temp;
}

Node* createHeap(){
    Node *temp = NULL;
    return temp;
}

Node* heapUnion(Node *h1, Node *h2){
        Node* nhead = createHeap();
        nhead = mergeHeap(h1, h2);
        
        if(nhead == NULL){
            return nhead;
        }

        Node *prev = NULL;
        Node *temp = nhead;
        Node *nextNode = getsibling(temp);

        while((temp->sibling) != NULL){
            if((temp->degree != (temp->sibling)->degree) || (((temp->sibling)->sibling != NULL) && temp->degree == ((temp->sibling)->sibling)->degree)){
                prev = temp;
                temp = nextNode;
            }
            else{
                if(temp->data > (temp->sibling)->data){
                    if(prev == NULL){
                        nhead = getsibling(temp);
                    }
                    else{
                        prev->sibling = getsibling(temp);
                    }

                    temp->parent = nextNode;
                    temp->sibling = nextNode->child;
                    nextNode->child = temp;
                    nextNode->degree++;
                    
                    temp = nextNode;
                }
                else{
                    temp->sibling = nextNode->sibling;
                    nextNode->parent = temp;
                    nextNode->sibling = temp->child;
                    temp->child = nextNode;
                    temp->degree++;
                }
            }
            nextNode = temp->sibling;
        }

        return nhead;
}

Node* mergeHeap(Node *h1, Node *h2){

    Node* heap = createHeap();
    Node* h1h = h1;
    Node* h2h = h2;
    
    if(h1h == NULL){
        heap = h2h;
    }
    else{

        if(h2h != NULL && h1h->degree > h2h->degree){
            heap = h2h;
        }
        else{
            heap = h1h;
        } 

    }
    while(evaluate1(h1h,h2h)){
        if(h1h->degree < h2h->degree){
            h1h = getsibling(h1h);
        }
        else if(h2h->degree == h1h->degree){
            Node *x = getsibling(h1h);
            h1h->sibling = h2h;
            h1h = x;
        }
        else{
            Node *y = getsibling(h2h);
            h2h->sibling = h1h;
            h2h = y;
        }
    }
    return heap;
}

Node* insert(Node *h, Node* val){
    Node* heap = createHeap();
    heap = val;
    h = heapUnion(h, heap);
    return h;
}

Node* extractMin(Node* h){ 
    Node* minimum = h;
    Node *currNode = minimum;
    Node* behindMin = NULL;
    Node* tempheap = NULL;
    if(minimum != NULL){
        puts("Extracting minimum ...");
    }
    else{ 
        puts("No minimum to extract.");
        return;
    }

    while(currNode->sibling != NULL){
        if((currNode->sibling)->data < minimum->data){ //found a value smaller than the current min
            behindMin = currNode;
            minimum = getsibling(currNode);
        }
        currNode = getsibling(currNode);
    }

    if(evaluate(behindMin, minimum->sibling)){
        h = NULL;
    }
    else if(behindMin == NULL){ 
        h = minimum->sibling;
    }
    else if (behindMin->sibling == NULL){
        behindMin = NULL;
    }
    else{
        behindMin->sibling = getsibling(minimum);
    }

    if(minimum->child != NULL){
        tempheap = revert(minimum->child);
        (minimum->child)->sibling = NULL;
    }

    returnH = heapUnion(h, tempheap);

    return minimum;
}

void printNodes(Node *h){
    if(!heapEmpty(h)){
         Node* placeholder = malloc(sizeof(Node));
         puts("Root Nodes:");
         placeholder = h;
         while(placeholder != NULL){
             printf("%d\t", placeholder->data);
             placeholder = getsibling(placeholder);
        }
        puts("");
        free(placeholder);
        return;
    } 
    puts("Heap is empty");
    puts("");
}

Node *findNode(Node* h, int val){
    Node* temp = NULL;
    Node* val1 = h;
    if(val1->data == val){
        temp = val1;
        return temp;
    }

    if(evaluate2(val1->sibling, temp)){
        temp = findNode(val1->sibling, val);
    }

    else if(evaluate2(val1->child, temp)){
        temp = findNode(val1->child, val);
    }
    return temp;
}

void decreaseKey(Node *h, int val1, int newk){
    Node* place1;
    Node* place2;
    Node *comp = findNode(h, val1);

    if(comp == NULL || comp->data <= newk){
        puts("No decrement can be made (key will not be decreased)");
        return;
    }
    else{
        comp->data = newk;
        place1 = comp;
        place2 = comp->parent;
    }

    while(place2 != NULL && place1->data < place2->data){
        swap(place1, place2);
        place1 = place2;
        place2 = place2->parent;
    }
}

Node* delete(Node* h, int val){
    int val1 = -1000000; //-1000000 is assumed to be lower than any value in the heap
    if(h != NULL){
       printf("Deleting %d.....\n", val);
       decreaseKey(h, val, val1); //large negative number to ensure that it will be min value removed
       Node* placeholder = extractMin(h); 

       if(placeholder == NULL){
           puts("Deletion unsuccessful (node might not be in binomial heap)");
        }
        else if (placeholder != NULL){
           puts("Deletion Successful");
        }
        else{
            puts("Deletion unsuccessful");
        }
        return;
    }
    puts("Heap is already empty");
}

void destroyHeap(Node *h){
      free(h);
}

void swap(Node* x, Node* y){
     int temp = 0;
     temp = x->data;
     x->data = y->data;
     y->data = temp;
}

Node* revert(Node *h){
    if(h->sibling != NULL){
      revert(h->sibling);
      (h->sibling)->sibling = h;
    }
    else{
        Node *hl = NULL;
        hl = h;
        return hl;
    }
}

bool evaluate(Node* x, Node* y){
    return x==NULL && y == NULL ? true : false;
}

bool heapEmpty(Node *h){
    bool val = false; 
    if(h == NULL)
        val = true;
        return val;
    return val;
}

void swapNodes(Node* a, Node*b){
       Node *temp = a->sibling;
       a->sibling = b; 
       a = temp;
}

Node* getsibling(Node* x){
    if(x->sibling != NULL)
         return x->sibling;
    else
        return;
}

bool evaluate1(Node* x, Node* y){
    return x != NULL && y != NULL ? true : false;
}

bool evaluate2(Node* x, Node* y){
    return x != NULL && y == NULL ? true : false;
}
void printMin(Node* x){
    printf("The minimum value (removed during extraction) was: %d\n", x->data);
}
