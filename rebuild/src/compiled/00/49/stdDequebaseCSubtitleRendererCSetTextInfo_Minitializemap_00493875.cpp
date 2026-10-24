// std::_Deque_base<CSubtitleRenderer::CSetTextInfo>::_M_initialize_map(size_t num_elements) -- size and allocate the node map, create the centred run of nodes, point start/finish at them
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493875, 156 bytes, `ret 4`.
// Callers: the _Deque_base(const allocator&, size_t) constructor @ 0x00493BF4 (zeroes the ten words first; itself
// called from 0x00493CEA and 0x00494173).
// Callees: malloc @ 0x00BFEA0E (the map: `_M_map.allocate(n)` behind its n != 0 test), _M_create_nodes(nstart, nfinish)
// @ 0x0049369B.
// Element: 0x1C bytes, four per 0x70-byte node (STLport `__deque_buf_size`: 128 / 28). Ego_r.pdb's only 0x1C-byte
// deque element is CSubtitleRenderer::CSetTextInfo (four CWideStrings, DisplayFlags, DisplayTime, UpdateWhilePaused) --
// CSubtitleRenderer::TextInfoQueue is a queue<CSetTextInfo> -- so the instantiation is named after it (inferred).
// Layout: STLport 4.x _Deque_base: _M_start +0x00 and _M_finish +0x10 (iterators {cur, first, last, node}), _M_map
// +0x20, _M_map_size +0x24.
// Shape: STLport `_M_initialize_map` with no exception handling: `num_nodes = n / buffer_size() + 1`;
// `map_size = (max)(size_t(_S_initial_map_size), num_nodes + 2)` (both by-reference temporaries: 8 in a frame word,
// num_nodes + 2 in the dead `n` slot); nstart/nfinish centred; `_M_set_node` for both ends, then the two cursors.
// Frame-pointer size TU (`optimize("y",off)` + `("s",on)`); `(max)` and the allocator are `__forceinline`.
// Label correction: catalog "CEngineInterpolationInfo_Initialize" is propagated.
// Leaf: _M_initialize_map
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

class CWideString {
public:
    void* PStringData;
};

class CSubtitleRenderer {
public:
    class CSetTextInfo {                                    // 0x1c bytes
    public:
        CWideString   Text;                                 // +0x00
        CWideString   Button1Text;                          // +0x04
        CWideString   Button2Text;                          // +0x08
        CWideString   Button3Text;                          // +0x0c
        unsigned long DisplayFlags;                         // +0x10
        float         DisplayTime;                          // +0x14
        bool          UpdateWhilePaused;                    // +0x18
    };
};

namespace std {
template <class _Tp> __forceinline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline unsigned int __deque_buf_size(unsigned int __size) { return __size < 128 ? (unsigned int)(128 / __size) : 1u; }

template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
public:
    _Value _M_data;
};

template <class _Tp> struct _Deque_iterator {
    _Tp*  _M_cur;                                           // +0x00
    _Tp*  _M_first;                                         // +0x04
    _Tp*  _M_last;                                          // +0x08
    _Tp** _M_node;                                          // +0x0c
    static unsigned int _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    void _M_set_node(_Tp** __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + _S_buffer_size();
    }
};

template <class _Tp> class _Deque_base {
public:
    enum { _S_initial_map_size = 8 };
    static unsigned int buffer_size() { return __deque_buf_size(sizeof(_Tp)); }

    _Deque_iterator<_Tp> _M_start;                          // +0x00
    _Deque_iterator<_Tp> _M_finish;                         // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*> _M_map;                  // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp> _M_map_size;       // +0x24

    void _M_create_nodes(_Tp** __nstart, _Tp** __nfinish);  // 0x0049369B
    void _M_initialize_map(unsigned int __num_elements);
};

template <class _Tp> void _Deque_base<_Tp>::_M_initialize_map(unsigned int __num_elements)
{
    unsigned int __num_nodes = __num_elements / buffer_size() + 1;

    _M_map_size._M_data = (max)((unsigned int)_S_initial_map_size, __num_nodes + 2);
    _M_map._M_data = _M_map.allocate(_M_map_size._M_data);

    _Tp** __nstart = _M_map._M_data + (_M_map_size._M_data - __num_nodes) / 2;
    _Tp** __nfinish = __nstart + __num_nodes;

    _M_create_nodes(__nstart, __nfinish);
    _M_start._M_set_node(__nstart);
    this->_M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    this->_M_finish._M_cur = this->_M_finish._M_first + __num_elements % buffer_size();
}

template void _Deque_base<CSubtitleRenderer::CSetTextInfo>::_M_initialize_map(unsigned int);
}