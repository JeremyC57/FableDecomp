#pragma optimize("y", off)
#pragma optimize("s", on)
// std::deque<NUISystem::CComponent*>::back() -- STLport 4.6 deque::back(): copies the finish iterator, steps it back one element and returns a reference to that element
// Leaf: ?back@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEAAPAVCComponent@NUISystem@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B039, length 33 bytes. Frame-pointer size TU (`push ebp ; mov ebp,esp`,
// the 16-byte iterator copied with four `movsd`, `leave`): `optimize("y",off)` + `("s",on)`.
// Callees: _Deque_iterator_base<NUISystem::CComponent*>::_M_decrement() @ 0x0059AC41 (out of line in this TU).
// Identity: the COMDAT the NUISystem::CFrontEndManager screen stack (std::stack<NUISystem::CComponent*>, i.e. a deque) calls from
// its inline stack::top() (0x00599E9C, NF76 row 0x00599E3F); STLport 4.6 _deque.h `iterator tmp = _M_finish; --tmp; return *tmp;`.
// NF79 (round 7, q10966): re-authored as the explicit instantiation of the shared header's deque::back (stlport/fable_deque.h,
//   FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES); was the local stand-in CDwordDeque::back with an SDequeIterator model.
#define FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template std::deque<NUISystem::CComponent*>::reference std::deque<NUISystem::CComponent*>::back();
