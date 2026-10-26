// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `sub` -> `PObjectToUse`.
#include "engine/CCreatureAction_PickCrop.h"

struct CPickCropActionState {
    int PerformStep();
};

struct CCreatureAction_PickCrop_Methods : CCreatureAction_PickCrop {
    void PerformAction();
};

void CCreatureAction_PickCrop_Methods::PerformAction()
{
    CPickCropActionState* state = (CPickCropActionState*)&this->PObjectToUse;
    if (state->PerformStep()) {
        state->PerformStep();
    }
}
