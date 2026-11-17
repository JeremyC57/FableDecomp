// std::deque::_M_reserve_map_at_back(size_t __nodes_to_add) -- (<NUISystem::CComponent*>) make room in the node map for n more nodes at the back
// Leaf: ?_M_reserve_map_at_back@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@IAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B4D8, 38 bytes, thiscall, `ret 4`.
// Identity (proven): it calls deque<NUISystem::CComponent*>::_M_reallocate_map 0x0059B0C3 (family M070, same TU: 0x80-byte nodes (32 pointers); the same TU's _Deque_base constructor 0x0059AEF9 / _M_initialize_map 0x0059AC9F are deque<NUISystem::CComponent*>'s (CFrontEndManager::m_stackScreens; its users are the landed CFrontEndManager members)); its caller is
//   that deque's _M_push_back_aux_v 0x0059B576.
// Callees: deque::_M_reallocate_map(size_t, bool) 0x0059B0C3.
// Shape (family M071, representative 0x00493FA8): STLport 4.6 _deque.h `_M_reserve_map_at_back` -- not inlined in a size
//   TU (`optimize("s")`): the free map slots behind finish compared with n + 1, `_M_reallocate_map(n, false)`.
// NF86 (round 7, q11103): access-only self respell to the STLport 4.6 protected member (IAE; _deque.h declares _M_reserve_map_at_back in
//   the protected section, as stlport/fable_deque.h does), the spelling the re-authored caller 0x0059B576 (NF84) binds. The local
//   deque / allocator / iterator models are gone: one explicit instantiation on stlport/fable_deque.h (the 0x0059B576 recipe).
//   Descriptive by library (STLport; both PDBs are Dinkumware builds).
#pragma optimize("s", on)
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template void std::deque<NUISystem::CComponent*>::_M_reserve_map_at_back(unsigned int);
