// Byte-exact reconstruction of operator==(CIntelligentPointer<CThingBuilding>&, CIntelligentPointer<CThingBuilding>&) @ 0x0065d673
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingBuilding;

struct CIntelligentPointer_CThingBuilding_ {
    CThingBuilding* Get();
};

// retail 0xa01b50 = CIntelligentPointer<CThingBuilding>::Get, this-in-ecx.
// modeled as __fastcall free fn (self in ecx). extern so a real call is emitted.

bool __fastcall operator_eq_CThingBuilding_(CIntelligentPointer_CThingBuilding_* lhs,
                                            CIntelligentPointer_CThingBuilding_* rhs)
{
    return ((CThingBuilding*)(((CBaseIntelligentPointer*)(lhs))->GetPItem())) == ((CThingBuilding*)(((CBaseIntelligentPointer*)(rhs))->GetPItem()));
}