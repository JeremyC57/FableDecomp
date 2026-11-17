// std::deque<NUISystem::CComponent*>::_M_pop_back_aux() -- pop_back across a buffer boundary: free the (empty) finish buffer, step the
// finish iterator to the previous node and point it at that buffer's last element
// Leaf: ?_M_pop_back_aux@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AD35, 47 bytes, a COMDAT in a size-optimised TU (free 0x00BFEA14).
// Identity (NF83, round 7, q11024 self): STLport 4.6 _deque.c deque::_M_pop_back_aux (protected); the landed pop_back 0x0059AEE5
//   (NF79) binds this address under this spelling; the deque carries the default allocator argument as both PDBs'
//   deque<NUISystem::CComponent*> members do. Descriptive-by-library (STLport; both PDBs are Dinkumware builds). Was the stand-in
//   CRetailDeque_0059ad35::PopBackAux over a local deque / iterator model.
// Port (NF83): one explicit instantiation of the stlport/fable_deque.h body (NF83 overlay opt-in FABLE_RETAIL_STLP_DEQUE_POP_BACK_AUX_BODY
//   with NF79's FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES
#define FABLE_RETAIL_STLP_DEQUE_POP_BACK_AUX_BODY
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template void std::deque<NUISystem::CComponent*>::_M_pop_back_aux();
