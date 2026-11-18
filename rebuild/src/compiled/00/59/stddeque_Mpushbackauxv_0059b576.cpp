#pragma optimize("s", on)
// std::deque<NUISystem::CComponent*>::_M_push_back_aux_v(NUISystem::CComponent* const&) -- STLport 4.6 deque push_back slow path:
// reserve a map slot at the back, allocate the next 0x80-byte node, construct the value at the old finish and move finish to the
// new node
// Leaf: ?_M_push_back_aux_v@?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@IAEXABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B576, length 76 bytes. Size TU (`pop ecx` after the allocation call).
// Callees: deque::_M_reserve_map_at_back(size_type) @ 0x0059B4D8, malloc @ 0x00BFEA0E (the node allocator, 0x80 bytes = 32 elements).
// Identity (NF84, round 7, q11046): the slow path of the explicit deque<NUISystem::CComponent*>::push_back 0x0059B61C (NF83), which
//   binds this address as the protected allocator-form STLport member (STLport 4.6 _deque.h / _deque.c); the deque is
//   NUISystem::CFrontEndManager's screen history (stack<CComponent*>, 0x00599E3F / 0x00596763). Descriptive by library (STLport;
//   both PDBs are Dinkumware builds). Was the stand-in CDwordDeque::_M_push_back_aux_v (a local 4-byte-element deque model).
// Port (NF84): the shared stlport/fable_deque.h body (FABLE_STLPORT_DEQUE_PUSH_BACK + _AUX_NOINLINE, the 0x00912840 deque<CBezierCurve>
//   opt-ins), explicitly instantiated.
#define FABLE_STLPORT_DEQUE_PUSH_BACK
#define FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template void std::deque<NUISystem::CComponent*>::_M_push_back_aux_v(NUISystem::CComponent* const& t);
