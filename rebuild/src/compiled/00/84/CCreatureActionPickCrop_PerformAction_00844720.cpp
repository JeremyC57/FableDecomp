// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `sub` -> `PObjectToUse`.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_PickCrop.h"

struct CPickCropActionState {
};

struct CCreatureAction_PickCrop_Methods : CCreatureAction_PickCrop {
    void PerformAction();
};

void CCreatureAction_PickCrop_Methods::PerformAction()
{
    CPickCropActionState* state = (CPickCropActionState*)&this->PObjectToUse;
    if (((CBaseIntelligentPointer*)state)->GetPItem()) {
        ((CBaseIntelligentPointer*)state)->GetPItem();
    }
}
