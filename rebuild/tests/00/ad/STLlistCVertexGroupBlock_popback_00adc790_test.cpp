// Behaviour test for 0x00ADC790 _STL::list<CVertexGroupBlock>::pop_back: unlink the last node, destroy its
// vertex-group block and free the node
#include <stdio.h>
#include <stdlib.h>
static int gFrees = 0;
static void* gFreed = 0;
static int gDtors = 0;
static unsigned long gLastDestroyed = 0;
static void test_free(void* p) { ++gFrees; gFreed = p; free(p); }
class CVertexGroupBlock {
public:
    unsigned long Fields[6];
    ~CVertexGroupBlock();
};
CVertexGroupBlock::~CVertexGroupBlock() { ++gDtors; gLastDestroyed = Fields[0]; }   // stand-in
namespace _STL {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { test_free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
};
}
typedef _STL::list<CVertexGroupBlock> CBlockList;
typedef CBlockList::_Node CNode;
int main()
{
    CNode* h = (CNode*)malloc(sizeof(CNode));
    CNode* n0 = (CNode*)malloc(sizeof(CNode));
    CNode* n1 = (CNode*)malloc(sizeof(CNode));
    n0->_M_data.Fields[0] = 10;
    n1->_M_data.Fields[0] = 11;
    h->_M_next = n0; n0->_M_next = n1; n1->_M_next = h;
    h->_M_prev = n1; n1->_M_prev = n0; n0->_M_prev = h;
    CBlockList l;
    l._M_node._M_data = h;
    l.pop_back();
    bool ok = true;
    ok = ok && gDtors == 1 && gLastDestroyed == 11 && gFrees == 1 && gFreed == (void*)n1;
    ok = ok && h->_M_prev == n0 && n0->_M_next == h && h->_M_next == n0 && n0->_M_prev == h;
    l.pop_back();
    ok = ok && gDtors == 2 && gLastDestroyed == 10 && gFrees == 2 && gFreed == (void*)n0;
    ok = ok && h->_M_next == h && h->_M_prev == h;
    free(h);
    if (ok) { printf("00adc790_TEST PASS\n"); return 0; }
    printf("00adc790_TEST FAIL\n");
    return 1;
}