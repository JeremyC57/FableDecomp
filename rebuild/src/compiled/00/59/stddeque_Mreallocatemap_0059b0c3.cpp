// std::deque::_M_reallocate_map(size_t __nodes_to_add, bool __add_at_front) -- (<NUISystem::CComponent*>) grow or recentre the node map, then re-point start and finish
// Leaf: ?_M_reallocate_map@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXI_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B0C3, 251 bytes, thiscall, `ret 8`.
// Identity (proven): 0x80-byte nodes (32 pointers); the same TU's _Deque_base constructor 0x0059AEF9 / _M_initialize_map 0x0059AC9F are deque<NUISystem::CComponent*>'s (CFrontEndManager::m_stackScreens; its users are the landed CFrontEndManager members).
// Callees: std::__copy_trivial 0x00405BA0, std::__copy_trivial_backward 0x00405BC0 (both `__fastcall`, out of line in
//   this size TU), malloc 0x00BFEA0E, free 0x00BFEA14.
// Shape (family M070, representative 0x00493D30): STLport 4.6 _deque.c `_M_reallocate_map`; frame-pointer size TU
//   (`optimize("y", off)` + `("s", on)`): new_map_size / new_map in the two frame words, `(max)` by reference (the
//   `lea edx,[ebp+8]` select), the map allocator's allocate/deallocate and `_M_set_node` inline.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace NUISystem { class CComponent; }
namespace std {
void* __fastcall __copy_trivial(const void* __first, const void* __last, void* __result);            // 0x00405BA0
void* __fastcall __copy_trivial_backward(const void* __first, const void* __last, void* __result);   // 0x00405BC0
template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator {
public:
    allocator() { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    __forceinline void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> struct _Deque_iterator_base {
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    typedef _Tp** _Map_pointer;
    _Tp* _M_cur;
    _Tp* _M_first;
    _Tp* _M_last;
    _Map_pointer _M_node;
    __forceinline void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(__buffer_size);
    }
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
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);
};
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_reallocate_map(size_type __nodes_to_add, bool __add_at_front)
{
    size_type __old_num_nodes = this->_M_finish._M_node - this->_M_start._M_node + 1;
    size_type __new_num_nodes = __old_num_nodes + __nodes_to_add;

    _Map_pointer __new_nstart;
    if (this->_M_map_size._M_data > 2 * __new_num_nodes) {
        __new_nstart = this->_M_map._M_data + (this->_M_map_size._M_data - __new_num_nodes) / 2
                       + (__add_at_front ? __nodes_to_add : 0);
        if (__new_nstart < this->_M_start._M_node)
            __copy_trivial(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart);
        else
            __copy_trivial_backward(this->_M_start._M_node, this->_M_finish._M_node + 1,
                                    __new_nstart + __old_num_nodes);
    } else {
        size_type __new_map_size =
            this->_M_map_size._M_data + (max)(this->_M_map_size._M_data, __nodes_to_add) + 2;

        _Map_pointer __new_map = this->_M_map.allocate(__new_map_size);
        __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2
                       + (__add_at_front ? __nodes_to_add : 0);
        __copy_trivial(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart);
        this->_M_map.deallocate(this->_M_map._M_data, this->_M_map_size._M_data);

        this->_M_map._M_data = __new_map;
        this->_M_map_size._M_data = __new_map_size;
    }

    this->_M_start._M_set_node(__new_nstart);
    this->_M_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
}
template void deque<NUISystem::CComponent*, allocator<NUISystem::CComponent* > >::_M_reallocate_map(unsigned int, bool);
}