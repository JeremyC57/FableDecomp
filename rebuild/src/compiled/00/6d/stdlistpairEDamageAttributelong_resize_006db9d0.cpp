// std::list<pair<EDamageAttribute,long> >::resize(unsigned int new_size, const pair<EDamageAttribute,long>& x) -- STLport resize: walk up to new_size nodes, erase the surplus, or append the missing copies of x at the end
// Leaf: ?resize@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB9D0, length 90 bytes (+ int3 padding), thiscall `ret 8`. Single
// caller: CTCHeroExperience::OnInitialActivate 0x006D8B21 (landed; it names this callee
// `list<pair<EDamageAttribute,long>>::resize(n, const value&)` on AttackHistory, retail +0x50). The manifest label
// "LinkedList_Trim" is descriptive.
// Callees: free @ 0x00BFEA14 (node deallocate, no null test), list::_M_fill_insert(pos, n, x) @ 0x006DB340 (this
// chunk's row).
// Shape: STLport 4.x verbatim: `for (; i != end() && len < new_size; ++i, ++len); if (len == new_size) erase(i, end());
// else insert(end(), new_size - len, x);` with erase(first, last) -> `erase(first++)` inlined (unlink, free) and
// insert(pos, n, x) forwarding to the out-of-line _M_fill_insert. The end() iterator is passed BY VALUE and the
// iterator has a user copy constructor, so it is built in its argument slot (`push ecx ; mov eax,esp ; mov [eax],edi`).
#include <stddef.h>
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    _List_iterator operator++(int) { _List_iterator tmp = *this; _M_node = _M_node->_M_next; return tmp; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator {
public:
    allocator() { }
};

template <class T, class A = allocator<T> > class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    typedef unsigned int size_type;
    _Node* _M_node;                                        // the sentinel (alloc proxy)
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    void _M_fill_insert(iterator pos, size_type n, const T& x);   // 0x006DB340
    void insert(iterator pos, size_type n, const T& x) { _M_fill_insert(pos, n, x); }
    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        _Node* n = (_Node*)position._M_node;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        free(n);
        return iterator(next_node);
    }
    iterator erase(iterator first, iterator last)
    {
        while (first != last)
            erase(first++);
        return last;
    }
    void resize(size_type new_size, const T& x);
};

template <class T, class A> void list<T, A>::resize(size_type new_size, const T& x)
{
    iterator i = begin();
    size_type len = 0;
    for (; i != end() && len < new_size; ++i, ++len);
    if (len == new_size)
        erase(i, end());
    else
        insert(end(), new_size - len, x);
}

template void list<pair<EDamageAttribute, long> >::resize(unsigned int, const pair<EDamageAttribute, long>&);
}