// std::deque<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const& t) -- store at the finish cursor when its buffer has room,
// else go through _M_push_back_aux_v (0x0059B576)
// Leaf: ?push_back@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B61C, 37 bytes, a COMDAT in a size-optimised TU.
// Identity (NF83, round 7, q11026 self): Ego_r.pdb 0x006077B8 / FableWin.pdb 0x020203DF
//   ?push_back@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAEXABQAVCComponent@NUISystem@@@Z;
//   the landed 0x00596763 (NF79) binds it so. Was a local deque model whose leaf lacked the allocator argument.
// Port (NF83): one explicit instantiation of the STLport 4.6 push_back body in stlport/fable_deque.h (FABLE_STLPORT_DEQUE_PUSH_BACK;
//   _M_push_back_aux_v, protected, declared only: retail calls 0x0059B576).
#pragma optimize("s", on)
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template void std::deque<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const& t);
