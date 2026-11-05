// Leaf: ?GetStateGroupType@CAIStateGroup_StealthSeekEnemy_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_StealthSeekEnemy.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_StealthSeekEnemy_Methods : CAIStateGroup_StealthSeekEnemy {
    int GetStateGroupType();
};
int CAIStateGroup_StealthSeekEnemy_Methods::GetStateGroupType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetStateGroupType@CAIStateGroup_StealthSeekEnemy@@UBE?AW4EAIStateGroupType@@XZ); evidence: RTTI vtable slot + PDB name set.
