// std::_Deque_iterator_base<NUISystem::CComponent*>::_M_subtract(const _Deque_iterator_base& x) const -- the iterator difference: whole
// buffers between the two nodes plus the two partial ends
// Leaf: ?_M_subtract@?$_Deque_iterator_base@PAVCComponent@NUISystem@@@std@@QBEHABU12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059A8F3, 43 bytes, a COMDAT in a size-optimised TU.
// Identity (NF83, round 7, q11025 self): STLport 4.6 _deque.h: _M_subtract lives in _Deque_iterator_base and _Deque_iterator::operator-
//   is the inline `return _M_subtract(x);`; retail 0x0059AAE3 / 0x0059AC1A are the 5-byte `jmp 0x0059A8F3` out-of-line operator-
//   instances, and the landed 0x0059AFD7 / 0x0059AAE3 / NF79 0x00596763 bind this address under this spelling. Descriptive-by-library
//   (STLport; both PDBs are Dinkumware builds). Was a local std::_Deque_iterator<T> model whose leaf was its operator-.
// Port (NF83): one explicit instantiation of the stlport/fable_deque.h body (NF83 overlay opt-in FABLE_RETAIL_STLP_DEQUE_MSUBTRACT_BODY).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_DEQUE_MSUBTRACT_BODY
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template ptrdiff_t std::_Deque_iterator_base<NUISystem::CComponent*>::_M_subtract(const std::_Deque_iterator_base<NUISystem::CComponent*>&) const;
