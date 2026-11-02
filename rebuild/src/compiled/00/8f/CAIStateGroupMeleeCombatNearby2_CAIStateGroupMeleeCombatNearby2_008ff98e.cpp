// CAIStateGroup_MeleeCombatNearby2::CAIStateGroup_MeleeCombatNearby2() -- base CAIStateGroup_MeleeCombatNearbyBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_MeleeCombatNearby2@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FF98E, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128B99C (RTTI .?AVCAIStateGroup_MeleeCombatNearby2@@, base chain CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_MeleeCombatNearby2@@QAE@XZ.
// Callees: CAIStateGroup_MeleeCombatNearbyBase::CAIStateGroup_MeleeCombatNearbyBase 0x008FF94E (??0CAIStateGroup_MeleeCombatNearbyBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_MeleeCombatNearbyBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_008ff94e@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_MeleeCombatNearbyBase : public CAIStateGroupBase {                   // RTTI CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase
public:
    CAIStateGroup_MeleeCombatNearbyBase();                     // 0x008FF94E ??0CAIStateGroup_MeleeCombatNearbyBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_MeleeCombatNearby2 : public CAIStateGroup_MeleeCombatNearbyBase {      // vtable 0x0128B99C
public:
    CAIStateGroup_MeleeCombatNearby2();
};

CAIStateGroup_MeleeCombatNearby2::CAIStateGroup_MeleeCombatNearby2()
{
}
