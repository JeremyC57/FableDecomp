// std::list::push_back(const value_type& x) -- (list<CCountedPointer<CSoundEmitterLH::CActiveSound > >) STLport push_back: malloc a node, copy the counted pointer into it (taking a reference) and link it before the sentinel
// Leaf: ?push_back@?$list@V?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@V?$allocator@V?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@@std@@@std@@QAEXABV?$CCountedPointer@VCActiveSound@CSoundEmitterLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACCC0, 59 bytes (+ int3 padding to 64), thiscall `ret 4`.
// Identity (descriptive): list<CCountedPointer<CSoundEmitterLH::CActiveSound > >; evidence: DESCRIPTIVE -- its callers are CSoundEmitterLH vtable 0x0129E6EC slots 3 / 5 (0x00AABF60, the landed 0x00AAC1D0) and 0x00AABD50, each pushing a freshly started active sound (the landed CSoundEmitterLH::CActiveSound constructor 0x00AAB910, CCountedPointer(T*) 0x00AAC820) onto the emitter's list at +0x14; the audio layer is not in Ego_r.pdb, so the class names are the landed descriptive ones.
// Label: the manifest's "_Buynode<CCountedPointer<CSpawnedFuncBase>_>" is a propagated Dinkumware spelling (family H010: seven rows that differ only in the
//   malloc displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: malloc @ 0x00BFEA0E (the STLport node allocator: 0x10-byte node, the 8-byte counted pointer at +8).
// Shape: STLport 4.6 `push_back(x) { insert(end(), x); }` with insert and _M_create_node inline: the sentinel is read
//   before the allocation, the element is placement-copied (null test on &node->_M_data; CCountedPointer's copy
//   constructor: Data, Info, then ++*Info when Info is set), then `tmp->next = n; tmp->prev = n->prev; prev->next = tmp;
//   n->prev = tmp` (the landed list push_back 0x004BA420 model).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class CSoundEmitterLH {
public:
    class CActiveSound;
};
class CCPPointerInfo {                                  // the shared reference count block
public:
    long Count;                                         // +0x00
};
template <class T> class CCountedPointer {             // 8 bytes: object + shared reference-count block
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->Count;
    }
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int n) { return (_Tp*)malloc(n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 the sentinel
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator end() { return iterator(this->_M_node._M_data); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}   // namespace std

template void std::list<CCountedPointer<CSoundEmitterLH::CActiveSound > >::push_back(const CCountedPointer<CSoundEmitterLH::CActiveSound >&);