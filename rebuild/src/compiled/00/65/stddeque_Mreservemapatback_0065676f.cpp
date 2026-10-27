// std::deque::_M_reserve_map_at_back(size_t __nodes_to_add) -- (<std::pair<CCharString, long>>) make room in the node map for n more nodes at the back
// Leaf: ?_M_reserve_map_at_back@?$deque@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065676F, 38 bytes, thiscall, `ret 4`.
// Identity (proven): it calls deque<std::pair<CCharString, long>>::_M_reallocate_map 0x00656082 (family M070, same TU: 0x80-byte nodes (16 eight-byte elements); its caller chain is _M_reserve_map_at_back 0x0065676F <- _M_push_back_aux_v 0x006569DA, which copies through pair<CCharString, long>'s copy constructor 0x0050B7B0 (NPlayerGui::CDrawGuildSeal::Messages, Ego_r.pdb queue<pair<CCharString, long> >)); its caller is
//   that deque's _M_push_back_aux_v 0x006569DA.
// Callees: deque::_M_reallocate_map(size_t, bool) 0x00656082.
// Shape (family M071, representative 0x00493FA8): STLport 4.6 _deque.h `_M_reserve_map_at_back` -- not inlined in a size
//   TU (`optimize("s")`): the free map slots behind finish compared with n + 1, `_M_reallocate_map(n, false)`.
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
    _Value _M_data;
};
template <class _Tp> struct _Deque_iterator_base {
    _Tp* _M_cur;
    _Tp* _M_first;
    _Tp* _M_last;
    _Tp** _M_node;
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> { };
template <class _Tp, class _Alloc = allocator<_Tp> > class deque {
public:
    typedef unsigned int size_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator _M_start;                                    // +0x00
    iterator _M_finish;                                   // +0x10
    _STLP_alloc_proxy<_Map_pointer, _Tp*, allocator<_Tp*> > _M_map;   // +0x20
    _STLP_alloc_proxy<size_type, _Tp, _Alloc> _M_map_size;            // +0x24
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size._M_data - (this->_M_finish._M_node - this->_M_map._M_data))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);   // 0x00656082
};
template void deque<std::pair<CCharString, long>, allocator<std::pair<CCharString, long> > >::_M_reserve_map_at_back(unsigned int);
}