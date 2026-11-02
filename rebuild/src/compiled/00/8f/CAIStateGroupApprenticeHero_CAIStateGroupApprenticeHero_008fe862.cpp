// CAIStateGroup_ApprenticeHero::CAIStateGroup_ApprenticeHero() -- base CAIStateGroupBase(), own vtable, then the Interaction member (+0x24)
// Leaf: ??0CAIStateGroup_ApprenticeHero@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE862, 26 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128AEC4 (RTTI .?AVCAIStateGroup_ApprenticeHero@@, base CAIStateGroupBase : CBaseClass); the CCharString member is built out of line; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_ApprenticeHero@@QAE@XZ.
// Callees: CAIStateGroupBase::CAIStateGroupBase 0x008FD255, CCharString::CCharString 0x0099E4B0 (??0CCharString@@QAE@XZ).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_ApprenticeHero::Interaction +0x24; the members before it
//   are left unmodelled.
// Shape: user default ctor, empty; the CCharString member's ctor is called out of line after the own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008fd255 / CMember_0099e4b0 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255


class CAIStateGroup_ApprenticeHero : public CAIStateGroupBase {      // vtable 0x0128AEC4
public:
    unsigned char _unmodeled14[0x24 - 0x14];       // Ego_r.pdb: NextFramePractice +0x14, PDummy +0x18, Action +0x1c, PracticeOpponent +0x20
    CCharString Interaction;                     // +0x24
    CAIStateGroup_ApprenticeHero();
};

CAIStateGroup_ApprenticeHero::CAIStateGroup_ApprenticeHero()
{
}
