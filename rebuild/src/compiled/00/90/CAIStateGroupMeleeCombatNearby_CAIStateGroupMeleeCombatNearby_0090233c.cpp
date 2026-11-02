// CAIStateGroup_MeleeCombatNearby::CAIStateGroup_MeleeCombatNearby() -- base CAIStateGroup_MeleeCombatNearbyBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_MeleeCombatNearby@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090233C, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D05C (RTTI .?AVCAIStateGroup_MeleeCombatNearby@@, base chain CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_MeleeCombatNearby@@QAE@XZ.
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

class CAIStateGroup_MeleeCombatNearby : public CAIStateGroup_MeleeCombatNearbyBase {      // vtable 0x0128D05C
public:
    CAIStateGroup_MeleeCombatNearby();
};

CAIStateGroup_MeleeCombatNearby::CAIStateGroup_MeleeCombatNearby()
{
}
