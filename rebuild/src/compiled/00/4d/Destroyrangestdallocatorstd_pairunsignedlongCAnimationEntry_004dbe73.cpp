#pragma optimize("s",on)
// Leaf: ?_Destroy_range_pair@@YIXPAUCAnimEntryPair@@0PAUAllocDummy@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// _Destroy_range<std::allocator<std::pair<unsigned_long,CAnimationEntry>>>
// retail 004dbe73, __fastcall(first, last, allocator&)
// loop over [first,last) stride 0xc calling element dtor via thiscall.

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CAnimEntryPair { unsigned long k; unsigned long a; unsigned long b; }; // 12 bytes

struct AllocDummy {};

void __fastcall _Destroy_range_pair(CAnimEntryPair* first, CAnimEntryPair* last, AllocDummy* al)
{
    (void)al;
    while (first != last)
    {
        ((CBaseIntelligentPointer*)(first))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
        ++first;
    }
}