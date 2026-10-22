#include "engine/CCreatureAction_ForcePushOutOf.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCreatureActionForcePushVTable {
    void (__fastcall *Slots[4])(CCreatureAction_ForcePushOutOf*);
};

extern bool __fastcall CreatureForcePush_Check(CCreatureAction_ForcePushOutOf* action);
extern void __fastcall CreatureAction_FinishAction_Base(CCreatureAction_ForcePushOutOf* action);

void __fastcall CCreatureAction_ForcePushOutOf_FinishAction(
    CCreatureAction_ForcePushOutOf* action)
{
    if (!CreatureForcePush_Check(action)) {
        ((CCreatureActionForcePushVTable*)action->__vftable)->Slots[3](action);
    }
    CreatureAction_FinishAction_Base(action);
}
