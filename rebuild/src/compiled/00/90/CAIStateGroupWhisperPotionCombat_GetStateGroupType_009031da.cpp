// Leaf: ?GetStateGroupType@CAIStateGroup_WhisperPotionCombat_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CAIStateGroup_WhisperPotionCombat.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_WhisperPotionCombat_Methods : CAIStateGroup_WhisperPotionCombat {
    int GetStateGroupType();
};
int CAIStateGroup_WhisperPotionCombat_Methods::GetStateGroupType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetStateGroupType@CAIStateGroup_WhisperPotionCombat@@UBE?AW4EAIStateGroupType@@XZ); evidence: RTTI vtable slot + PDB name set.
