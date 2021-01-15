#ifndef __LIB_LINKINGLIST_H
#define __LIB_LINKINGLIST_H

#define INIT_LIST_HEADER          NULL

typedef struct __list_node {
    void *ins;
    struct __list_node *next;
    struct __list_node *prev;
} List_Node;

struct __list_header {
    List_Node *first;
    List_Node *tail;
    unsigned int node_cnt;
};

typedef struct __list_header * List_Header;

int List_Header_Init(List_Header *);
int List_Header_Delete(List_Header *);
int List_Header_Length(List_Header);

int List_Insert_Tail(List_Header head, void *ins);
int List_Insert_Index(List_Header head, void *ins, unsigned int idx);
int List_Insert_First(List_Header head, void *ins);

void *List_Get_Tail(List_Header head);
void *List_Get_Index(List_Header head, unsigned int idx);
void *List_Get_First(List_Header head);

int List_Remove_Tail(List_Header head);
int List_Remove_Index(List_Header head, unsigned int idx);
int List_Remove_First(List_Header head);

#endif

