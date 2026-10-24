// Behaviour test for 0x004BA420 std::list<pair<CCharString, long> >::push_back(value): allocate a node, copy the pair
// into it (CCharString copy ctor + the long) and link it before the sentinel.
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#endif

#include <new>

struct CCharStringData;
class CCharString {
public:
    CCharString(const CCharString& other);      // 0x0099EC30
public:
    CCharStringData* m_pData;
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return static_cast<T*>(malloc(n * sizeof(T))); }
};

template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    __forceinline _List_iterator(_List_node_base* x) : _M_node(x) {}
    __forceinline _List_iterator(const _List_iterator& x) : _M_node(x._M_node) {}   // user copy ctor (the 0x004C0050 fix)
};

template <class T> class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    struct : allocator<_Node> { _List_node_base* _M_data; } _M_node;   // _STLP_alloc_proxy: the sentinel
    __forceinline iterator end() { return iterator(_M_node._M_data); }
    __forceinline _Node* _M_create_node(const T& x)
    {
        _Node* p = _M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    __forceinline iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x);
};

template <class T> void list<T>::push_back(const T& x)
{
    insert(end(), x);
}


}

static int g_copies = 0;
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; }

int main()
{
    typedef std::pair<CCharString, long> P;
    std::list<P> l;
    std::_List_node_base sentinel; sentinel._M_next = sentinel._M_prev = &sentinel;
    l._M_node._M_data = &sentinel;
    int tags[3];
    P* v = (P*)malloc(sizeof(P));
    int ok = 1;
    for (int k = 0; k < 3; ++k) {
        v->first.m_pData = (CCharStringData*)&tags[k]; v->second = 100 + k;
        l.push_back(*v);
    }
    if (g_copies != 3) ok = 0;
    std::_List_node_base* n = sentinel._M_next;
    for (int k = 0; k < 3 && ok; ++k) {
        std::_List_node<P>* node = (std::_List_node<P>*)n;
        if (node->_M_data.first.m_pData != (CCharStringData*)&tags[k] || node->_M_data.second != 100 + k) ok = 0;
        if (n->_M_next->_M_prev != n) ok = 0;
        n = n->_M_next;
    }
    if (n != &sentinel || sentinel._M_prev->_M_next != &sentinel) ok = 0;
    printf(ok ? "004ba420_TEST PASS\n" : "004ba420_TEST FAIL\n");
    return ok ? 0 : 1;
}