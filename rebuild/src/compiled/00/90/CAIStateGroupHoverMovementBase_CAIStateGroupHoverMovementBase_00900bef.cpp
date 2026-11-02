// CAIStateGroup_HoverMovementBase::CAIStateGroup_HoverMovementBase() -- base CAIStateGroup_EnemyCreatureBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_HoverMovementBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900BEF, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128A1DC (RTTI .?AVCAIStateGroup_HoverMovementBase@@, base chain CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_HoverMovementBase@@QAE@XZ.
// Callees: CAIStateGroup_EnemyCreatureBase::CAIStateGroup_EnemyCreatureBase 0x008FF720 (??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_EnemyCreatureBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_008ff720@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {                   // RTTI CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase
public:
    CAIStateGroup_EnemyCreatureBase();                     // 0x008FF720 ??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {      // vtable 0x0128A1DC
public:
    CAIStateGroup_HoverMovementBase();
};

CAIStateGroup_HoverMovementBase::CAIStateGroup_HoverMovementBase()
{
}
