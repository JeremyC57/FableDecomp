// std::_Deque_base::_Deque_base(const allocator<NUISystem::CComponent*>& __a, size_t __num_elements) -- (<NUISystem::CComponent*>) empty iterators and map, then size the map for n elements
// Leaf: ??0?$_Deque_base@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@ABV?$allocator@PAVCComponent@NUISystem@@@1@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AEF9, 49 bytes, thiscall, `ret 8`.
// Identity (proven): it calls the same TU's _Deque_base::_M_initialize_map 0x0059AC9F (landed); the deque constructor 0x0059B039 that calls it is used by the landed NUISystem::CFrontEndManager members (ClearComponents 0x0059525D, ChangeStateFirstTime 0x005952C3, ...); Ego_r.pdb CFrontEndManager::m_stackScreens is stack<NUISystem::CComponent*> (the callee's landed source places it in the CFrontEndManager screen-stack module).
// Callees: _Deque_base::_M_initialize_map(size_t) 0x0059AC9F.
// Shape (family M069, representative 0x00493BF4): STLport 4.6 `_Deque_base(const allocator_type& __a, size_t __num_elements)
//   : _M_start(), _M_finish(), _M_map(__a, 0), _M_map_size(__a, 0) { _M_initialize_map(__num_elements); }` -- ten zero
//   stores (two {cur, first, last, node} iterators, the map pointer and size proxies), n pushed from its slot; size TU.
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
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
    void _M_initialize_map(unsigned int);                 // 0x0059AC9F
    iterator _M_start;                                    // +0x00
    iterator _M_finish;                                   // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc> _M_map_size;            // +0x24
};
template class _Deque_base<NUISystem::CComponent*, allocator<NUISystem::CComponent* > >;
}