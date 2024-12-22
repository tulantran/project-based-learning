#include "pq.h"

#include "node.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree; 
    ListElement *next; //pointer to next node
};
struct PriorityQueue { //pointer to head of list
    ListElement *list;
};

PriorityQueue *pq_create(void) { // Allocate a PriorityQueue object and return a pointer to it.
  
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq == NULL) {
        return NULL;
    } else {
        pq->list = NULL;
        return pq;
    }
}

void pq_free(PriorityQueue **q) {
    if (*q != NULL) {
        free(*q);
        *q = NULL;
    }
}


bool pq_is_empty(PriorityQueue *q) {//indicate an empty queue by storing NULL in the queue’s list field. Return true if that’s the case.
    if (q->list == NULL) {
        return true;
    } else {
        return false;
    }
}

bool pq_size_is_1(PriorityQueue *q) {
    if (!pq_is_empty(q) && (q->list->next == NULL)) {
        return true;
    } else {
        return false;
    }
}

bool pq_less_than(ListElement *e1, ListElement *e2) {// returning true if the weight of the first element is less than the weight of the second element.
    if (e1->tree->weight < e2->tree->weight) {
        return true;
    } else if (e1->tree->weight == e2->tree->weight && e1->tree->symbol < e2->tree->symbol) {//If the weights of the elements are equal, then compare their tree->symbol values, and return true if the symbol of the first element is less than the symbol of the second element.
        return true;
    } else {
        return false;
    }
}//note: This function is not used outside of pq.c, and so it is not declared in pq.h.


void enqueue(PriorityQueue *q, Node *tree) {
    //create ListElement
    ListElement *insertee = malloc(sizeof(ListElement));
    if (insertee == NULL) {
        // Error allocating memory
        return;
    }
    //initialize 
    insertee->tree = tree;
    insertee->next = NULL;

    //create pointers to traverse list
    ListElement *prev = NULL;
    ListElement *current = q->list;

    while (current != NULL && !pq_less_than(insertee, current)) { //use value comparison function to correctly place element in list
        prev = current;
        current = current->next;
    }

    if (prev == NULL) { //Case1: insert at beginning of list
        insertee->next = q->list;
        q->list = insertee;
    } else { //Case2: insert somewhere after
        insertee->next = current;
        prev->next = insertee;
    }
}

Node *dequeue(PriorityQueue *q) {//Remove the queue element with the lowest weight and return it
    assert(q != NULL);
    assert(!pq_is_empty(q)); // If the queue is empty, report a fatal error.
    Node *rtn = q->list->tree;
    ListElement *tem = q->list;
    q->list = q->list->next;
    free(tem);
    return rtn;
}


void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree);
        e = e->next;
    }
    printf("=============================================\n");
}
