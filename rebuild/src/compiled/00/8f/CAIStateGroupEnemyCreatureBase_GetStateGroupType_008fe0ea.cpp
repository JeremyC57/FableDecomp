// Leaf: ?GetStateGroupType@CAIStateGroup_EnemyCreatureBase_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_EnemyCreatureBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_EnemyCreatureBase_Methods : CAIStateGroup_EnemyCreatureBase {
    int GetStateGroupType();
};
int CAIStateGroup_EnemyCreatureBase_Methods::GetStateGroupType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetStateGroupType@CAIStateGroup_EnemyCreatureBase@@UBE?AW4EAIStateGroupType@@XZ); evidence: RTTI vtable slot + PDB name set.
