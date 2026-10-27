// std::_Deque_base::_Deque_base(const allocator<std::pair<CCharString, long>>& __a, size_t __num_elements) -- (<std::pair<CCharString, long>>) empty iterators and map, then size the map for n elements
// Leaf: ??0?$_Deque_base@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@QAE@ABV?$allocator@U?$pair@VCCharString@@J@std@@@1@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00655C55, 49 bytes, thiscall, `ret 8`.
// Identity (proven): it calls the same TU's _Deque_base::_M_initialize_map 0x00655516 (landed); the deque constructor 0x00656D0B that calls it is called by NPlayerGui::CDrawGuildSeal::CDrawGuildSeal 0x00651904: Ego_r.pdb CDrawGuildSeal::Messages is queue<pair<CCharString, long> >; the same TU's _M_push_back_aux_v 0x006569DA copies through pair<CCharString, long>'s copy constructor 0x0050B7B0 (the landed push_back 0x00656E0C). The landed _M_initialize_map 0x00655516 names the 8-byte element C2DVector (a guess) -- corrected here.
// Callees: _Deque_base::_M_initialize_map(size_t) 0x00655516.
// Shape (family M069, representative 0x00493BF4): STLport 4.6 `_Deque_base(const allocator_type& __a, size_t __num_elements)
//   : _M_start(), _M_finish(), _M_map(__a, 0), _M_map_size(__a, 0) { _M_initialize_map(__num_elements); }` -- ten zero
//   stores (two {cur, first, last, node} iterators, the map pointer and size proxies), n pushed from its slot; size TU.
#pragma optimize("s", on)
class CCharString {                                       // 4 bytes (PStringData)
public:
    void* PStringData;
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp> struct _Deque_iterator_base {
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    _Tp* _M_cur;
    _Tp* _M_first;
    _Tp* _M_last;
    _Tp** _M_node;
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    _Deque_iterator() { }
};
template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Alloc allocator_type;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Deque_base(const allocator_type& __a, unsigned int __num_elements)
        : _M_start(), _M_finish(), _M_map(allocator<_Tp*>(), 0), _M_map_size(__a, (unsigned int)0)
    {
        _M_initialize_map(__num_elements);
    }
    void _M_initialize_map(unsigned int);                 // 0x00655516
    iterator _M_start;                                    // +0x00
    iterator _M_finish;                                   // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc> _M_map_size;            // +0x24
};
template class _Deque_base<std::pair<CCharString, long>, allocator<std::pair<CCharString, long> > >;
}