#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

enum Type {OP, INT, FLOAT, PAREN};
//implement this as an enum

typedef union data{
    long long i;
    double f;
    char o;
} Data;

typedef struct node{
    Data data;
    enum Type type;
    struct node* left;
    struct node* right;
} Node;

bool preprocess(char **expressionPtr);
bool isNumeric(char character);
Node *parse(char *val);
Node *parseNum(char *str);
Node *createNode(enum Type type);
Node *pop(Node **stack, size_t* head);
void popop(Node **stack, size_t* head);
void printStack(Node **stack, size_t head);
void push(Node** stack, size_t *head, Node *node);
void printTree(Node *root);
double evaluate(Node* root);//base case: nothing there
bool containInt(Node* root, size_t* head);

int main(void){
   char* str = "6-2";
   puts(str);
   Node* tree1 = parse(str);
   printTree(tree1);
   str = "(6-2)^4";
   Node* tree2 = parse(str);
   puts(str);
   printTree(tree2);
   str = "-(6-2)^4+8/3";
   Node* tree3 = parse(str);
   puts(str);
   printTree(tree3);
   str = "-(6-2)^4+8 / 3";
   Node* tree4 = parse(str);
   puts(str);
   printTree(tree4);
   return 0;
}

Node* parse(char* val){
    Node* new;
    size_t start = 0, stop = 0;
    bool needToFree = preprocess(&val);
    size_t length = strlen(val);
    Node* stack[length];
    memset(stack, 0, sizeof(stack));
    size_t head = 0;
   
    for(size_t i = 0; i < length; ++i){
    //    printStack(stack,head);
        switch(val[i]){
            case '(':
                new = createNode(PAREN);
                new->data.o = '(';
                break;
            case'-':
               if(i==0 || (!isNumeric(val[i-1]) && val[i-1] != ')')){
                    new = createNode(OP);
                    new->data.o = '~';
                    break;
               }
            case'+':
                //while (+-, x/%, ^, -) pop
                while(head != 1 && stack[head-2]->type == OP)
                    popop(stack, &head);
                
            case 'x':
            case '/':
            case'%':
                while(head != 1 && stack[head-2]->type == OP 
                && stack[head-2]->data.o != '+' && stack[head-2]->data.o != '-'){
                popop(stack, &head);
                //while (+/%, ^, -) pop
            }
            case'^':
                new = createNode(OP);
                new->data.o = val[i];
                break;
            case ')':
                //pop to open parem
                while(stack[head-2]->type != PAREN)
                    popop(stack, &head);
                // can't just pop because that would pop off everything in the parentheses.
                new = pop(stack, &head);
                pop(stack,&head);
                push(stack, &head, new);
                continue;
           default:
                if(!isNumeric(val[i]))
                    continue;
                for(start = i; i<length && isNumeric(val[i]);++i);
                stop = i--;
                char num[stop-start+1];
                memset(num, 0, sizeof(num));
                strncpy(num, val+start, stop-start);
                new = parseNum(num);
        }
        push(stack, &head, new);

      }
       // print(stack, head); NOTE: just testing
        if(needToFree){
            free(val);
        }
        while(head>1){
            popop(stack, &head);
        }
      //  printStack(stack,head);
        return stack[0];
    }

bool preprocess(char** expressionPtr){
   bool needToFree = false;
   char* expression = *expressionPtr;
   for(size_t i = 0; expression[i] != '\0'; ++i){
        if((expression[i] == '(' && (isNumeric(expression[i-1]) || expression[i-1] == ')')) || 
         (isNumeric(expression[i]) && expression[i-1] == ')')){
          char* new = calloc(strlen(expression)+2,sizeof(char));
          strncpy(new, expression, i);
          strcat(new,"x");
          strcat(new, expression+i);
          if(needToFree)
            free(expression);
         else
            needToFree = false;
          expression = new;
          ++i;
        }
   }
   if(needToFree)    
       *expressionPtr = expression;
    return needToFree;
}

bool isNumeric(char character){
    return character == '.' || (character >= '0' && character <= '9');
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
    for(size_t i = 0; i<head; ++i){
        switch(stack[i]->type){
            case PAREN:
            case OP:
                printf("%c", stack[i]->data.o);
            case INT:
                printf("%lld ", stack[i]->data.i);
            case FLOAT:
                printf("%.4f ",stack[i]->data.f);
        }
    }
    puts("");
}

void popop(Node **stack, size_t* head){ 
        Node *right, *op, *left;
        right = pop(stack, head);
        op = pop(stack, head);
        if(op->data.o == '~'){
            op->data.o = '-';
            left = createNode(INT);
            left->data.i = 0;
        }
        else{
            left = pop(stack, head);
        }
        op->right = right;
        op->left = left;
        push(stack, head, op);
}

Node *parseNum(char *str){
    Node *left, *right, *new,*temp;
    size_t length = strlen(str);
    char* decimal = strstr(str, ".");
    
    if(decimal != NULL)
        *decimal = 0;
    if(decimal!=str){
        temp = createNode(INT); 
        temp->data.i = atoll(str);
        if(decimal == NULL|| decimal == str+length-1){
            return temp;
        }
    }    
    long long count = (long long)strlen(decimal+1);
    right = createNode(INT);
    right->data.i = count;
    left = createNode(INT);
    left->data.i = 10;
    new = createNode(OP);
    new->data.o= '^';
    new->left = left;
    new->right = right;
    right = new;
    left = createNode(INT);
    left->data.i = atoll(decimal+1);
    new = createNode(OP);
    new->data.o= '/';
    new->left = left;
    new->right = right;
    if(decimal != str){
        right = new;
     //       left = createNode(INT);
        new = createNode(OP);
        new->data.o = '+';
        new->left = temp;
        new->right = right;
    }
    return new;
}

void printTree(Node *root){
    static unsigned int depth = 0;
    if(root == NULL){
        return;
    }
    ++depth;
    
    printTree(root->right);
    
    for(unsigned int i = 1; i<depth; ++i){
        printf("   ");
    }
    switch(root->type){
         case PAREN:
         case OP:
              printf("%c\n", root->data.o);
              break;
         case INT:
             printf("%lld\n ", root->data.i);
             break;
         case FLOAT:
             printf("%f\n",root->data.f);
    }
    
    printTree(root->left);
    
    --depth;
    
    if(depth == 0)
        puts("---------------------------");
}

double evaluate(Node* root){
   if(root == NULL){
	return 0;
   }
   if(!root->left && !root->right){
	return root;
   }
   if(root->type == INT){
        return (double)root->data.i
   } //base case

   switch(root->type){
        case root->data.c == '-':
            return evaluate(root->right) - evaluate(root->left);
        case root->data.c == '+':
            return evaluate(root->right) + evaluate(root->left);
        case root->data.c == '/':
            return evaluate(root->right) / evaluate(root->left);
        case root->data.c == 'x':
            return evaluate(root->right) x evaluate(root->left);
        case root->data.c == '%':
            return evaluate(root->right) % evaluate(root->left);
        case root->data.c == '^':
            return Math.pow(evaluate(root->right), evaluate(root->left));
   	default:
 	    continue;
     }
}

bool containInt(Node* root){
    return  Math.floor(root->data.f) == Math.ceil(root->data.f);
}
