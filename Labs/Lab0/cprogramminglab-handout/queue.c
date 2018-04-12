/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    /* Remember to handle the case if malloc returned NULL */
    queue_t* temp = malloc(sizeof(queue_t));
    if (temp) {//malloc success 
        temp->head = NULL;
        temp->tail = NULL;
        temp->size = 0;
        return temp;
    } else {//malloc fails
        return NULL;
    }
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Remember to free the queue structue and list elements */
    while(q_remove_head(q, NULL));//simply remove head iteratively unitl a false is returned
    if (q) {
        free(q);
    }
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v)
{
    /* What should you do if the q is NULL? */
    /* What if malloc returned NULL? */
    if (q == NULL) {//if q is NULL
        return false;
    } else {
        list_ele_t* newItem = malloc(sizeof(list_ele_t));//try malloc
        if (newItem == NULL) {//malloc fails
            return false;
        } else {//malloc success
            newItem->value = v;
            newItem->next = q->head;
            q->head = newItem;
            if (q->size++ == 0) {
                q->tail = newItem;
            }
            return true;
        }
    }
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v)
{
    /* Remember: It should operate in O(1) time */
    if (q == NULL) {
        return false;
    } else {
        list_ele_t* newItem = malloc(sizeof(list_ele_t));
        if (newItem == NULL) {
            return false;
        } else {
            newItem->next = NULL;
            newItem->value = v;
            if (q->size++ == 0) {
                q->head = newItem;
                q->tail = newItem;
            } else {
                q->tail->next = newItem;
                q->tail = newItem;
            }
            return true;
        }
    }
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp)
{
    if (q == NULL || q->size == 0) {//if the queue is NULL or Empty
        return false;
    } else {//Valid Non-empty queue
        list_ele_t* target = q->head;
        list_ele_t* newHead = target->next;
        if (vp) {
            *vp = target->value;
        }
        free(target);
        q->head = newHead;
        q->size--; 
        if (q->size == 0) {
            q->tail = NULL;
        }
        return true;
    }
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* Remember: It should operate in O(1) time */
    if (q == NULL || q->size == 0) {
        return 0;
    } else {
        return q->size;
    }
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q)
{
   if (q) {
       list_ele_t* temp = q->head;
       q->head = q->tail;
       q->tail = temp;
       
       list_ele_t* curr = q->tail;
       list_ele_t* next = NULL;
       list_ele_t* prev = NULL;
       while (curr) {//reverse linked-list in-place
           next = curr->next;
           curr->next = prev;
           prev = curr;
           curr = next;
       }
   }
}

