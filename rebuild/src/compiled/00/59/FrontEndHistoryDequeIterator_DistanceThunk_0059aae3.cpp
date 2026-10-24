// std::_Deque_iterator<NUISystem::CComponent*, _Nonconst_traits>::operator-(const _Deque_iterator&) const -- forwards to the base _M_subtract
// Leaf: ??G?$_Deque_iterator
// Retail: Fable.exe @ 0x0059AAE3, 5 bytes: a lone `jmp 0x0059A8F3`. STLport 4.6 keeps the element distance in
// _Deque_iterator_base::_M_subtract (0x0059A8F3, landed as the size-TU out-of-line body) and every traits flavour of
// _Deque_iterator gets a one-line operator- that forwards `this` and the stack argument unchanged, so VC7.1 emits a
// sibling-call `jmp` (no frame, ecx and [esp+4] untouched). 0x0059AC1A is the second flavour's identical thunk.
// No xrefs reach 0x0059AAE3 by call: it is reached only through inlined deque size()/operator- users.
// Opt: size TU, as the CFrontEndManager deque code around it.
#pragma optimize("s", on)
#include <stddef.h>
namespace NUISystem { class CComponent; }
namespace std {
template <class T> struct _Nonconst_traits {};
template <class T> struct _Deque_iterator_base {
    typedef ptrdiff_t difference_type;
    T*  _M_cur;                                  // +0x00
    T*  _M_first;                                // +0x04
    T*  _M_last;                                 // +0x08
    T** _M_node;                                 // +0x0c
    difference_type _M_subtract(const _Deque_iterator_base& x) const;   // 0x0059A8F3 (out of line)
};
template <class T, class Traits> struct _Deque_iterator : public _Deque_iterator_base<T> {
    typedef _Deque_iterator_base<T> _Base;
    ptrdiff_t operator-(const _Deque_iterator& x) const { return this->_M_subtract(x); }
};
template struct _Deque_iterator<NUISystem::CComponent*, _Nonconst_traits<NUISystem::CComponent*> >;
}
