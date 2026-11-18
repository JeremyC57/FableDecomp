// std::_Deque_iterator_base<NUISystem::CComponent*>::_M_decrement() -- step the iterator back one element, moving to the previous buffer
// (the end of the previous node) when it is at the start of its buffer
// Leaf: ?_M_decrement@?$_Deque_iterator_base@PAVCComponent@NUISystem@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AC41, 35 bytes, a COMDAT in a size-optimised TU.
// Identity (NF83, round 7, q11023 self): STLport 4.6 _deque.h declares _M_decrement in _Deque_iterator_base<T>; the landed back()
//   0x0059B039 (NF79) binds this address as ?_M_decrement@?$_Deque_iterator_base@...; its twin _M_increment 0x0059AAF9 is bound so by
//   0x0059AFD7. Descriptive-by-library (STLport; both PDBs carry Dinkumware deque members only). Was a local
//   std::_Deque_iterator<T> model whose leaf was ?_M_decrement@?$_Deque_iterator@...
// Port (NF83): one explicit instantiation of the stlport/fable_deque.h body (FABLE_STLPORT_DEQUE_PUSH_BACK carries it).
#pragma optimize("s", on)
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template void std::_Deque_iterator_base<NUISystem::CComponent*>::_M_decrement();
