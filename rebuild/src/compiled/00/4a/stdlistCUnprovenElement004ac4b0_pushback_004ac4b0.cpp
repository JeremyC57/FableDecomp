// std::list<CUnprovenElement_004ac4b0*>::push_back(const CUnprovenElement_004ac4b0*& x) -- STLport push_back: malloc a node, copy the element into it and link it before the sentinel
// Leaf: ?push_back@?$list@PAVCUnprovenElement_004ac4b0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AC4B0, 45 bytes, thiscall `ret 4`.
// Identity (descriptive): callers CWorldMap::PopulateRegionWithVillagers 0x00502E90 (on a list returned by 0x0051BF90) and CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed 0x006C6040 (a local list); the 4-byte element is not named.
// Callee: malloc 0x00BFEA0E (the STLport node allocator: 0xC-byte node, the element at +8).
// Shape (family K107, representative 0x004AC480): the landed list push_back 0x007A3870 (family H010) with a 4-byte element:
//   the sentinel read before the allocation, the null-tested placement copy, then the four link stores.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class CUnprovenElement_004ac4b0;
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

template void std::list<CUnprovenElement_004ac4b0* >::push_back(CUnprovenElement_004ac4b0* const&);