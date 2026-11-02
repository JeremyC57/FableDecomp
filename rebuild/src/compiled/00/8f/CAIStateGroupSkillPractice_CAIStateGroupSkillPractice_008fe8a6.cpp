// CAIStateGroup_SkillPractice::CAIStateGroup_SkillPractice() -- base CAIStateGroup_ApprenticeHero(), then this class's own vtable
// Leaf: ??0CAIStateGroup_SkillPractice@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE8A6, 18 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128AF1C (RTTI .?AVCAIStateGroup_SkillPractice@@, base chain CAIStateGroup_ApprenticeHero : CAIStateGroupBase : CBaseClass); FableWin.pdb ??0CAIStateGroup_SkillPractice@@QAE@XZ.
// Callees: CAIStateGroup_ApprenticeHero::CAIStateGroup_ApprenticeHero 0x008FE862 (??0CAIStateGroup_ApprenticeHero@@QAE@XZ, Ego_r.pdb / FableWin.pdb; the callee stores the CAIStateGroup_ApprenticeHero vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base ??0CAIStateGroupBase@N_008fe862@@QAE@XZ;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_ApprenticeHero : public CAIStateGroupBase {                   // RTTI CAIStateGroup_ApprenticeHero : CAIStateGroupBase
public:
    CAIStateGroup_ApprenticeHero();                     // 0x008FE862 ??0CAIStateGroup_ApprenticeHero@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
};

class CAIStateGroup_SkillPractice : public CAIStateGroup_ApprenticeHero {      // vtable 0x0128AF1C
public:
    CAIStateGroup_SkillPractice();
};

CAIStateGroup_SkillPractice::CAIStateGroup_SkillPractice()
{
}
