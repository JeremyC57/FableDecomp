// behaviour test for 005bac52: list::_M_create_node mallocs a node and copy-constructs the def pointer
// into its data slot, bumping the def's reference count (a null def pointer is copied without one).
#include <cstdio>
#include <cstdlib>
#include <new>

struct CCreatureGenerationFamilyDef {
    long m_tag;
    long m_refCount;
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

typedef CDefPointer<CCreatureGenerationFamilyDef> CFamilyDefPointer;

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

struct _List_node : _List_node_base {
    CFamilyDefPointer _M_data;
};

struct list {
    _List_node_base* _M_node;
    _List_node* _M_create_node(const CFamilyDefPointer& x)
    {
        _List_node* p = (_List_node*)std::malloc(sizeof(_List_node));
        new (&p->_M_data) CFamilyDefPointer(x);
        return p;
    }
};

int main()
{
    CCreatureGenerationFamilyDef def;
    CFamilyDefPointer ptr;
    CFamilyDefPointer none;
    list l;
    _List_node* node = 0;
    _List_node* empty = 0;
    int ok = 1;
    def.m_tag = 7;
    def.m_refCount = 1;
    ptr.Object = &def;
    l._M_node = 0;
    node = l._M_create_node(ptr);
    if (node == 0 || node->_M_data.Object != &def || def.m_refCount != 2)
        ok = 0;
    empty = l._M_create_node(none);
    if (empty == 0 || empty->_M_data.Object != 0 || def.m_refCount != 2)
        ok = 0;
    std::free(node);
    std::free(empty);
    if (!ok) {
        std::printf("005bac52_TEST FAIL\n");
        return 1;
    }
    std::printf("005bac52_TEST PASS\n");
    return 0;
}