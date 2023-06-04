#include "LinkedList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BOARD.h"


ListItem *LinkedListNew(char *data) {
    ListItem *list = (ListItem *) malloc(sizeof(ListItem));
    list->data = data;
    list->nextItem = NULL;
    list->previousItem = NULL;
    return list;
}

ListItem *LinkedListCreateAfter(ListItem *item, char *data) {
    ListItem *cur = LinkedListNew(data);
    if (item != NULL) {
        ListItem *next = item->nextItem;
        if (next != NULL) {
            next->previousItem = cur;
        }
        item->nextItem = cur;
        cur->previousItem = item;
        cur->nextItem = next;
    }
    return cur;
}

char *LinkedListRemove(ListItem *item) {
    if (item == NULL) return NULL;
    ListItem *next = item->nextItem;
    ListItem *prev = item->previousItem;
    if (next != NULL)
        next->previousItem = prev;
    if (prev != NULL)
        prev->nextItem = next;
    char *s = item->data;
    free(item);
    return s;
    
}

int LinkedListSize(ListItem *list) {
    ListItem *first = LinkedListGetFirst(list);
    int count = 0;
    while (first->nextItem != NULL) {
        first = first->nextItem;
        count++;
    }
    return count;
}

ListItem *LinkedListGetFirst(ListItem *list) {
    if (list == NULL) return NULL;
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}

ListItem *LinkedListGetLast(ListItem *list) {
    if (list == NULL) return NULL;
    while (list->nextItem != NULL) {
        list = list->nextItem;
    }
    return list;
}

int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem) {
    if (firstItem == NULL || secondItem == NULL) return STANDARD_ERROR;
    char *temp = secondItem->data;
    secondItem->data = firstItem->data;
    firstItem->data = temp;
    return SUCCESS;
}

int LinkedListPrint(ListItem *list) {
    if (list == NULL) return STANDARD_ERROR;
    list = LinkedListGetFirst(list);
    while (list != NULL) {
        printf("%s ", list->data);
        list = list->nextItem;
    }
    printf("\n");
    return SUCCESS;
    
}