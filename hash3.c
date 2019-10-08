#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define INITIAL 11

typedef struct node{
    char* key;
    char* value;
    bool deleted;
} Node;

typedef struct hashMap{
    size_t size;
    size_t capacity;
    Node *array;
} HashMap;

size_t hash(char *key);
HashMap *create();
void destroy (HashMap *hm);
void insert (HashMap *hm, char *key, char *value);
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
    out->array = calloc(out->capacity, sizeof(Node));
    return out;
}

void destroy (HashMap *hm){
    for(size_t i = 0; i < hm->capacity; ++i){
        if(hm->array[i].key){
            free(hm->array[i].key);
            free(hm->array[i].value);
        }
    }
    free(hm->array);
    free(hm);
}

void insert (HashMap *hm, char *key, char *value){
    size_t index = lookup(hm, key);
    bool exists = hm->array[index].key;
    hm->array[index].deleted = false;
    if(exists){
        free(hm->array[index].value);
    }
    hm->array[index].value = calloc((strlen(value)+1), sizeof(char));
    strcpy(hm->array[index].value, value);
    if (!exists){
        hm->array[index].key = calloc((strlen(key)+1), sizeof(char));
        strcpy(hm->array[index].key, key);
        ++hm->size;
        resizeifloaded(hm);
    }
}

void resizeifloaded (HashMap *hm){
    if (hm->size * 2 > hm->capacity){
        puts("Resizing:");
        puts("Before: ");
        print(hm);
        hm->size = 0;
        size_t oldCap = hm->capacity, index;
        hm->capacity = nextPrime(oldCap);
        Node *old = hm->array;
        hm->array = calloc(hm->capacity, sizeof(Node));
        for(size_t i = 0; i < oldCap; ++i){
            if(old[i].key){
                if(old[i].deleted){
                    free(old[i].key);
                    free(old[i].value);
                }
                else{
                  //Need to copy keys, can't reassign (can't do: hm->array[index].key = old[i].key)
                    index = lookup(hm, old[i].key);
                    hm->array[index].key = calloc(strlen(old[i].key)+1 , sizeof(char));
                    strcpy(hm->array[index].key, old[i].key);
                    hm->array[index].value = calloc(strlen(old[i].value)+1, sizeof(char));
                    strcpy(hm->array[index].value, old[i].value);
                    ++hm->size;
                }
           }
        }
        puts("\nAfter: ");
        print(hm);
        free(old);
   }
}

char *retrieve(HashMap *hm, char *key){
    size_t index = lookup(hm, key);
    if(hm->array[index].deleted){
        return "Element has been deleted";
    }
    else if(hm->array[index].key == NULL){
        return "Not found";
    }
    else if (strcmp(hm->array[index].key, key)){
        return "Not found";
    }
    return hm->array[index].value;
}

void delete (HashMap *hm, char *key){
    size_t index = lookup(hm, key);
    if(hm->array[index].key){
        hm->array[index].deleted = true;
        --hm->size;
    }
}

size_t lookup(HashMap *hm, char *key){
    size_t index, hashed = hash(key);
    index = hashed % hm->capacity;
    char* nodeKey = hm->array[index].key;

    for(size_t i = 0; nodeKey != NULL && strcmp(nodeKey, key); ++i){
       index = (hashed + i*i) % hm->capacity;
       nodeKey = hm->array[index].key;
    }
    return index;
}

void print(HashMap *hm){
    Node bucket;
    puts("");
    for(size_t i = 0; i < hm->capacity; ++i){
        bucket = hm->array[i];
        printf("%zu: ", i);
        printf("%c \"%s\" => \"%s\" \n", bucket.deleted ? 'D': ' ', bucket.key, bucket.value);
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
