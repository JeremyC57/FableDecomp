// Test for 0x004AB490 std::list<CThing*>::~list: every node and the header are freed exactly once.
#include <stdio.h>
#include <stdlib.h>
static int g_freed = 0;
static void CountedFree(void* p) { ++g_freed; free(p); }
#define FREE CountedFree
namespace std {
template <class _Tp> class allocator { };

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list: one header-node pointer
public:
    _List_node_base* _M_node;                           // +0x00

    void clear()
    {
        _List_node_base* __cur = _M_node->_M_next;
        while (__cur != _M_node) {
            _List_node_base* __tmp = __cur;
            __cur = __cur->_M_next;
            FREE(__tmp);                                // _Destroy(CThing**) is empty; deallocate(tmp, 1)
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            FREE(_M_node);                              // the base destructor: deallocate(header, 1)
    }
};
}

class CThing;

typedef std::_List_node<CThing*> TNode;
int main()
{
    bool ok = true;
    for (int n = 0; n < 4; ++n) {
        g_freed = 0;
        {
            std::list<CThing*>* l = (std::list<CThing*>*)malloc(sizeof(std::list<CThing*>));
            TNode* hdr = (TNode*)malloc(sizeof(TNode));
            hdr->_M_next = hdr; hdr->_M_prev = hdr; l->_M_node = hdr;
            for (int i = 0; i < n; ++i) {
                TNode* x = (TNode*)malloc(sizeof(TNode)); x->_M_data = (CThing*)(size_t)(i + 1);
                x->_M_next = hdr; x->_M_prev = hdr->_M_prev; hdr->_M_prev->_M_next = x; hdr->_M_prev = x;
            }
            l->~list();
            free(l);
        }
        ok = ok && g_freed == n + 1;
    }
    if (!ok) { printf("004ab490_TEST FAIL\n"); return 1; }
    printf("004ab490_TEST PASS\n");
    return 0;
}
