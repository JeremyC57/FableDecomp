// std::list<pair<EDamageAttribute,long> >::_M_fill_insert(iterator pos, unsigned int n, const pair<EDamageAttribute,long>& x) -- STLport: insert n copies of x before pos, one malloc'd node each
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DB340, length 70 bytes (+ int3 padding), thiscall `ret 0Ch` (`this`
// unused: the node allocator is empty). Callers: list::resize(n, x) 0x006DB9D0 (this chunk's row, its grow arm) and
// 0x006DB6E3. The list is CTCHeroExperience::AttackHistory, list<pair<EDamageAttribute, long>> (the landed
// OnInitialActivate 0x006D8B00 resizes it to ten {-1, 0} records). The manifest label "LinkedList_InsertElements" is
// descriptive.
// Callees: malloc @ 0x00BFEA0E (STLport node allocator, 0x10-byte node: next, prev, 8-byte pair).
// Shape: STLport 4.x verbatim: `for (; n > 0; --n) insert(pos, x);` with insert/_M_create_node inlined -- the
// placement copy of the pair behind its null test, then tmp->next = pos, tmp->prev = pos->prev, prev->next = tmp,
// pos->prev = tmp (the `--n` is scheduled before the links). The iterator is the STLport `_List_iterator` over an
// `_List_iterator_base` holding `_M_node`, passed by value.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
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
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        _Construct(&p->_M_data, x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void _M_fill_insert(iterator pos, size_type n, const T& x);
};

template <class T, class A> void list<T, A>::_M_fill_insert(iterator pos, size_type n, const T& x)
{
    for (; n > 0; --n)
        insert(pos, x);
}

template void list<pair<EDamageAttribute, long> >::_M_fill_insert(iterator, unsigned int, const pair<EDamageAttribute, long>&);
}