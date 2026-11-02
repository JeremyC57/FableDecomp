// CAIStateGroup_LightWorkCandle::CAIStateGroup_LightWorkCandle() -- base CAIStateGroup_LightHomeCandle(), then this class's own vtable
// Leaf: ??0CAIStateGroup_LightWorkCandle@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090004B, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128BE04 (RTTI .?AVCAIStateGroup_LightWorkCandle@@, base chain CAIStateGroup_LightHomeCandle : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_LightWorkCandle@@QAE@XZ.
// Callees: CAIStateGroup_LightHomeCandle::CAIStateGroup_LightHomeCandle 0x00900008 (??0CAIStateGroup_LightHomeCandle@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_LightHomeCandle vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBehaviourBase@N_00900008@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_LightHomeCandle : public CAIStateGroupBase {                   // RTTI CAIStateGroup_LightHomeCandle : CAIStateGroupBase
public:
    CAIStateGroup_LightHomeCandle();                     // 0x00900008 ??0CAIStateGroup_LightHomeCandle@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_LightWorkCandle : public CAIStateGroup_LightHomeCandle {      // vtable 0x0128BE04
public:
    CAIStateGroup_LightWorkCandle();
};

CAIStateGroup_LightWorkCandle::CAIStateGroup_LightWorkCandle()
{
}
