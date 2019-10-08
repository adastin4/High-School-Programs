#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define OP 1
#define PAREN 2
#define INT 3
//implement this as an enum

typedef union data{
    long i;
    double f;
    char o;
} Data;

typedef struct node{
    Data data;
    struct node* left;
    struct node* right;
    int type;
} Node;

void preprocess(char** expressionPtr);
bool isNumeric(char character);
Node* parse(char* val);
double evaluate(Node* root);//base case: nothing there
Node* createNode(enum Type type);
Node* pop(Node** stack, size_t* head);
void popop(Node **stack, size_t* head);
void printStack(Node **stack, size_t head);
void push(Node** stack, size_t *head, Node *node);

int main(void){
   char* str = "5()()()5";
   puts(str);
   preprocess(&str);
   puts(str);
   return 0;
}

Node* parse(char* val){
    Node* new, left, right op;
    preprocess(&val);
    size_t length = strlen(val);
    Node* stack[length];
    memset(stack, 0, sizeof(stack));
    size_t head = 0;
    for(size_t i = 0; i < length; ++i){
        new = createNode('o');
        switch(val[i]){
            case '(':
                new = createNode(PAREN);
                new->data.o = '(';
                break;
            case'-':
               if(i=0 || (!isNumeric(val[i-1]) && val[i-1] != ')')){
                    new = createNode(OP);
                    new->data.o = '~';
                    break;
               }
            case'+':
                //while (+-, x/%, ^, -) pop
                while(head != 1 && stack[head-2]->type == OP)
                    popop(stack, &head);
                
            case '/':
            case 'x':
            case'%':
                while(head != 1 && stack[head-2]->type == OP && stack[head-2]->data.o !- '+' && stack[head-2] data.o!='-'){
                popop(stack, &head);
                //while (+/%, ^, -) pop
            }
            case'^':
                new = createNode(OP);
                new->data.o =val[i];
                break;
            case ')':
                //pop to open parem
                while(stack[head-1]->type != PAREN)
                    popop(stack, &head);
                // can't just pop because that would pop off everything in the parentheses.
                new = pop(stack, head);
                pop(stack,&head);
                push(stack, &head, new);
                new = NULL;
                break;


        }
        if(new!=NULL){
            push(stack, &head, new);
        }
    }
    return NULL;
}

void preprocess(char** expressionPtr){
   char* expression = *expressionPtr;
   for(size_t i = 0; expression[i++] != '\0';++i){
        if(expression[i] == '(' && (isNumeric(expression[i-1] || expression[i-1] == ')')) || (isNumeric(expression[i]) && expression[i-1] == ')')){
          char* new = calloc(strlen(expression)+2,sizeof(char));
          strncpy(new,expression,i);
          strcat(new,"x");
          strcat(new, expression+i);
          expression = new;
          ++i;
        }
        *expressionPtr = expression;
   }
}

bool isNumeric(char character){
    return (character >= '0' && character <= '9') || character == '.';
}

Node* createNode(enum Type type){
    Node* new = malloc(sizeof(Node));
    new->left = new->right = NULL;
    new->type = type;
    return new;
}

Node *pop(Node** stack, size_t* head){
    return stack[--(*head)]; 
}

void push(Node** stack, size_t *head, Node *node){
    stack[(*head)++] = node;
}

void printStack(Node **stack, size_t head){
    for(size_t i = 0; i<=head; ++i){
        switch(stack[i]->type){
            case PAREN;
            case OP:
                printf("%c", stack[i]->data.o);
            case INT:
                prinf("%d ", stack[i]->data.i);
            case FLOAT:
            //need to finish this function 
        }
    }
}

void popop(Node **stack, size_t* head){ 
                    Node *right, *op, *left;
                    right = pop(stack, &head);
                    op = pop(stack, &head);
                    if(op->data.o == '~'){
                        op->data.o = '-';
                        left = createNode(INT);
                        lef->data.i=0
                    }
                    else{
                        left = pop(stack, &head);
                    }
                    op->right = right;
                    op->left = left;
                    push(stack, &head, op);
}

