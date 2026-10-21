// std::_Deque_iterator<NUISystem::CComponent*>::_M_increment() -- step to the next element, moving to the next 128-byte block at the end of one
// Leaf: _M_increment
// Retail: Fable.exe @ 0x0059AAF9, 38 bytes, plain `ret` (no return value). Catalog label
// CTCInventoryExperience::IncrementStatIndicator is wrong: the body is STLport's deque iterator increment -- ++_M_cur
// (+0x00); at _M_last (+0x08) _M_set_node(_M_node + 1) (+0x0c; _M_first +0x04 = *node, _M_last = _M_first + 32 elements =
// 0x80 bytes) and _M_cur = _M_first. Callers 0x0059ADF3/0x0059ADFF/0x0059AFFE/0x0059B006 sit with the
// CFrontEndManager::m_stackScreens deque code (0x0059AA43 _M_create_nodes, 0x0059B61C push_back), hence CComponent*.
// Opt: size TU (unaligned entry, in-place `add dword ptr [ecx],4`).
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
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
};
template struct _Deque_iterator<NUISystem::CComponent*>;
}