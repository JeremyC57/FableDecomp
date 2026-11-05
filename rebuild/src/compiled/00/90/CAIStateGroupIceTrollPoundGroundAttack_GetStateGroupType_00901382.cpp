// Leaf: ?GetStateGroupType@CAIStateGroup_IceTrollPoundGroundAttack_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_IceTrollPoundGroundAttack.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_IceTrollPoundGroundAttack_Methods : CAIStateGroup_IceTrollPoundGroundAttack {
    int GetStateGroupType();
};
int CAIStateGroup_IceTrollPoundGroundAttack_Methods::GetStateGroupType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetStateGroupType@CAIStateGroup_IceTrollPoundGroundAttack@@MBE?AW4EAIStateGroupType@@XZ); evidence: RTTI vtable slot + PDB name set.
