// std::_Deque_iterator<NUISystem::CComponent*, _Nonconst_traits>::operator-(const _Deque_iterator&) const -- forwards to the base _M_subtract
// Leaf: ??G?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@std@@QBEHABU01@@Z
// Retail: Fable.exe @ 0x0059AAE3, 5 bytes: a lone `jmp 0x0059A8F3`. STLport 4.6 keeps the element distance in
// _Deque_iterator_base::_M_subtract (0x0059A8F3, landed as the size-TU out-of-line body) and every traits flavour of
// _Deque_iterator gets a one-line operator- that forwards `this` and the stack argument unchanged, so VC7.1 emits a
// sibling-call `jmp` (no frame, ecx and [esp+4] untouched). 0x0059AC1A is the second flavour's identical thunk.
// No xrefs reach 0x0059AAE3 by call: it is reached only through inlined deque size()/operator- users.
// Opt: size TU, as the CFrontEndManager deque code around it.
// NF84 (round 7, q11052 binder): moved onto the shared stlport/fable_deque.h (NF79's FABLE_RETAIL_STLP_STACK_SIZE_PUSH +
//   FABLE_RETAIL_STLP_STACK_TOP_POP declare _M_subtract and this operator-; the landed 0x00596763 uses the same pair); the local
//   namespace-std iterator models are gone. The call already bound the STLport _M_subtract spelling that 0x0059A8F3 defines.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template ptrdiff_t std::_Deque_iterator<NUISystem::CComponent*, std::_Nonconst_traits<NUISystem::CComponent*> >::operator-(
    const std::_Deque_iterator<NUISystem::CComponent*, std::_Nonconst_traits<NUISystem::CComponent*> >& x) const;
