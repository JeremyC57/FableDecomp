// CTCEffectOnDie::CTCEffectOnDie(CThing* parent) -- CTCBase(parent), own vtable, cleared flags and timers, empty DeathSoundCriteria, Pausable = true
// Leaf: ??0CTCEffectOnDie@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCEffectOnDie@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0B50, length 56 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800; CCharString::CCharString() @ 0x0099E4B0.
// Vtable 0x0126FEB4 = CTCEffectOnDie (rtti-vtables.tsv).
// PDB CTCEffectOnDie (0x24 bytes): StartedNonRecoverableFadeout +0x0c, Effect +0x10, Timer +0x14,
// FadeOutTimer +0x18, DeathSoundCriteria (CCharString) +0x1c, Pausable +0x20, Paused +0x21.
// Shape: speed TU; ebx is the shared zero; the four scalars before the string member are init-list
// initialisers stored after the vtable store; the two bools after the CCharString constructor call.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString();                                          // 0x0099E4B0
};

class CTCEffectOnDie : public CTCBase { public:
    bool        StartedNonRecoverableFadeout;               // +0x0c
    long        Effect;                                     // +0x10
    long        Timer;                                      // +0x14
    long        FadeOutTimer;                               // +0x18
    CCharString DeathSoundCriteria;                         // +0x1c
    bool        Pausable;                                   // +0x20
    bool        Paused;                                     // +0x21
    CTCEffectOnDie(CThing& parent);
};

CTCEffectOnDie::CTCEffectOnDie(CThing& parent)
    : CTCBase(parent)
    , StartedNonRecoverableFadeout(false)
    , Effect(0)
    , Timer(0)
    , FadeOutTimer(0)
    , DeathSoundCriteria()
    , Pausable(true)
    , Paused(false)
{
}