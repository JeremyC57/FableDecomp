// Behaviour test for 0x006864C0 StdList_InitEmpty_006864c0: the head ends up on a fresh sentinel that links to itself
// both ways (an empty list).
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#endif

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { int _M_data; };
struct SListHead { _List_node* _M_node; };

void __fastcall StdList_InitEmpty_006864c0(SListHead* list)
{
    list->_M_node = 0;
    _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
    n->_M_next = n;
    n->_M_prev = n;
    list->_M_node = n;
}

int main()
{
    SListHead h;
    h._M_node = (_List_node*)&h;                            // garbage head
    StdList_InitEmpty_006864c0(&h);
    if (h._M_node == 0 || h._M_node == (_List_node*)&h || h._M_node->_M_next != h._M_node ||
        h._M_node->_M_prev != h._M_node) {
        std::printf("006864c0_TEST FAIL\n"); return 1;
    }
    std::free(h._M_node);
    std::printf("006864c0_TEST PASS\n");
    return 0;
}