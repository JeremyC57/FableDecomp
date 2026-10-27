// std::_Deque_base::_M_initialize_map(size_t num_elements) -- (deque<ETutorialCategory>) size and allocate the node map, allocate the centred run of 0x80-byte nodes, point start/finish at them
// Leaf: ?_M_initialize_map@?$_Deque_base@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F4A0, 187 bytes, thiscall, `ret 4`.
// Identity (probable): 4-byte element (node shift 5, mask 0x1F: 32 per 0x80-byte node); the CUserProfileManager constructor 0x0040CEC0 zeroes the ten deque words at +0x0..+0x24 of its member and calls it with 0 (0x0040CF5A); Ego_r.pdb's CUserProfileManager has one deque, PendingTutorialCategories (queue<ETutorialCategory, deque<ETutorialCategory> >), and ETutorialCategory is 4 bytes. Other callers 0x00410550 / 0x00410F30 are that deque's own members. No binding yet; the member offset in retail is not pinned to the PDB's (retail containers are smaller), hence probable.
// Callees: malloc 0x00BFEA0E (the map, behind its n != 0 test, and every node: allocate(buffer_size())).
// Shape (family M009, representative 0x0040F4A0): STLport 4.6 _M_initialize_map with _M_create_nodes inlined (a speed TU,
//   unlike the landed size-TU twin 0x00493875): `(max)(size_t(_S_initial_map_size), num_nodes + 2)` through two by-reference
//   temporaries (the `sub esp,8` words), nstart/nfinish centred, the node loop (`cur < nfinish`), `_M_set_node` for both
//   ends, then the two cursors (`num_elements % buffer_size()`).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };
namespace std {
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline unsigned int __deque_buf_size(unsigned int __size) { return __size < 128 ? (unsigned int)(128 / __size) : 1u; }
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> struct _Deque_iterator {
    _Tp*  _M_cur;                                         // +0x00
    _Tp*  _M_first;                                       // +0x04
    _Tp*  _M_last;                                        // +0x08
    _Tp** _M_node;                                        // +0x0c
    static unsigned int _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    void _M_set_node(_Tp** __new_node) { _M_last = (_M_first = *(_M_node = __new_node)) + _S_buffer_size(); }
};
template <class _Tp, class _Alloc> class _Deque_base {
public:
    enum { _S_initial_map_size = 8 };
    static unsigned int buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    _Deque_iterator<_Tp> _M_start;                        // +0x00
    _Deque_iterator<_Tp> _M_finish;                       // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc> _M_map_size;          // +0x24
    void _M_initialize_map(unsigned int __num_elements);
    void _M_create_nodes(_Tp** __nstart, _Tp** __nfinish)
    {
        _Tp** __cur;
        for (__cur = __nstart; __cur < __nfinish; ++__cur)
            *__cur = _M_map_size.allocate(buffer_size());
    }
};
template <class _Tp, class _Alloc> void _Deque_base<_Tp, _Alloc>::_M_initialize_map(unsigned int __num_elements)
{
    unsigned int __num_nodes = __num_elements / buffer_size() + 1;
    _M_map_size._M_data = (max)((unsigned int)_S_initial_map_size, __num_nodes + 2);
    _M_map._M_data = _M_map.allocate(_M_map_size._M_data);
    _Tp** __nstart = _M_map._M_data + (_M_map_size._M_data - __num_nodes) / 2;
    _Tp** __nfinish = __nstart + __num_nodes;
    _M_create_nodes(__nstart, __nfinish);
    _M_start._M_set_node(__nstart);
    _M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    _M_finish._M_cur = _M_finish._M_first + __num_elements % buffer_size();
}
template void _Deque_base<ETutorialCategory, allocator<ETutorialCategory > >::_M_initialize_map(unsigned int);
}