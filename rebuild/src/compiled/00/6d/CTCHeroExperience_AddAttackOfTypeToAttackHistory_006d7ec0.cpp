// CTCHeroExperience::AddAttackOfTypeToAttackHistory(EDamageAttribute type) -- drop the oldest attack record and append {type, current world frame} to the attack history
// Leaf: ?AddAttackOfTypeToAttackHistory@
// Grade: RELOCATION_MATCH (agent902, 1 grade this pass) -- the parked `std` list (raw sentinel pointer, one-parameter
//        iterator, pair without its copy ctor) coloured `type` into edi; the real STLport 4.6 list in std (alloc-proxy
//        sentinel, _List_iterator<T, _Nonconst_traits<T> > with its declared copy ctor, erase with _Destroy + deallocate,
//        pair with its user ctors) gives retail's frame-only edi and the `type` read from its argument slot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D7EC0, length 89 bytes (+ int3 padding), thiscall `ret 4`.
// Ego_r.pdb `?AddAttackOfTypeToAttackHistory@CTCHeroExperience@@AAEXW4EDamageAttribute@@@Z`; single caller 0x006D879A
// (0x006D8790, the CTCHeroExperience strike handler in this chunk). The manifest label "CViewBase_LogMemoryEntry" is
// propagated from GetWorld's wrong "CViewBase::GetDisplayEngine" label.
// Callees: free @ 0x00BFEA14 (node deallocate), CTCBase::GetWorld() @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine"
// is propagated; it reads a global, not `this`), CWorld::GetFrame() @ 0x0049D870 (static; manifest
// "GFGetPreMainMemoryUsed" is propagated), malloc @ 0x00BFEA0E (new node).
// Layout: AttackHistory list<pair<EDamageAttribute, long>> at retail +0x50 (PDB +0x64; see the landed
// OnInitialActivate 0x006D8B00, which sizes it to ten records): the STLport list is its 4-byte sentinel pointer.
// Shape: `AttackHistory.pop_front(); AttackHistory.push_back(pair(type, GetWorld()->GetFrame()));` with STLport's
// pop_front -> erase(begin()) (unlink, free without a null test) and push_back -> insert(end(), x) (malloc, placement
// copy behind its null test, link before the sentinel) inline. GetFrame is static, so `GetWorld()->` only evaluates
// the call for its object (result discarded), as retail does.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

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
protected:
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
private:
    void AddAttackOfTypeToAttackHistory(EDamageAttribute type);   // private in Ego_r.pdb (AAE)
};

void CTCHeroExperience::AddAttackOfTypeToAttackHistory(EDamageAttribute type)
{
    AttackHistory.pop_front();
    AttackHistory.push_back(std::pair<EDamageAttribute, long>(type, GetWorld()->GetFrame()));
}