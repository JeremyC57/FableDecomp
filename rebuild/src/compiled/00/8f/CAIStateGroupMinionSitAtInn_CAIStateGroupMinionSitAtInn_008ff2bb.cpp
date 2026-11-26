// CAIStateGroup_MinionSitAtInn::CAIStateGroup_MinionSitAtInn() -- base CAIStateGroup_MinionSitOnSeatBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_MinionSitAtInn@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FF2BB, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128B624 (RTTI .?AVCAIStateGroup_MinionSitAtInn@@, base chain CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_MinionSitAtInn@@QAE@XZ.
// Callees: CAIStateGroup_MinionSitOnSeatBase::CAIStateGroup_MinionSitOnSeatBase 0x008FF247 (??0CAIStateGroup_MinionSitOnSeatBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_MinionSitOnSeatBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_008ff247@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
// NF471 (T3 nf303 0x008FD288, the r469a declaration-only form): retail's ~CAIStateGroup_UsesSeat is the implicit one, kept once at 0x008FD544
//   (both PDBs flag it compiler-generated; owner 0x008FF22B on engine/CAIStateGroup_UsesSeat.h). This local model's implicit copy was not retail's (the
//   19-byte ~CAIStateGroupBase shape: the model gives PSeat no destructor), so the destructor is declared here and the object
//   references 0x008FD544's ??1CAIStateGroup_UsesSeat@@UAE@XZ; the row's own bytes are unchanged.
// NF472 (0x008FD56A self respell, the r469a declaration-only form): retail's ~CAIStateGroup_MinionSitOnSeatBase is the implicit one, kept once at
//   0x008FD56A (both PDBs flag it compiler-generated; owner 0x008FF271 on engine/CAIStateGroup_MinionSitOnSeatBase.h). This local model's implicit copy was
//   not retail's (a lone jmp ~CAIStateGroup_UsesSeat: the model gives POwner +0x34 no destructor), so the destructor is declared here and the object
//   references 0x008FD56A's ??1CAIStateGroup_MinionSitOnSeatBase@@UAE@XZ; the row's own bytes are unchanged.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {                   // RTTI CAIStateGroup_UsesSeat : CAIStateGroupBase
public:                                 // ctor FableWin.pdb only (??0CAIStateGroup_UsesSeat@@IAE@XZ): inlined into this row, which calls the base ctor directly
    virtual ~CAIStateGroup_UsesSeat();                  // ??1CAIStateGroup_UsesSeat@@UAE@XZ 0x008FD544 (implicit in retail; declared here only)
};

class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat {                   // RTTI CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat
public:
    CAIStateGroup_MinionSitOnSeatBase();                     // 0x008FF247 ??0CAIStateGroup_MinionSitOnSeatBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    virtual ~CAIStateGroup_MinionSitOnSeatBase();           // ??1CAIStateGroup_MinionSitOnSeatBase@@UAE@XZ 0x008FD56A (implicit in retail; declared here only)
};

class CAIStateGroup_MinionSitAtInn : public CAIStateGroup_MinionSitOnSeatBase {      // vtable 0x0128B624
public:
    CAIStateGroup_MinionSitAtInn();
};

CAIStateGroup_MinionSitAtInn::CAIStateGroup_MinionSitAtInn()
{
}
