// Behaviour test for 0x00617230 std::list<pair<const CRegion*, const CRegion*> >::push_back(x): insert(end(), x) --
// malloc a node, copy the region pair, link it before the sentinel (CTCInventoryMap::RegionsCorrespondances).
#include <cstdio>
#include <cstdlib>
#include <new>

class CRegion { public: int m_id; };

namespace stl {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

typedef stl::pair<const CRegion*, const CRegion*> RP;
typedef stl::list<RP> L;

int main()
{
    int ok = 1;
    CRegion r[4] = { { 0 }, { 1 }, { 2 }, { 3 } };
    stl::_List_node_base header;
    header._M_next = header._M_prev = &header;
    L l;
    l._M_node = &header;
    l.push_back(RP(&r[0], &r[1]));
    l.push_back(RP(&r[2], &r[3]));
    l.push_back(RP(&r[3], 0));
    const CRegion* wantA[3] = { &r[0], &r[2], &r[3] };
    const CRegion* wantB[3] = { &r[1], &r[3], 0 };
    int n = 0;
    for (stl::_List_node_base* p = header._M_next; p != &header; p = p->_M_next, ++n) {
        const RP& v = ((L::_Node*)p)->_M_data;
        if (n >= 3 || v.first != wantA[n] || v.second != wantB[n] || p->_M_next->_M_prev != p) ok = 0;
    }
    if (n != 3 || header._M_prev->_M_next != &header) ok = 0;
    stl::_List_node_base* p = header._M_next;
    while (p != &header) { stl::_List_node_base* nx = p->_M_next; std::free(p); p = nx; }
    if (!ok) { std::printf("00617230_TEST FAIL\n"); return 1; }
    std::printf("00617230_TEST PASS\n");
    return 0;
}