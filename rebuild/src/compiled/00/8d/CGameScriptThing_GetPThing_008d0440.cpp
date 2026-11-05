// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CThing;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CThingRef {
};

class CGameScriptThing { public:
    char pad[0xc];
    CThingRef ref;
};

CThing* __fastcall CGameScriptThing_GetPThing(const CGameScriptThing* self)
{
    CThingRef* r = (CThingRef*)&self->ref;
    if (((CThing*)((CBaseIntelligentPointer*)r)->GetPItem()) != 0) {
        if ((*((unsigned char*)((CThing*)((CBaseIntelligentPointer*)r)->GetPItem()) + 0x91) & 1) == 0)
            return ((CThing*)((CBaseIntelligentPointer*)r)->GetPItem());
    }
    return 0;
}