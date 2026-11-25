// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("s", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
// Naming (NF423 gen, 2026-11-25): the callee 0x007891F8 now binds its PDB spelling ??1CTCTrapBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCProjectileTrap_Destruct@@YIXPAX@Z.
class CTCTrapBase { public: virtual ~CTCTrapBase(); };
void* __fastcall CTCProjectileTrap_CompositeScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(trap) + 108))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    reinterpret_cast<CTCTrapBase*>(trap)->CTCTrapBase::~CTCTrapBase();
    if (flags & 1)
        operator delete(trap);
    return trap;
}