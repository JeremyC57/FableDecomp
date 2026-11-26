// CAIStateGroup_HoverKeepInArea::CAIStateGroup_HoverKeepInArea() -- base CAIStateGroup_EnemyCreatureBase(), then this class's own vtable
// Leaf: ??0CAIStateGroup_HoverKeepInArea@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900CF1, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128C6F4 (RTTI .?AVCAIStateGroup_HoverKeepInArea@@, base chain CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_HoverKeepInArea@@QAE@XZ.
// Callees: CAIStateGroup_EnemyCreatureBase::CAIStateGroup_EnemyCreatureBase 0x008FF720 (??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_EnemyCreatureBase vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise; the ctor of CAIStateGroup_HoverMovementBase is inlined (FableWin.pdb-only symbol), so the call goes to CAIStateGroup_EnemyCreatureBase.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_008ff720@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
// NF471 (T3 nf303 0x008FD288, the r469a declaration-only form): retail's ~CAIStateGroup_EnemyCreatureBase is the implicit one, kept once at 0x008FD769
//   (both PDBs flag it compiler-generated; owner 0x008FF744 on engine/CAIStateGroup_EnemyCreatureBase.h). This local model's implicit copy was not retail's (the
//   19-byte ~CAIStateGroupBase shape: the model gives PTargetThing no destructor), so the destructor is declared here and the object
//   references 0x008FD769's ??1CAIStateGroup_EnemyCreatureBase@@UAE@XZ; the row's own bytes are unchanged.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {                   // RTTI CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase
public:
    virtual ~CAIStateGroup_EnemyCreatureBase();         // ??1CAIStateGroup_EnemyCreatureBase@@UAE@XZ 0x008FD769 (implicit in retail; declared here only)
    CAIStateGroup_EnemyCreatureBase();                     // 0x008FF720 ??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase {                   // RTTI CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase
public:                                 // ctor FableWin.pdb only (??0CAIStateGroup_HoverMovementBase@@QAE@XZ): inlined into this row, which calls the base ctor directly
};

class CAIStateGroup_HoverKeepInArea : public CAIStateGroup_HoverMovementBase {      // vtable 0x0128C6F4
public:
    CAIStateGroup_HoverKeepInArea();
};

CAIStateGroup_HoverKeepInArea::CAIStateGroup_HoverKeepInArea()
{
}
