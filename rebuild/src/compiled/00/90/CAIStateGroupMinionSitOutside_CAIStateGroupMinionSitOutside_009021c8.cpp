// CAIStateGroup_MinionSitOutside::CAIStateGroup_MinionSitOutside() -- base CAIStateGroup_MinionSitOnSeatBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_MinionSitOutside@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009021C8, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D00C (RTTI .?AVCAIStateGroup_MinionSitOutside@@, base chain CAIStateGroup_MinionSitOnSeat : CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_MinionSitOutside@@QAE@XZ.
// Callees: CAIStateGroup_MinionSitOnSeatBase::CAIStateGroup_MinionSitOnSeatBase 0x008FF247 (??0CAIStateGroup_MinionSitOnSeatBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_MinionSitOnSeatBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise; the ctor of CAIStateGroup_MinionSitOnSeat is inlined (FableWin.pdb-only symbol), so the call goes to CAIStateGroup_MinionSitOnSeatBase.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_008ff247@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {                   // RTTI CAIStateGroup_UsesSeat : CAIStateGroupBase
public:                                 // ctor FableWin.pdb only (??0CAIStateGroup_UsesSeat@@IAE@XZ): inlined into this row, which calls the base ctor directly
};

class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat {                   // RTTI CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat
public:
    CAIStateGroup_MinionSitOnSeatBase();                     // 0x008FF247 ??0CAIStateGroup_MinionSitOnSeatBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_MinionSitOnSeat : public CAIStateGroup_MinionSitOnSeatBase {                   // RTTI CAIStateGroup_MinionSitOnSeat : CAIStateGroup_MinionSitOnSeatBase
public:                                 // ctor FableWin.pdb only (??0CAIStateGroup_MinionSitOnSeat@@QAE@XZ): inlined into this row, which calls the base ctor directly
};

class CAIStateGroup_MinionSitOutside : public CAIStateGroup_MinionSitOnSeat {      // vtable 0x0128D00C
public:
    CAIStateGroup_MinionSitOutside();
};

CAIStateGroup_MinionSitOutside::CAIStateGroup_MinionSitOutside()
{
}
