// Leaf: ?PerformAction@CCreatureAction_PickCrop@@UAEXXZ
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `sub` -> `PObjectToUse`.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Header route (NF319, 2026-11-23): defined on the real class of engine/CCreatureAction_PickCrop.h (was ?PerformAction@CCreatureAction_PickCrop_Methods@@QAEXXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address;
//   declared there under FABLE_RETAIL_CCREATUREACTION_PICKCROP (switches landed rows define). Bytes unchanged.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PICKCROP
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureAction_PickCrop.h"
class CBaseObject;

struct CPickCropActionState {
};


void CCreatureAction_PickCrop::PerformAction() {
    CPickCropActionState* state = (CPickCropActionState*)&this->PObjectToUse;
    if (((CBaseIntelligentPointer*)state)->GetPItem()) {
        ((CBaseIntelligentPointer*)state)->GetPItem();
    }
}
