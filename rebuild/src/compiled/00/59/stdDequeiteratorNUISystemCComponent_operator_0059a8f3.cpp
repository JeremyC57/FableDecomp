// std::_Deque_iterator<NUISystem::CComponent*>::operator-(const _Deque_iterator&) const -- element distance between two deque iterators
// Leaf: ??G?$_Deque_iterator
// Retail: Fable.exe @ 0x0059A8F3, 43 bytes, `ret 4`, returns the distance in eax. Catalog label
// NThingMap::CThingMapManager::GetMemoryFootprint is wrong: the body is STLport's deque iterator difference,
// 32 * (_M_node - x._M_node - 1) + (_M_cur - _M_first) + (x._M_last - x._M_cur), for 4-byte elements in 128-byte blocks
// (`shl 5`, `lea [..-20h]`). Its 13 callers (0x005952AB .. 0x0059B513, two tail jumps from 0x0059AAE3/0x0059AC1A) sit with
// the CFrontEndManager screen-stack deque code (0x0059AAF9 _M_increment, 0x0059B61C push_back), hence CComponent*.
// Layout: iterator {_M_cur +0, _M_first +4, _M_last +8, _M_node +0xc}, as 0x0059AAF9.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#include <stddef.h>
namespace NUISystem { class CComponent; }
namespace std {
inline size_t __deque_buf_size(size_t n) { return n < 128 ? (128 / n) : 1; }
template <class T> struct _Deque_iterator {
    typedef T** _Map_pointer;
    typedef ptrdiff_t difference_type;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(T)); }
    T*          _M_cur;                                  // +0x00
    T*          _M_first;                                // +0x04
    T*          _M_last;                                 // +0x08
    _Map_pointer _M_node;                                // +0x0c
    difference_type operator-(const _Deque_iterator& x) const
    {
        return difference_type(_S_buffer_size()) * (_M_node - x._M_node - 1) +
            (_M_cur - _M_first) + (x._M_last - x._M_cur);
    }
};
template struct _Deque_iterator<NUISystem::CComponent*>;
}