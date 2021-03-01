#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (NULL == q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    list_ele_t *cur, *next;
    if (q) {
        cur = q->head;
        while (cur) {
            next = cur->next;
            free(cur->value);
            free(cur);
            cur = next;
        }
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (NULL == q)
        return false;

    list_ele_t *newh;
    int len = strlen(s);
    newh = malloc(sizeof(list_ele_t));
    if (NULL == newh)
        goto fail_head;
    newh->next = NULL;

    newh->value = (char *) malloc(sizeof(char) * (len + 1));
    if (NULL == newh->value)
        goto fail_value;

    memcpy(newh->value, s, len);
    newh->value[len] = '\0';

    newh->next = q->head;
    q->head = newh;
    if (NULL == q->tail)
        q->tail = newh;
    q->size++;
    return true;

fail_value:
    free(newh);
fail_head:
    return false;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (NULL == q)
        return false;

    int len = strlen(s);
    list_ele_t *node = malloc(sizeof(list_ele_t));
    if (NULL == node)
        goto fail_head;
    node->next = NULL;

    node->value = malloc(sizeof(char) * (len + 1));
    if (NULL == node->value)
        goto fail_value;

    memcpy(node->value, s, len);
    node->value[len] = '\0';

    if (q->tail)
        q->tail->next = node;

    q->tail = node;
    if (NULL == q->head)
        q->head = node;
    q->size++;
    return true;

fail_value:
    free(node);
fail_head:
    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (NULL == q || NULL == q->head)
        return false;

    list_ele_t *node = q->head;

    if (sp && bufsize > 0) {
        int vlen = strlen(node->value);
        int cpylen = vlen < bufsize ? vlen : bufsize - 1;
        memcpy(sp, node->value, cpylen);
        sp[cpylen] = '\0';
    }
    if (node == q->tail)
        q->tail = NULL;
    q->head = q->head->next;
    free(node->value);
    free(node);

    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (NULL == q)
        return 0;
    else
        return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (NULL == q || q->size <= 1)
        return;
    list_ele_t *left, *right;
    list_ele_t *tmp, *prev;
    prev = NULL;
    left = q->head;
    right = left->next;
    tmp = right->next;
    while (right) {
        left->next = prev;
        right->next = left;
        prev = left;
        left = right;
        right = tmp;
        tmp = tmp ? tmp->next : NULL;
    }
    left->next = prev;

    tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
}

void list_add(list_ele_t **list, list_ele_t *node)
{
    node->next = *list;
    *list = node;
}

void list_concat(list_ele_t **left, list_ele_t *right)
{
    while (*left) {
        left = &((*left)->next);
    }
    *left = right;
}

void quick_sort(list_ele_t **list)
{
    if (NULL == list || NULL == *list)
        return;
    list_ele_t *pivot;
    list_ele_t *left = NULL, *right = NULL;
    list_ele_t *ptr;

    pivot = *list;
    ptr = pivot->next;
    pivot->next = NULL;

    while (ptr) {
        list_ele_t *node = ptr;
        ptr = ptr->next;
        if (strcmp(pivot->value, node->value) <= 0) {
            list_add(&right, node);
        } else {
            list_add(&left, node);
        }
    }

    if (!left || !right)
        printf("L %p R %p\n", left, right);
    quick_sort(&left);
    quick_sort(&right);

    *list = NULL;
    list_concat(list, left);
    list_concat(list, pivot);
    list_concat(list, right);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (NULL == q || q->size <= 1)
        return;

    list_ele_t *cur;
    quick_sort(&(q->head));
    cur = q->head;
    while (cur->next) {
        cur = cur->next;
    }
    q->tail = cur;
}
