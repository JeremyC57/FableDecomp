// Leaf: ?OnAnimationLoaded@CCreatureAction_GetUpFromSitting@@QAEXXZ
#include "rebuild_abi.h"
// OnAnimationLoaded @ 0x008b6530  : [this+0x14] = [this+0x10] / 2 (signed) after helper()
struct CCreatureAction_GetUpFromSitting { char pad[0x10]; int m_a; int m_b; void OnAnimationLoaded(); };
extern "C" void FABLE_FASTCALL FableBase_8b6530(CCreatureAction_GetUpFromSitting *action);
void CCreatureAction_GetUpFromSitting::OnAnimationLoaded(){
    FableBase_8b6530(this);
    m_b = m_a / 2;
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnAnimationLoaded@CCreatureAction_GetUpFromSitting@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
