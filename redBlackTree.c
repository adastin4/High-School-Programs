/* Name: Alex Dastin
 * 
 * Class: CS436 Data Structures
 * 
 * Who I worked with: Noone
 * 
 * What my project is: A Red-Black Tree (a type of Self-Balancing Binary Search Tree)  
 * 
 * How it Works: There are numerous functions that this program implements to maintain its 
 * self-balanced nature. The properties that the tree had to maintain at all times are that 
 * each node has to be either red or black, the root is black, all leaves (NULL) are black, 
 * if the node is red, both of its children are black, and every path from a given node to its
 * descendent nodes contains the same number of black nodes. A function was implemented to 
 * verify that the tree contained all of these characteristics upon insertion and deletion. The 
 * major functions that are used to balance the tree are those involved in insertion, those 
 * involved in deletion, and those needed to get information about other nodes relative to the 
 * current node of insertion or deletion. Functions for rotation (rotateRight and rotateLeft)
 * along with those to pull information about nodes and their data relative to the root node of 
 * investigation (namely: parent, grandparent, uncle, and sibling) were used to account for
 * unique cases in both insertion and deletion. In deletion the main cases that were considered
 * were deletion when the node being deleted is root, that when there is one child that is a leaf
 * , and that where both children were not leafs. Insertion essentially entails inserting data
 * into a right subtree if it is larger than the root of question, and inserting data into a left
 * subtree if it is smaller than the point of question. Then, after insertion is complete, 
 * the tree is rebalanced by accounting for unique cases in the program (detailed below).
 * 
 * Challenges Encountered: There were quite a few problems and difficulties that I ran into 
 * throughout my attempts to properly implement my RedBlack Tree. My first significant problem
 * came when implementing my insertion rebalance function. Although I was able to effectively 
 * implement my insertBST function as well as the first few cases in the rebalance function, I
 * struggled to properly implement my rotation functions. A primary reason behind this was that I
 * did not initially understand the rotations to their fullest extent and how they related to 
 * maintaining properties in a Red-Black tree. Upon turning to online sources, however, I was 
 * able ot better understand the function and properly implement it into my program. The next 
 * major problems that I encountered came when attempting to implement my deletion function.
 * Although it functioned properly for some cases, my deletion failed to account for the case
 * where the node being removed is the root (when the children for the root are leafs). I 
 * overcame this difficulty by creating a function separate from my recursive deletion function
 * to account for the case where the node being removed was the root. The other major problem
 * that I ran into when attempting to implement my deletion function came when I was performing
 * rotations on the root node. The problem was that I wasn't setting the root of the tree to 
 * the correct value upon rotating, and I overcame this problem through tracing my code line-by-
 * line and identifying the problem in the 6th case of my singlechild deletion function. In 
 * order to set my node to the proper value, I created a function to do so, and my problem was 
 * resolved. Although there were many difficulties that I came across when implementing my 
 * red-black tree, I learned a lot along the way and feel that I have improved my abilities as 
 * a debugger and a programmer.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node{
    int data;
    char color; 
    struct node *left;
    struct node *right;
    struct node *parent;
} Node;

Node temp = {0, 'B', NULL, NULL, NULL};
Node *LEAF = &temp; //substitute for isLeaf

Node *newNode(int data);
void printTree(Node *tree);
void clearTree(Node **tree);
void replace(Node* node, Node* r); 

void insert (Node **tree, int data);
void insertBST(Node **tree, Node *new);
void rotateRight(Node **node);
void rotateLeft(Node **node);
void rebalance(Node **tree, Node *new);

void delete(Node **tree, int data); 
void deleteRecur(Node **tree, int data);
void deleteSingle(Node **node);
void deletec1(Node *node);
void deletec2(Node *node);
void deletec3(Node *node);
void deletec4(Node *node);
void deletec5(Node *node);
void deletec6(Node *node);
void setRoot(Node **tree);
Node* inOrderPredecessor(Node *node);

bool isRedBlackTree(Node **tree); //Used to verify if the tree follows properties of RBT
bool leavesBlack_RedHasBlackChildren(Node **tree);
bool sameNumberBlack(Node **tree);

Node* parent(Node* n);
Node* grandparent(Node* n);
Node* sibling(Node *n);
Node* uncle(Node *n); 

int main(void){
    Node *tree = NULL; 
    puts("\nInserting into the tree:");
    insert(&tree, 41);
    printTree(tree);
    puts("-----------------------------------------");
    insert(&tree, 32);
    printTree(tree);
    puts("-----------------------------------------");
    insert(&tree, 28);
    printTree(tree);
    puts("-----------------------------------------");
    insert(&tree, 12);
    printTree(tree);
    puts("-----------------------------------------");
    insert(&tree, 19);
    printTree(tree);
    puts("-----------------------------------------");
    insert(&tree, 62);
    printTree(tree);
    puts("-----------------------------------------");
    
    puts("Deleting from the tree:");
    delete(&tree, 41);
    printTree(tree);
    puts("-----------------------------------------");
    delete(&tree, 62);
    printTree(tree);
    puts("-----------------------------------------");
    delete(&tree, 32);
    printTree(tree);
    puts("-----------------------------------------");
    delete(&tree, 28);
    printTree(tree);
    puts("-----------------------------------------");
    delete(&tree, 12);
    printTree(tree);
    puts("-----------------------------------------");
    delete(&tree, 19);
    printTree(tree);
    puts("-----------------------------------------");
   
    puts("Randomly Inserting into tree:");
    srand(42);
    for(int i = 0; i < 10; ++i){
        insert(&tree, rand());
        printTree(tree);
        puts("-----------------------------------------");
    }  
    puts("Checking if Red-Black Tree: (1 means it is an RBT, 0 means it isn't)");
    printf("%d\n", isRedBlackTree(&tree));
    puts("Clearing Tree...\n");
    clearTree(&tree);
    puts("Tree:");
    printTree(tree);
    return 0;
}

Node *newNode(int data){
    Node *temp = malloc(sizeof(Node));
    temp->data = data;
    temp->color = 'R';
    temp->left = LEAF;
    temp->right = LEAF;
    temp->parent = NULL;
    
    return temp;
}

void printTree(Node *root){
    static int depth = 0;
    if(root == NULL){
        puts("");
        return;
    }
    ++depth;
    printTree(root->right);
    --depth;
    for(int i = 0; i < depth; ++i){
        printf("      ");
    }

    if(root->color == 'R'){
        printf("\x1b[31m%d\x1b[0m\n", root->data);
    }

    else{
        if(root == LEAF){
           printf("NULL\n\n");
        }
        else{
            printf("%d\n", root->data);
            puts("");
        }
    }

    ++depth;
    printTree(root->left);
    --depth;
}

void clearTree(Node **tree){
    //base case
     if((*tree) == LEAF){
        return;
     }
     clearTree(&((*tree)->left));
     clearTree(&((*tree)->right));
     free(*tree);
     *tree = NULL;
}

void replace(Node* node, Node* r){ 
//Similar to the typical replacement algorithm (with BST)
    if(node->parent!=NULL)
        r->parent = node->parent;
    if(node->parent->left != NULL && node == node->parent->left)
        node->parent->left = r;
    else
        node->parent->right = r;
}

void insert(Node **tree, int data){
    Node *new= newNode(data);
    insertBST(tree, new);
    rebalance(tree, new);
    while(new->parent != NULL){
        new = new->parent;
        //sets the tree back to root
    }
    *tree = new;
}

void insertBST(Node **tree, Node *new){
   if(*tree == NULL){
        new->color = 'B';
        *tree = new;
        return;
   }

   if((*tree)->data > new->data){
        if((*tree)->left != LEAF){
            return insertBST(&((*tree)->left), new);
        }
        else{
            new->parent = *tree;
            (*tree)->left = new;
        }
   }
   else{
        if((*tree)->right != LEAF){
            return insertBST(&((*tree)->right), new);
        }
        else{   
            new->parent = (*tree);
            (*tree)->right = new;
        }
   }
}

void setRoot(Node **tree){
    Node *tmp = *tree;
    while(tmp->parent != NULL){
        tmp = tmp->parent;
    }
    *tree = tmp;
}

void delete(Node **tree, int data){
    if((*tree)->data == data && (*tree)->right == LEAF && (*tree)->left == LEAF){
        free(*tree);
        (*tree) = NULL;
    }
    else{
        deleteRecur(tree, data);
        setRoot(tree);
    }
}

void deleteRecur(Node **tree, int data){
/* Before anything else: Need to make sure y=that you're at the right node (iterate through tree)
 * 1. Need to account for the case where there is one child using an algorithm that accounts for 
 * all (use special cases from wikepedia) 
 * 2. Need to account for the case when there is both a left and a right child 
 */
   Node *tmp = *tree;
   if(tmp == LEAF){ //Node wasn't found in the tree (base case)
        return;
   }
   if(data > tmp->data){ //searching to see if we're at the right node in tree
        return deleteRecur(&(tmp->right), data);
   }
   if(data < tmp->data){ //checking to see if we're at the right node
        return deleteRecur(&(tmp->left), data);
   }

   if(tmp->right == LEAF || (tmp->left == LEAF)){
        deleteSingle(tree);
        return;
   }
   if((tmp)->right != LEAF && (tmp)->left != LEAF){
   //We find either the maximum element in its left subtree (in-order predecessor) or the minimum element in the right subtree)
        Node* val = inOrderPredecessor(tmp);
        (tmp)->data = val->data;
        deleteRecur(&(val), val->data);
        return;
   }
   
   return; 
}

Node* inOrderPredecessor(Node *node){ //Finds maximum element in the left subtree
    Node* sTree = (node)->left;
    while((sTree)->right != LEAF){
        sTree = sTree->right; 
    }
    return sTree;
}

void deleteSingle(Node **node){
    Node* n = *node;
    Node* child = n->right == LEAF ? n->left : n->right;
    if(n->parent != NULL)
        replace(n, child);
    else
        *node = child;
    child->parent = n->parent;    
    if(n->color == 'B'){
        if(child->color == 'R'){
            child->color = 'B';
        }
        else{
            deletec1(child);
        }    
    }

    free(n);

}

void deletec1(Node *node){ //Case 1: N is the root; we're done
    if(node->parent != NULL){
        deletec2(node);
    }
}

void deletec2(Node *node){ //Case 2: S (sibling) is red. Reverse the colors of P and S, rotate left at P
    Node *s = sibling(node);
    Node *p = parent(node);
    if(s->color == 'R'){
        p->color = 'R';
        s->color = 'B';
        if(node == p->left){
            rotateLeft(&(node->parent));
        }
        else{
            rotateRight(&(node->parent));
        }
    }
    deletec3(node);
}

void deletec3(Node *node){ //Case 3: P, S, and S's children are black. Repaint S red.
    Node *s = sibling(node);
    Node *p = parent(node);
    if((p->color == 'B') && (s->color == 'B') && (s->left->color == 'B') && 
        (s->right->color == 'B'))
        {
            s->color = 'R';
            deletec1(node->parent);
        }
    else{
        deletec4(node);
    }
}

void deletec4(Node *node){ //Case 4: S and S's children are black but P is red. Exchange colors of S and P
    Node *s = sibling(node);
    Node *p = parent(node);
    if((p->color == 'R') && (s->color == 'B') && (s->right->color == 'B') && 
        (s->left->color == 'B'))
        {
            s->color = 'R';
            p->color = 'B';
        }
    else{
        deletec5(node);
    }
}

void deletec5(Node *node){ //Case 5: S is black, S's left child is red, S's right child is black;rotate right at S and exchange colors of S and parent 
    Node* s = sibling(node);
    if((s->color == 'B')){
        if((node == node->parent->left) && (s->left->color == 'R') && 
         (s->right->color == 'B')){
            s->color = 'R';
            s->left->color = 'B';
            rotateRight(&s);
        }
        else if((node == node->parent->right) && (s->left->color == 'B') && 
         (s->right->color == 'R')){
            s->color = 'R';
            s->right->color = 'B';
            rotateLeft(&s);
         }
    }
    deletec6(node);
}

void deletec6(Node *node){ //S is black, S's right child is red, and N is the left child of its parent P; rotate left at P so S becomes Parent of P and S's right child. Exchange p an s colors
    Node *s = sibling(node);
    s->color = node->parent->color;
    node->parent->color = 'B';
    if(node == node->parent->left){
        s->right->color = 'B';
        rotateLeft(&(node->parent));
    }
    else{
        s->left->color = 'B';
        rotateRight(&(node->parent));
    }
}

void rebalance(Node **tree, Node *node){
    /* Accounting for different cases
     * Case 1: the Current Node N is at the root of the tree; repainted black 
     * Case 2: Parent is Black
     * Case 3: Both the parent P and the uncle U are red so they are repainted black
     * Case 4: Parent P is red but the Uncle is black ; the current Node is 
     * "outside" the subtree
    */
    
    //Case 1
    if(node->parent == NULL){
        node->color = 'B';
        return;
    }

    //Case 2
    if(parent(node)->color == 'B'){
        return;
    }

    //Case 3
    if(parent(node)->color == 'R' && uncle(node)->color == 'R'){
        Node *g = grandparent(node);
        g->color = 'R';
        parent(node)->color = 'B';
        uncle(node)->color = 'B';
        rebalance(tree, g);
        return;
    }
    
    //Case 4
    else{
        Node* p = parent(node);
        Node* g = grandparent(node);
        if(p == (g->left)){
            if(node == p->right){
                rotateLeft(&p);
                node->color = 'B';
                if(g == *tree)
                    *tree = node;
            }
            else{
                p->color = 'B';
                if(g == *tree){
                    *tree = p;
                }
            }
            g->color = 'R';
            rotateRight(&g);
        }
        else{
            if(node == p->left){
                rotateRight(&p);
                node->color = 'B';
                if(g == *tree){
                    *tree = node;
                }
            }
            else{
                p->color = 'B';
                if(g == *tree){
                    *tree = p;
                }
            }
            g->color = 'R';
            rotateLeft(&g);
        }
    }
}

bool isRedBlackTree(Node **tree){
    if(*tree == NULL) return true;
    return (*tree)->color == 'B' && leavesBlack_RedHasBlackChildren(tree); 
}

bool leavesBlack_RedHasBlackChildren(Node **tree){
    if(*tree == NULL) 
        return true;
    if(*tree == LEAF){
        if((*tree)->color == 'R')
            return false;
        return true;
    }
    if((*tree)->color == 'R' && ((*tree)->right->color == 'R' || (*tree)->left->color == 'R'))
        return false;
    return leavesBlack_RedHasBlackChildren(&((*tree)->left)) 
        && leavesBlack_RedHasBlackChildren(&((*tree)->right));
}

void rotateLeft(Node **node){
    Node* temp = *node;
    Node* temp2 = temp->right;
    temp2->parent = temp->parent;
    if(temp2->parent == NULL){
        *node = temp2;
    }
    else{
        if(temp2->parent->left == temp)
            temp2->parent->left = temp2;
        else if(temp2->parent->right == temp)
            temp2->parent->right = temp2;
    }
    temp->right = temp2->left;
    if(temp->right != LEAF)
        temp->right->parent = temp;
    temp2->left = temp;
    temp->parent = temp2;
}

void rotateRight(Node **node){
    Node* temp = *node;
    Node* temp2 = temp->left;
    temp2->parent = temp->parent;
    if(temp2->parent == NULL){
        *node = temp2;
    }
    else{
        if(temp2->parent->right == temp)
            temp2->parent->right = temp2;
        else if(temp2->parent->left == temp)
            temp2->parent->left = temp2;
    }
    temp->left = temp2->right;
    if(temp->left != LEAF)
        temp->left->parent = temp;
    temp2->right = temp;
    temp->parent = temp2;
}

Node* parent(Node *n){
    return n->parent;
}

Node* grandparent(Node* n){
    Node* parent1 = parent(n);
    if(parent1 == NULL){
        return NULL; 
    }
    return parent(parent1);
}

Node* sibling(Node *n){
    Node* parent1 = parent(n);
    if(!parent1){
        return NULL;
    }
    if(n == parent1->right){
        return parent1->left;
    }
    return parent1->right;
}

Node* uncle(Node *n){
    Node* parent1 = parent(n);
    if(parent1 ==NULL){
        return NULL;
    }
    return sibling(parent1);
}
