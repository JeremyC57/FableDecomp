// std::vector<CIntelligentPointer<CThing> >::reserve(unsigned int n) -- STLport reserve for a vector of intelligent thing pointers: reallocate to n slots when the capacity is smaller, copying the elements over and destroying the old block
// Leaf: ?reserve@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E580, 151 bytes, thiscall `ret 4`.
// Callees: malloc @ 0x00BFEA0E (inline allocator, both arms), __uninitialized_copy(first, last, result, const
//   __false_type&) @ 0x004AA140 (`__fastcall`: the CIntelligentPointer copy loop -- vtable 0x01238C6C, GetPItem/SetPItem),
//   _M_clear() @ 0x004AC1D0 (virtual destructor of each 8-byte element, then free).
// Callers: CTCBurntEffect::CTCBurntEffect 0x007CDFE0 (landed: names this address `vector<CIntelligentPointer<CThing> >
//   ::reserve` for TintList), 0x0077D5B0, 0x007CEB.., 0x007CF1.., 0x008E8060.
// Label correction: catalogued as `vector_reserve_CConversation`; the element is CIntelligentPointer<CThing>.
// Shape: byte twin (immediates masked) of the landed vector<pair<CCharString,long> >::reserve 0x006D4680: STLport 4.x
//   reserve with the member-template _M_allocate_and_copy inlined (allocate + the out-of-line copy), the old size
//   computed first (ebp), the cached finish spilled to the `push ecx` frame word; the `__false_type` tag has a user
//   constructor and the copy loop has a same-TU noinline body, so VC7.1 homes the tag over the live `n` argument slot.
// Name fix (NF56, round 7, queue q10457): Ego_r.pdb 0x004B3300 + FableWin.pdb 0x01B59550
//   ?reserve@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXI@Z. The local
//   std::vector / allocator / __uninitialized_copy models are replaced by the shared stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR
//   (+ _C for capacity / _M_allocate_and_copy), FABLE_RETAIL_STLP_VECTOR_RESERVE with the overlay opt-in
//   FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY (STLport's own reserve body), FABLE_STLP_OUTOFLINE_UNINIT_COPY (the __fastcall
//   noinline copy 0x004AA140), and the retail CIntelligentPointer copy (engine/CIntelligentPointer.h).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
class CThing;
#include "engine/CIntelligentPointer.h"

template void std::vector<CIntelligentPointer<CThing> >::reserve(size_t n);
