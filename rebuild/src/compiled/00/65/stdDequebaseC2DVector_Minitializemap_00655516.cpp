// std::_Deque_base<C2DVector>::_M_initialize_map(unsigned int) -- size and allocate the block map for n elements, create the blocks and point start/finish at them
// Leaf: _M_initialize_map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00655516, 150 bytes, `ret 4` (thiscall: num_elements).
// Callees: malloc @ 0x00BFEA0E (the map allocator's allocate, inline), _Deque_base::_M_create_nodes(nstart, nfinish)
// @ 0x00654E46 (out of line, landed: one 128-byte block per map slot, `push 80h`).
// Label: the catalog "CEngineInterpolationInfo_Initialize" is propagated: the body is STLport 4.x
// `_Deque_base::_M_initialize_map` for an 8-byte element (`shr ebx,4`: 16 elements per 128-byte block; `n & 0xf` for
// the finish cursor). The landed _M_create_nodes 0x00654E46 names the element C2DVector (Ego_r.pdb's 8-byte deque
// element candidate); kept here so the pair stays consistent.
// Shape: num_nodes = n / 16 + 1, map_size = max(8, num_nodes + 2) through the const-reference `(max)` (8 in [ebp-8],
// num_nodes + 2 in [ebp-4], `lea` select on `ja`), map = allocate(map_size), nstart = map + (map_size - num_nodes) / 2,
// nfinish = nstart + num_nodes, _M_create_nodes, start/finish `_M_set_node`, start.cur = start.first, finish.cur =
// finish.first + n % 16. No `_STLP_TRY` (exceptions off).
// Layout: _M_start +0x00, _M_finish +0x10 ({cur, first, last, node}), _M_map +0x20, _M_map_size +0x24 (both
// `_STLP_alloc_proxy` words).
// Opt: frame-pointer size TU (`leave`, `push 8 ; pop ecx`): `optimize("y", off)` + `optimize("s", on)`; same body as
// the landed 0x0059AC9F (deque<CComponent*>) with the element changed.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class C2DVector { public: float X, Y; };

namespace std {

__forceinline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }

template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> struct _Deque_iterator {
    typedef _Tp** _Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    _Tp*         _M_cur;                                 // +0x00
    _Tp*         _M_first;                               // +0x04
    _Tp*         _M_last;                                // +0x08
    _Map_pointer _M_node;                                // +0x0c
    __forceinline void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + ptrdiff_t(_S_buffer_size());
    }
};

template <class _Tp> struct _Deque_base {
    enum { _S_initial_map_size = 8 };
    _Deque_iterator<_Tp>                                 _M_start;      // +0x00
    _Deque_iterator<_Tp>                                 _M_finish;     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> >     _M_map;        // +0x20
    _STLP_alloc_proxy<size_t, _Tp, allocator<_Tp> >      _M_map_size;   // +0x24
    void _M_initialize_map(size_t __num_elements);
    void _M_create_nodes(_Tp** __nstart, _Tp** __nfinish);              // 0x00654E46
};

template <class _Tp>
void _Deque_base<_Tp>::_M_initialize_map(size_t __num_elements)
{
    size_t __num_nodes = __num_elements / __deque_buf_size(sizeof(_Tp)) + 1;

    _M_map_size._M_data = (max)((size_t)_S_initial_map_size, __num_nodes + 2);
    _M_map._M_data = _M_map.allocate(_M_map_size._M_data);

    _Tp** __nstart = _M_map._M_data + (_M_map_size._M_data - __num_nodes) / 2;
    _Tp** __nfinish = __nstart + __num_nodes;

    _M_create_nodes(__nstart, __nfinish);
    _M_start._M_set_node(__nstart);
    _M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    _M_finish._M_cur = _M_finish._M_first + __num_elements % __deque_buf_size(sizeof(_Tp));
}

template struct _Deque_base<C2DVector>;

}