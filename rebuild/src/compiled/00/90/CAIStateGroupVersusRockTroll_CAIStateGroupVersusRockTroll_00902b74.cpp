// CAIStateGroup_VersusRockTroll::CAIStateGroup_VersusRockTroll() -- base CAIStateGroup_VersusEnemyBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_VersusRockTroll@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00902B74, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D21C (RTTI .?AVCAIStateGroup_VersusRockTroll@@, base chain CAIStateGroup_VersusEnemyBase : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_VersusRockTroll@@QAE@XZ.
// Callees: CAIStateGroup_VersusEnemyBase::CAIStateGroup_VersusEnemyBase 0x009013FA (??0CAIStateGroup_VersusEnemyBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_VersusEnemyBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_009013fa@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_VersusEnemyBase : public CAIStateGroupBase {                   // RTTI CAIStateGroup_VersusEnemyBase : CAIStateGroupBase
public:
    CAIStateGroup_VersusEnemyBase();                     // 0x009013FA ??0CAIStateGroup_VersusEnemyBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_VersusRockTroll : public CAIStateGroup_VersusEnemyBase {      // vtable 0x0128D21C
public:
    CAIStateGroup_VersusRockTroll();
};

CAIStateGroup_VersusRockTroll::CAIStateGroup_VersusRockTroll()
{
}
