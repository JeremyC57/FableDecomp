// std::_Deque_base::_Deque_base(const allocator<CSubtitleRenderer::CSetTextInfo>& __a, size_t __num_elements) -- (<CSubtitleRenderer::CSetTextInfo>) empty iterators and map, then size the map for n elements
// Leaf: ??0?$_Deque_base@VCSetTextInfo@CSubtitleRenderer@@V?$allocator@VCSetTextInfo@CSubtitleRenderer@@@std@@@std@@QAE@ABV?$allocator@VCSetTextInfo@CSubtitleRenderer@@@1@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493BF4, 49 bytes, thiscall, `ret 8`.
// Identity (proven): it calls the same TU's _Deque_base::_M_initialize_map 0x00493875 (landed); the deque constructor 0x00494168 that calls it is called by the landed CSubtitleRenderer::CSubtitleRenderer 0x0049166E (the text queue); Ego_r.pdb CSubtitleRenderer::TextInfoQueue is queue<CSubtitleRenderer::CSetTextInfo> (its only 0x1C-byte deque element; four elements per 0x70-byte node in the callee).
// Callees: _Deque_base::_M_initialize_map(size_t) 0x00493875.
// Shape (family M069, representative 0x00493BF4): STLport 4.6 `_Deque_base(const allocator_type& __a, size_t __num_elements)
//   : _M_start(), _M_finish(), _M_map(__a, 0), _M_map_size(__a, 0) { _M_initialize_map(__num_elements); }` -- ten zero
//   stores (two {cur, first, last, node} iterators, the map pointer and size proxies), n pushed from its slot; size TU.
#pragma optimize("s", on)
class CSubtitleRenderer {
public:
    class CSetTextInfo {                                  // Ego_r.pdb, 0x1C bytes
    public:
        unsigned long Members[7];                         // four CWideStrings, DisplayFlags, DisplayTime, UpdateWhilePaused
    };
};
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
    void _M_initialize_map(unsigned int);                 // 0x00493875
    iterator _M_start;                                    // +0x00
    iterator _M_finish;                                   // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc> _M_map_size;            // +0x24
};
template class _Deque_base<CSubtitleRenderer::CSetTextInfo, allocator<CSubtitleRenderer::CSetTextInfo > >;
}