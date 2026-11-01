// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `sub` -> `PObjectToUse`.
class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
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
