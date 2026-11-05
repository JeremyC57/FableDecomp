// Leaf: ?GetStateGroupType@CAIStateGroup_MazeShieldedChase_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_MazeShieldedChase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_MazeShieldedChase_Methods : CAIStateGroup_MazeShieldedChase {
    int GetStateGroupType();
};
int CAIStateGroup_MazeShieldedChase_Methods::GetStateGroupType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetStateGroupType@CAIStateGroup_MazeShieldedChase@@UBE?AW4EAIStateGroupType@@XZ); evidence: RTTI vtable slot + PDB name set.
