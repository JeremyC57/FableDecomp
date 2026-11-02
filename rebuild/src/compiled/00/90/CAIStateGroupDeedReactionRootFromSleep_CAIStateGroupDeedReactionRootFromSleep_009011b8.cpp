// CAIStateGroup_DeedReactionRootFromSleep::CAIStateGroup_DeedReactionRootFromSleep() -- base CAIStateGroup_DeedReactionRoot(), then this class's own vtable
// Leaf: ??0CAIStateGroup_DeedReactionRootFromSleep@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009011B8, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128C9FC (RTTI .?AVCAIStateGroup_DeedReactionRootFromSleep@@, base chain CAIStateGroup_DeedReactionRoot : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_DeedReactionRootFromSleep@@QAE@XZ.
// Callees: CAIStateGroup_DeedReactionRoot::CAIStateGroup_DeedReactionRoot 0x00901178 (??0CAIStateGroup_DeedReactionRoot@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_DeedReactionRoot vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_00901178@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_DeedReactionRoot : public CAIStateGroupBase {                   // RTTI CAIStateGroup_DeedReactionRoot : CAIStateGroupBase
public:
    CAIStateGroup_DeedReactionRoot();                     // 0x00901178 ??0CAIStateGroup_DeedReactionRoot@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_DeedReactionRootFromSleep : public CAIStateGroup_DeedReactionRoot {      // vtable 0x0128C9FC
public:
    CAIStateGroup_DeedReactionRootFromSleep();
};

CAIStateGroup_DeedReactionRootFromSleep::CAIStateGroup_DeedReactionRootFromSleep()
{
}
