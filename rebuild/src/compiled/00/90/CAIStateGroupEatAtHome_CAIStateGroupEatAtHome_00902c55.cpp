// CAIStateGroup_EatAtHome::CAIStateGroup_EatAtHome() -- base CAIStateGroup_EatAtTable(), then this class's own vtable
// Leaf: ??0CAIStateGroup_EatAtHome@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00902C55, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D25C (RTTI .?AVCAIStateGroup_EatAtHome@@, base chain CAIStateGroup_EatAtTable : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_EatAtHome@@QAE@XZ.
// Callees: CAIStateGroup_EatAtTable::CAIStateGroup_EatAtTable 0x009019AA (??0CAIStateGroup_EatAtTable@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_EatAtTable vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_009019aa@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_EatAtTable : public CAIStateGroupBase {                   // RTTI CAIStateGroup_EatAtTable : CAIStateGroupBase
public:
    CAIStateGroup_EatAtTable();                     // 0x009019AA ??0CAIStateGroup_EatAtTable@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_EatAtHome : public CAIStateGroup_EatAtTable {      // vtable 0x0128D25C
public:
    CAIStateGroup_EatAtHome();
};

CAIStateGroup_EatAtHome::CAIStateGroup_EatAtHome()
{
}
