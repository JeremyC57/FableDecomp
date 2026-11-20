// std::vector<CThing*>::push_back(CThing* const& x) -- STLport 4.6 push_back kept out of line: construct x at _M_finish when there is
// room, else the POD _M_insert_overflow(_M_finish, x, __true_type(), 1, true) (0x00436AC0)
// Leaf: ?push_back@?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@QAEXABQAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00436C80, 50 bytes, thiscall, `ret 4`.
// Naming (NF108, 2026-11-19, q4258): was the stand-in CRetailVector_dword::PushBack_dword_00436c80. The landed REFS rows 0x005C552C,
//   0x0061B610, 0x0061E6D0, 0x00666EF0, 0x007A0D90, 0x008A7A70, 0x008F7950 (shared headers) bind
//   ?push_back@?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@QAEXABQAVCThing@@@Z (vector<CThing*>::push_back);
//   FableWin.pdb 0x01903FE0 spells the same instantiation; body STLport 4.6 _vector.h. Re-authored as an explicit instantiation over stlport/fable_containers.h,
//   the overflow body visible but out of line (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE) as in STLport's _vector.c TU.
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "stlport/fable_containers.h"
class CThing;
template void std::vector<CThing*>::push_back(CThing* const&);
