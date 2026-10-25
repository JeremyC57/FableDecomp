// Behaviour test for 0x00729960 (source body copied from src/00729960.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif

class CTCCreatureExpression {
public:
    class CExpressionState {                            // 0x14 bytes (PDB)
    public:
        long          AnimIndex;                        // +0x00
        float         MaxWeight;                        // +0x04
        float         Weight;                           // +0x08
        float         OldWeight;                        // +0x0c
        bool          Primary;                          // +0x10
        unsigned char Stage;                            // +0x11
        CExpressionState(long animIndex, float maxWeight)
            : AnimIndex(animIndex), MaxWeight(maxWeight), Weight(0.0f), OldWeight(0.0f), Primary(true), Stage(0) {}
    };
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
public:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
};
}

template void std::list<CTCCreatureExpression::CExpressionState>::push_front(const CTCCreatureExpression::CExpressionState&);


// ---- test harness ----
typedef CTCCreatureExpression::CExpressionState TState;
typedef std::list<TState> TList;
typedef std::_List_node<TState> TNode;
int main()
{
    static double sentinelRaw[sizeof(TNode) / sizeof(double) + 1];
    TNode* sentinel = (TNode*)sentinelRaw;
    sentinel->_M_next = sentinel; sentinel->_M_prev = sentinel;
    TList list;
    list._M_node._M_data = sentinel;
    int ok = 1;
    list.push_front(TState(7, 0.5f));               // empty list: sentinel <-> A
    TNode* a = (TNode*)sentinel->_M_next;
    if (a == sentinel || a->_M_next != sentinel || a->_M_prev != sentinel || sentinel->_M_prev != a) ok = 0;
    if (a->_M_data.AnimIndex != 7 || a->_M_data.MaxWeight != 0.5f || a->_M_data.Weight != 0.0f || !a->_M_data.Primary || a->_M_data.Stage != 0) ok = 0;
    list.push_front(TState(9, 1.0f));               // in front of A
    TNode* b = (TNode*)sentinel->_M_next;
    if (b == a || b->_M_next != a || b->_M_prev != sentinel || a->_M_prev != b || sentinel->_M_prev != a) ok = 0;
    if (b->_M_data.AnimIndex != 9 || b->_M_data.MaxWeight != 1.0f) ok = 0;
    printf("00729960_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}