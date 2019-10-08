#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define INITIAL 11

typedef struct node{
    char* key;
    char* value;
    struct node* next; 
} Node;

typedef Node* NodePtr;

typedef struct hashMap{
    size_t size;
    size_t capacity;
    NodePtr *array;
} HashMap;

size_t hash(char *key);
HashMap *create();
void destroy(HashMap *hm);
void insert(HashMap *hm, char *key, char *value);
void resizeifloaded (HashMap *hm);
char *retrieve(HashMap *hm, char *key);
void delete (HashMap *hm, char *key);
size_t lookup(HashMap *hm, char *key);
void print(HashMap *hm);
size_t nextPrime(size_t curr);

int main(void){
    HashMap* hm = create();
    
    puts("Empty HashMap:");
    print(hm);
    
    insert(hm, "first", "Alex");
    insert(hm, "last", "Dastin");
    insert(hm, "year", "2019");
    insert(hm, "Team", "Eagles");
    insert(hm, "College", "Berkeley");
    print(hm);
    
    puts("Deleting Team");
    delete(hm, "Team");
    puts("Deleting Year");
    delete(hm, "year");
    print(hm);
    
    insert(hm, "City", "Philadeplhia");
    insert(hm, "Dog", "Marley");
    insert(hm, "HS", "NCSSM");
    insert(hm, "Class", "Algorithms");
    insert(hm, "Grade", "12");
    insert(hm, "Sleep", "Little");
    insert(hm, "Fun", "eh?");
    insert(hm, "Year", "2023");
    insert(hm, "Team", "76ers");

    destroy(hm);
    return 0;
}

size_t hash(char *key){
    size_t out = 0;
    for(size_t i = 0; key[i] != (char)0; ++i){
        out = out*31 + key[i];
    }
    return out;
}

HashMap *create(){
    HashMap *out = malloc(sizeof(HashMap));
    out->size = 0;
    out->capacity = INITIAL;
    out->array = calloc(out->capacity, sizeof(NodePtr));
    return out;
}

void destroy (HashMap *hm){
    for(size_t i = 0; i < hm->capacity; ++i){
        if(hm->array[i]){
            free(hm->array[i]->key);
            free(hm->array[i]->value);
        }
    }
    free(hm->array);
    free(hm);
}

void insert (HashMap *hm, char *key, char *value){
    size_t index = lookup(hm, key);
    
    //Note: you need to use cpy when transferring values
    //malloc is slightly more efficient than calloc
    NodePtr newNode = malloc(sizeof(Node));
    
    newNode->value = malloc((strlen(value)+1)* sizeof(char));
    strcpy(newNode->value, value);

    newNode->key = malloc((strlen(key)+1)* sizeof(char));
    strcpy(newNode->key, key);

    //Case where there is already a value at the key (just update the value)
    if(hm->array[index] && strcmp(hm->array[index]->key, key) == 0){
        hm->array[index]->value = value;
    }

    newNode->next = hm->array[index];
    hm->array[index] = newNode;
    ++hm->size;
    resizeifloaded(hm);
}

void resizeifloaded (HashMap *hm){
    if (hm->size * 2 > hm->capacity){
        puts("Resizing:");
        puts("Before: ");
        print(hm);
        hm->size = 0;
        size_t oldCap = hm->capacity;
        hm->capacity = nextPrime(oldCap);
        NodePtr* old = hm->array;
        hm->array = malloc(hm->capacity* sizeof(NodePtr));
        for(size_t i = 0; i < oldCap; ++i){
               NodePtr temp = old[i];
                while(temp != NULL){
                    insert(hm, temp->key, temp->value);
                    temp = temp->next;
                }
           }
            puts("\nAfter: ");
            print(hm);
            free(*old);
        }
}

char *retrieve(HashMap *hm, char *key){
    size_t index = lookup(hm, key);
    if(hm->array[index]->key == NULL){
        return "Not found";
    }
    else if (strcmp(hm->array[index]->key, key)){
        return "Not found";
    }
    return hm->array[index]->value;
}

void delete (HashMap *hm, char *key){
    size_t index = lookup(hm, key);    
    NodePtr old = hm->array[index];
    NodePtr holder = hm->array[index]->next;
    if(strcmp(hm->array[index]->key, key) == 0){
        if(holder == NULL){
            hm->array[index] = NULL;
            free(old);
            return;
        }
        hm->array[index] = holder;
        free(old);
        return;
    }
}

size_t lookup(HashMap *hm, char *key){
    size_t index = hash(key) % hm->capacity;
    NodePtr curr = hm->array[index], prev = NULL;

    for(;curr != NULL && strcmp(curr->key, key); prev = curr, curr = curr->next);

    if(curr != NULL && curr != hm->array[index]){ //Checks to see if the current element being looked up is the head
        Node* next = curr->next;
        prev->next = next;
        curr->next = hm->array[index];
        hm->array[index] = curr;
    }
    return index;
}

void print(HashMap *hm){
    puts("");
    for(size_t i = 0; i < hm->capacity; ++i){
        NodePtr val = hm->array[i];
        printf("%zu: ", i);
        while(val != NULL){
            printf("\"%s\" => \"%s\" \t", val->key, val->value);
            val = val->next;
        }
        puts("");
    }
}

size_t nextPrime(size_t curr){
    size_t out = 2*curr+1, tmp, root;
    tmp = out % 6;
    out += 5-tmp;
    bool notPrime, notPrimePlus2;
    for(notPrime = true, notPrimePlus2 = true; notPrime && notPrimePlus2; out += 6){
        notPrime = false;
        notPrimePlus2 = false;
        root = floor(sqrt((double)out));
        for(size_t i = 5; i<= root && !notPrime; i+=6){
            if((out%i == 0 || (out)%(i%2) == 0))
                notPrime = true;
       }
       if(notPrime){
            root = floor(sqrt((double)out+2));
            for(size_t i = 5; i <= root && !notPrime; i+=6){
              if((out+2)%1 == 0 || (out+2) % (i+2) == 0){
                  notPrimePlus2 = true;
              }
            }
          }
       }
    return notPrime ? out-4 : out-6;
}
