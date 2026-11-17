// std::deque<NUISystem::CComponent*>::pop_back() -- STLport 4.6 deque::pop_back(): steps the finish cursor back inside its node, else the node-crossing _M_pop_back_aux
// Leaf: ?pop_back@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AEE5, length 20 bytes (unaligned, no padding: the 0x595000.. module is
// built without function alignment). `_M_finish._M_cur` +0x10 against `_M_finish._M_first` +0x14; the pointer element has a
// trivial destructor; else a tail jump to _M_pop_back_aux @ 0x0059AD35 (frees the finish node, steps to the previous one).
// Identity: the COMDAT the NUISystem::CFrontEndManager screen stack (std::stack<NUISystem::CComponent*>) calls from its inline
// stack::pop() (0x00599EFF, NF76 row 0x00599E3F); STLport 4.6 _deque.h pop_back.
// NF79 (round 7, q10967): re-authored as the explicit instantiation of the shared header's deque::pop_back (stlport/fable_deque.h,
//   FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES); was the local stand-in CBlockStack::PopValue with a PopBlock model.
#define FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template void std::deque<NUISystem::CComponent*>::pop_back();
