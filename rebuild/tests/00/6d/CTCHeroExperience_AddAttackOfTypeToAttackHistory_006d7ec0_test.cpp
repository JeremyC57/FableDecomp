// Behaviour test for 0x006D7EC0 (source body copied from src/006d7ec0.cpp; out-of-line callees stubbed below).
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
#include <stddef.h>

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
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
    void push_back(const _Tp& __x) { insert(end(), __x); }
    void pop_front() { erase(begin()); }
};
}

class CWorld { public:
    static long __fastcall GetFrame();                     // 0x0049D870
};

class CTCHeroExperience { public:
    void*         __vftable;                               // +0x00
    unsigned char _pad_0x04[0x4C];                         // +0x04 CTCBase fields, experience counters, vectors, Pickups
    std::list<std::pair<EDamageAttribute, long> > AttackHistory;   // +0x50 (PDB +0x64)

    CWorld* GetWorld();                                    // 0x00686A70 (CTCBase)
public:
    void AddAttackOfTypeToAttackHistory(EDamageAttribute type);   // private in Ego_r.pdb (AAE)
};

void CTCHeroExperience::AddAttackOfTypeToAttackHistory(EDamageAttribute type)
{
    AttackHistory.pop_front();
    AttackHistory.push_back(std::pair<EDamageAttribute, long>(type, GetWorld()->GetFrame()));
}


// ---- test harness ----
static long g_frame = 0;
static CWorld g_world;
CWorld* CTCHeroExperience::GetWorld() { return &g_world; }
long __fastcall CWorld::GetFrame() { return g_frame; }
typedef std::pair<EDamageAttribute, long> TRec;
typedef std::_List_node<TRec> TNode;
int main()
{
    static double raw[sizeof(CTCHeroExperience) / sizeof(double) + 1];
    memset(raw, 0, sizeof raw);
    CTCHeroExperience* hero = (CTCHeroExperience*)raw;
    static double sraw[sizeof(TNode) / sizeof(double) + 1];
    TNode* sentinel = (TNode*)sraw;
    sentinel->_M_next = sentinel; sentinel->_M_prev = sentinel;
    hero->AttackHistory._M_node._M_data = sentinel;
    int ok = 1;
    for (int k = 0; k < 3; ++k) {                       // three records: frames 10, 11, 12
        TNode* n = (TNode*)malloc(sizeof(TNode));
        n->_M_data.first = (EDamageAttribute)k; n->_M_data.second = 10 + k;
        n->_M_next = sentinel; n->_M_prev = sentinel->_M_prev;
        sentinel->_M_prev->_M_next = n; sentinel->_M_prev = n;
    }
    g_frame = 77;
    hero->AddAttackOfTypeToAttackHistory((EDamageAttribute)5);
    TNode* a = (TNode*)sentinel->_M_next;
    TNode* b = (TNode*)a->_M_next;
    TNode* c = (TNode*)b->_M_next;
    if (a->_M_data.second != 11 || b->_M_data.second != 12) ok = 0;   // oldest (frame 10) dropped
    if (c->_M_data.first != 5 || c->_M_data.second != 77 || c->_M_next != sentinel || sentinel->_M_prev != c) ok = 0;
    if (a->_M_prev != sentinel || b->_M_prev != a || c->_M_prev != b) ok = 0;
    printf("006d7ec0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}