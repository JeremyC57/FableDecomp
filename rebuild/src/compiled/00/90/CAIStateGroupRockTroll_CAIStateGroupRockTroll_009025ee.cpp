// CAIStateGroup_RockTroll::CAIStateGroup_RockTroll() -- base CAIStateGroup_TrollBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_RockTroll@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009025EE, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D0A0 (RTTI .?AVCAIStateGroup_RockTroll@@, base chain CAIStateGroup_TrollBase : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_RockTroll@@QAE@XZ.
// Callees: CAIStateGroup_TrollBase::CAIStateGroup_TrollBase 0x0090035D (??0CAIStateGroup_TrollBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_TrollBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_0090035d@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_TrollBase : public CAIStateGroupBase {                   // RTTI CAIStateGroup_TrollBase : CAIStateGroupBase
public:
    CAIStateGroup_TrollBase();                     // 0x0090035D ??0CAIStateGroup_TrollBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_RockTroll : public CAIStateGroup_TrollBase {      // vtable 0x0128D0A0
public:
    CAIStateGroup_RockTroll();
};

CAIStateGroup_RockTroll::CAIStateGroup_RockTroll()
{
}
