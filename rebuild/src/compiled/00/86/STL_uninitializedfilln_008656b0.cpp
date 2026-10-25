// std::__uninitialized_fill_n(NCreatureHitNotification::CFlash* first, unsigned int n, const CFlash& x, const __false_type&) -- STLport placement fill of n raw hit-flash slots with copies of x (each: a fresh self-linked list sentinel, the range insert of x's trail entries, then the creation frame), returning the end of the filled range
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008656B0, length 89 bytes (+ int3 padding), `ret 8`.
// Callers: vector<NCreatureHitNotification::CFlash>::_M_insert_overflow 0x008657E0 (0x0086585C, the fill_len != 1 arm).
// Callees: malloc @ 0x00BFEA0E (the 0x24-byte sentinel node), list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::
//   _M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&) @ 0x0076FD40 (landed, `ret 10h`).
// Label: catalog "std::vector_InitializePairAnimationNodes" is descriptive.
// Element: NCreatureHitNotification::CFlash {list<CWeaponTrailEntry> FlashGeometry; long CreationFrame;} (Ego_r.pdb; retail
//   8 bytes), whose implicit copy constructor the landed 0x00864F80 is; inline here.
// Shape: STLport 4.6 `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return __cur;` (unsigned n: `test edx,edx
//   ; jbe`) in a /Gr algorithm TU (`__fastcall`: first in ecx, n in edx, &x and the empty tag on the stack) with the
//   inline list copy constructor (`_List_base(a)`: proxy zeroed, sentinel allocated and self-linked; then `insert(begin(),
//   x.begin(), x.end())` -> `_M_insert_dispatch` with the by-value iterator built in its argument slot and the
//   __false_type temporary homed in the dead `x` slot), then the word copy. Same list model as the landed 0x007702B0.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C3DVector { public: float X, Y, Z; };
class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                               // Ego_r.pdb, 28 bytes
    public:
        C3DVector Top;                                      // +0x00
        C3DVector Bottom;                                   // +0x0c
        float     CreationTime;                             // +0x18
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

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
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const _Alloc&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&);   // 0x0076FD40
    void insert(iterator __pos, const_iterator __first, const_iterator __last) { _M_insert_dispatch(__pos, __first, __last, __false_type()); }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

namespace NCreatureHitNotification {
class CFlash {                                              // Ego_r.pdb (retail 8 bytes)
public:
    std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry;   // +0x00
    long                                                      CreationFrame;   // +0x04
};
}

template NCreatureHitNotification::CFlash* __fastcall std::__uninitialized_fill_n<NCreatureHitNotification::CFlash*, unsigned int, NCreatureHitNotification::CFlash>(
    NCreatureHitNotification::CFlash*, unsigned int, const NCreatureHitNotification::CFlash&, const std::__false_type&);