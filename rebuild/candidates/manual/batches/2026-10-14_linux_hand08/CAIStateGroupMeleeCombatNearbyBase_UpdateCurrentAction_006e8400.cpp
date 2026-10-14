// CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(CThingCreatureBase*) -- the static form of the
// creature-finish helper: finish the given creature's action once it is free to do so.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8400, length 38 bytes, `ret 4` with ecx unused.
// Callees on the argument: IsPerformingAnyAction @ 0x006622A0, IsFreeToFinishAction @ 0x006621C0,
// FinishCurrentAction @ 0x00662650. The manifest module label is propagated from the 0x0093D140 body
// (same logic on PBrain->Creature); this copy takes the creature as its only argument and pops it itself,
// so it is a static member compiled __stdcall (no sibling jump: the callee cannot pop the caller's slot).
struct CThingCreatureBase {
    bool IsPerformingAnyAction();           // 0x006622A0
    bool IsFreeToFinishAction();            // 0x006621C0
    void FinishCurrentAction();             // 0x00662650
};

struct CAIStateGroup_MeleeCombatNearbyBase {
    static void __stdcall UpdateCurrentAction(CThingCreatureBase* creature);
};

void __stdcall CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(CThingCreatureBase* creature)
{
    if (creature->IsPerformingAnyAction() && creature->IsFreeToFinishAction())
        creature->FinishCurrentAction();
}
