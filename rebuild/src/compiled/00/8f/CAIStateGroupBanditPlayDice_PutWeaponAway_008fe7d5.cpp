// Constant-return member: returns true. __fastcall this=ecx.
// Leaf: ?PutWeaponAway@CAIStateGroup_BanditPlayDice_Methods@@QAE_NXZ
#include "engine/CAIStateGroup_BanditPlayDice.h"  // retyped onto the PDB layout; byte parity re-verified
struct CAIStateGroup_BanditPlayDice_Methods : CAIStateGroup_BanditPlayDice {
    bool PutWeaponAway();
};
bool CAIStateGroup_BanditPlayDice_Methods::PutWeaponAway() { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PutWeaponAway@CAIStateGroup_BanditPlayDice@@MAE_NXZ); evidence: RTTI vtable slot + PDB name set.
