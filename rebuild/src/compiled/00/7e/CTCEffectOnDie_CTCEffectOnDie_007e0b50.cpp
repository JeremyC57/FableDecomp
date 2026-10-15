// CTCEffectOnDie::CTCEffectOnDie(CThing* parent) -- CTCBase(parent), own vtable, cleared flags and timers, empty DeathSoundCriteria, Pausable = true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0B50, length 56 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CCharString::CCharString() @ 0x0099E4B0.
// Vtable 0x0126FEB4 = CTCEffectOnDie (rtti-vtables.tsv).
// PDB CTCEffectOnDie (0x24 bytes): StartedNonRecoverableFadeout +0x0c, Effect +0x10, Timer +0x14,
// FadeOutTimer +0x18, DeathSoundCriteria (CCharString) +0x1c, Pausable +0x20, Paused +0x21.
// Shape: speed TU; ebx is the shared zero; the four scalars before the string member are init-list
// initialisers stored after the vtable store; the two bools after the CCharString constructor call.
struct CThing;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

struct CCharString {
    void* m_pData;
    CCharString();                                          // 0x0099E4B0
};

struct CTCEffectOnDie : CTCBase_Calls {
    bool        StartedNonRecoverableFadeout;               // +0x0c
    long        Effect;                                     // +0x10
    long        Timer;                                      // +0x14
    long        FadeOutTimer;                               // +0x18
    CCharString DeathSoundCriteria;                         // +0x1c
    bool        Pausable;                                   // +0x20
    bool        Paused;                                     // +0x21
    virtual void v00();                                     // own vtable 0x0126FEB4
    CTCEffectOnDie(CThing* parent);
};

CTCEffectOnDie::CTCEffectOnDie(CThing* parent)
    : CTCBase_Calls(parent)
    , StartedNonRecoverableFadeout(false)
    , Effect(0)
    , Timer(0)
    , FadeOutTimer(0)
    , DeathSoundCriteria()
    , Pausable(true)
    , Paused(false)
{
}