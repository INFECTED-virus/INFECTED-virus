#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void* mallocs[1000] = {NULL};
int mindex = 0;

typedef struct DLListNode {
    int value;
    struct DLListNode* next;
    struct DLListNode* prev;
} node;

typedef struct DLList {
    node* head;
    node *tail;
} list;

node *new_node(int value, node *prev, node* next) {
    node *a = malloc(sizeof(node));
    a->prev = prev;
    a->next = next;
    a->value = value;
    mallocs[mindex++] = a;
    return a;
}

list* new_list(node *head, node* tail) {
    list* a = malloc(sizeof(list));
    a->head = head;
    a->tail = tail;
    mallocs[mindex++] = a;
    return a;
}

void dealloc() {
    for(int i = 0; mallocs[i]; i++) free(mallocs[i]);
}

size_t size(list* l) {
    if (!l->head) return 0;
    int i = 0;
    for(node * n = l->head; n; n = n->next) i++;
    return i;
}

void insert(list* l, int value, size_t pos) {
    if (pos == 0) {
        node *a = new_node(value, NULL, l->head);
        l->head->prev = a;
        l->head = a;
    }
    else {
        node* cursor = l->head;
        for(int i = 0; i < pos; cursor = cursor->next) ++i;
        node *a = new_node(value, cursor, cursor->next);
        if (cursor->next) cursor->next->prev = a;
        cursor->next = a;
    }
}

void print(list *l) {
    for(node *cursor = l->head; cursor; cursor = cursor->next) printf("%d ", cursor->value);
    puts(" ");
}

void recalculateTail(list* l) {
    node* n = l->head;
    for(; n->next; n = n->next);
    l->tail = n;
}

void delete(list *l, size_t pos) {
    if (size(l) == 1) l->head = NULL;
    else if (pos == 0) {
        l->head = l->head->next;
        if (l->head) l->head->prev = NULL;
    }
    else {
        node* cursor = l->head;
        for(int i = 0; i < pos; cursor = cursor->next) ++i;
        if (cursor->prev) cursor->prev->next = cursor->next;
        if (cursor->next) cursor->next->prev = cursor->prev;
    }
}

int main() {
    list* l = new_list(NULL, NULL);
    char s[100], is[100], dump[100];
    int a, b;
    while (true) {
        printf(">>> ");
        fgets(s, 99, stdin);
        sscanf(s, "%s", is);
        if (!strcmp(is, "append")) {
            sscanf(s, "%s %d", dump, &a);
            if (!l->head) l->head = new_node(a, NULL, NULL);
            else insert(l, a, size(l) - 1);
        }
        else if (!strcmp(is, "delete")) {
            sscanf(s, "%s %d", dump, &a);
            if (a < 0) a = size(l) + a;
            if (a < 0 || a > size(l) - 1) fputs("Index out of bounds.\n", stderr);
            else delete(l, a);
        }
        else if (!strcmp(is, "insert")) {
            sscanf(s, "%s %d %d", dump, &b, &a);
            if (a < 0) a = size(l) + a;
            if (a < 0 || a > size(l) - 1) fputs("Index out of bounds.\n", stderr);
            else insert(l, b, a);
        }
        else if (!strcmp(is, "pop")) {
            if (l->head != NULL) delete(l, size(l) - 1);
            else fputs("Empty list.\n", stderr);
        }
        else if (!strcmp(is, "print") || !strcmp(is, "delete")) {
            if (l->head) print(l);
            else fputs("Empty list.\n", stderr);
        }
        else if (!strcmp(is, "exit")) break;
        else if (!strcmp(is, "size")) printf("%ld\n", size(l));
        if (l->head) recalculateTail(l);
    }

    dealloc();
}
