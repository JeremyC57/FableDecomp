// CAIStateGroup_MinionReactToCrimeFromSleep::CAIStateGroup_MinionReactToCrimeFromSleep() -- base CAIStateGroup_MinionReactToCrime(), then this class's own vtable
// Leaf: ??0CAIStateGroup_MinionReactToCrimeFromSleep@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900B81, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128C594 (RTTI .?AVCAIStateGroup_MinionReactToCrimeFromSleep@@, base chain CAIStateGroup_MinionReactToCrime : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_MinionReactToCrimeFromSleep@@QAE@XZ.
// Callees: CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime 0x00900B41 (??0CAIStateGroup_MinionReactToCrime@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_MinionReactToCrime vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_00900b41@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_MinionReactToCrime : public CAIStateGroupBase {                   // RTTI CAIStateGroup_MinionReactToCrime : CAIStateGroupBase
public:
    CAIStateGroup_MinionReactToCrime();                     // 0x00900B41 ??0CAIStateGroup_MinionReactToCrime@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_MinionReactToCrimeFromSleep : public CAIStateGroup_MinionReactToCrime {      // vtable 0x0128C594
public:
    CAIStateGroup_MinionReactToCrimeFromSleep();
};

CAIStateGroup_MinionReactToCrimeFromSleep::CAIStateGroup_MinionReactToCrimeFromSleep()
{
}
