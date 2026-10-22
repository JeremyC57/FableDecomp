#pragma optimize("y",on)
#include "engine/CCreatureAction_GetOutOfBed.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCreatureActionGetOutOfBedVTable {
    void (__fastcall *Slots[4])(void*);
};

extern void __fastcall CCreatureAction_Base_FinishAction(
    CCreatureAction_GetOutOfBed* action);

void __fastcall FinishAction(CCreatureAction_GetOutOfBed* action)
{
    if (!action->interrupted)
        ((CCreatureActionGetOutOfBedVTable*)action->__vftable)->Slots[3](action);
    CCreatureAction_Base_FinishAction(action);
}
