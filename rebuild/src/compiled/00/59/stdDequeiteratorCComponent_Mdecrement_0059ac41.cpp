// std::_Deque_iterator<CComponent*>::_M_decrement() -- step back one element, moving to the previous 128-byte block at the start of one
// Leaf: _M_decrement
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AC41, 35 bytes, plain `ret` (no return value).
// Callers 0x0059AF56 / 0x0059B04E sit with the CFrontEndManager screen-stack deque code (0x0059AAF9 _M_increment,
// 0x0059A8F3 operator-, 0x0059AD35 _M_pop_back_aux, all landed), hence CComponent* (4-byte element, 128-byte block).
// Label: the catalog "lua_getglobals" is wrong (no Lua state here): the body is STLport 4.x
// `_Deque_iterator_base::_M_decrement()` -- at _M_first (+0x04) `_M_set_node(_M_node - 1)` stores node (+0x0c),
// first = *node and last = first + 32 elements (+0x08), then `_M_cur = _M_last`; finally `--_M_cur` in place
// (`add dword ptr [ecx],-4`).
// Shape: size TU (unaligned entry, in-place memory decrement): `#pragma optimize("s", on)`, same as the landed
// _M_increment twin.
#pragma optimize("s", on)
#include <stddef.h>
namespace NUISystem { class CComponent; }
namespace std {
inline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> struct _Deque_iterator {
    typedef T** _Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(T)); }
    T*          _M_cur;                                  // +0x00
    T*          _M_first;                                // +0x04
    T*          _M_last;                                 // +0x08
    _Map_pointer _M_node;                                // +0x0c
    void _M_set_node(_Map_pointer new_node)
    {
        _M_last = (_M_first = *(_M_node = new_node)) + ptrdiff_t(_S_buffer_size());
    }
    void _M_decrement()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
    }
};
template struct _Deque_iterator<NUISystem::CComponent*>;
}