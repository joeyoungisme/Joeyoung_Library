#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "heap_4.h"
#include "LinkingList.h"

// static List_Header timer_list = INIT_LIST_HEADER;

//                     Header.first                    Header.tail
//                     V                               V
//                      ___     ___     ___     ___     ___   
//             NULL <--|   |<--|   |<--|   |<--|   |<--|   |  
//                     |___|-->|___|-->|___|-->|___|-->|___|--> NULL
// Node Index :          0       1       2       3       4
// Insert Idx :      0       1       2       3       4       5

int List_Insert_Tail(List_Header head, void *ins)
{
    if(!head || !ins) return -1;

    List_Node *new = (List_Node *)extmalloc(sizeof(List_Node));

    // First Node Add
    if(!head->first) {
        head->first = head->tail = new;
        new->next = NULL;
        new->prev = NULL;
        new->ins = ins;
    }
    else {
        head->tail->next = new;
        new->prev = head->tail;
        new->next = NULL;
        new->ins = ins;
        head->tail = new;
    }

    head->node_cnt++;

    return 0;
}

int List_Insert_First(List_Header head, void *ins)
{
    if(!head || !ins) return -1;

    List_Node *new = (List_Node *)extmalloc(sizeof(List_Node));

    // First Node Add
    if(!head->first) {
        head->first = head->tail = new;
        new->next = NULL;
        new->prev = NULL;
        new->ins = ins;
    }
    else {
        head->first->prev = new;
        new->prev = NULL;
        new->next = head->first;
        new->ins = ins;
        head->first = new;
    }

    head->node_cnt++;

    return 0;
}

int List_Insert_Index(List_Header head, void *ins, unsigned int idx)
{
    if(!head || !ins) return -1;

    if(!idx) return List_Insert_First(head, ins);
    if(idx >= head->node_cnt) List_Insert_Tail(head, ins);

    List_Node *new = (List_Node *)extmalloc(sizeof(List_Node));

    List_Node *p_node = head->first;

    for(unsigned int cnt = 1; p_node; ++cnt) {
        if(cnt == idx) {
            p_node->next->prev = new;
            new->next = p_node->next;

            new->prev = p_node;
            p_node->next = new;

            head->node_cnt++;

            return 0;
        }
        else p_node = p_node->next;
    }

    return -1;
}

void *List_Get_Tail(List_Header head)
{
    if(!head) return NULL;

    return head->tail->ins;
}

void *List_Get_First(List_Header head)
{
    if(!head) return NULL;

    return head->first->ins;
}

void *List_Get_Index(List_Header head, unsigned int idx)
{
    if(!head || idx > head->node_cnt) return NULL;

    if(!idx) return List_Get_First(head);

    List_Node *p_node = head->first;

    for(unsigned int cnt = 0; p_node; ++cnt)
    {
        if(cnt == idx) return p_node->ins;
        else p_node = p_node->next;
    }

    return NULL;
}

int List_Remove_Tail(List_Header head)
{
    if(!head) return -1;

    if(!head->tail) return 0;

    List_Node *tail_node = head->tail;

    // Have Prev Node
    if(tail_node->prev)
        tail_node->prev->next = tail_node->next;

    head->tail = tail_node->prev;

    // Only One Node
    if(head->first == tail_node) head->first = tail_node->next;

    extfree(tail_node);
    head->node_cnt--;

    return 0;
}

int List_Remove_First(List_Header head)
{
    if(!head) return -1;

    if(!head->first) return 0;

    List_Node *first_node = head->first;

    // Have Next Node
    if(first_node->next)
        first_node->next->prev = first_node->prev;

    head->first = first_node->next;

    // Only One Node
    if(head->tail == first_node) head->tail = first_node->prev;

    extfree(first_node);
    head->node_cnt--;

    return 0;
}

int List_Remove_Index(List_Header head, unsigned int idx)
{
    if(!head || idx > head->node_cnt) return -1;

    if(!idx) return List_Remove_First(head);

    List_Node *p_node = head->first;

    for(unsigned int cnt = 0; p_node; ++cnt) {
        if(cnt == idx) {
            if(p_node == head->first)
                return List_Remove_First(head);
            else if(p_node == head->tail)
                return List_Remove_Tail(head);
            else {
                p_node->prev->next = p_node->next;
                p_node->next->prev = p_node->prev;

                extfree(p_node);
                head->node_cnt--;

                return 0;
            }
        }
        else p_node = p_node->next;
    }

    return -1;
}

int List_Header_Init(List_Header *head)
{
    if(!head) return -1;
    if(*head) return 0;

    *head = (List_Header)extmalloc(sizeof(struct __list_header));
    if(*head) memset(*head, 0, sizeof(struct __list_header));

    return 0;
}

int List_Header_Delete(List_Header *head)
{
    if(!head) return -1;
    if(!*head) return 0;

    while(!(*head)->first && !(*head)->tail)
        List_Remove_First(*head);

    extfree(*head);

    return 0;
}

int List_Header_Length(List_Header head)
{
    if(!head) return 0;

    return head->node_cnt;
}

