// std::vector<CThingAICreature*>::push_back(CThingAICreature* const& x) -- STLport 4.6 push_back kept out of line: construct x at
// _M_finish when there is room, else the POD _M_insert_overflow(_M_finish, x, __true_type(), 1, true) (0x00515CC0)
// Leaf: ?push_back@?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@QAEXABQAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00517A10, 50 bytes, thiscall, `ret 4`.
// Naming (NF108, 2026-11-19, q4434): was the stand-in CRetailVector_dword::PushBack_dword_00517a10. Its growth callee 0x00515CC0 is
//   vector<CThingAICreature*>::_M_insert_overflow (landed binders 0x007B98C0 / 0x006186F0); CThingManager 0x004FF750 calls it three
//   times on the males / females vectors and binds this spelling (RELOCATION_MATCH). STLport 4.6 _vector.h body; FableWin.pdb 0x01CA8C70
//   spells the same instantiation (its Dinkumware copy). Explicit instantiation over stlport/fable_containers.h, the overflow body
//   visible but out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), as for 0x00436C80.
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "stlport/fable_containers.h"
class CThingAICreature;
template void std::vector<CThingAICreature*>::push_back(CThingAICreature* const&);
