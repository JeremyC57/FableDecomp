// CTCEffectOnDie::~CTCEffectOnDie() -- own vtable, destroy DeathSoundCriteria (+0x1c), then the CTCBase destructor (tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0B90, length 25 bytes. Vtable 0x0126FEB4 (CTCEffectOnDie).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (labelled "_Cons_val<...>", propagated), CTCBase::~CTCBase @ 0x00686830 (stores vtable
// 0x0125BE0C and jumps to CBase::~CBase; labelled "~CChunkedFileChunk", propagated).
// Layout from Ego_r.pdb CTCEffectOnDie (36 bytes): StartedNonRecoverableFadeout +0xc, Effect +0x10, Timer +0x14, FadeOutTimer +0x18,
// DeathSoundCriteria +0x1c, Pausable +0x20, Paused +0x21. Shape: the plain `??1` body (a polymorphic class with a non-virtual
// destructor in the model so the lander grades the non-deleting symbol), `mov ecx,esi ; pop esi ; jmp` into the base.
// Leaf: ??1CTCEffectOnDie
struct CCharString {                                // 4 bytes
    void* storage_;
    ~CCharString();                                 // 0x0099EAE0
};
struct CTCBase {                                    // vtable 0x0125BE0C, 0xc bytes
    virtual void v00();
    unsigned char _pad04[0x08];                     // +0x04
    ~CTCBase();                                     // 0x00686830
};
struct CTCEffectOnDie : CTCBase {                   // vtable 0x0126FEB4
    virtual void v00();
    bool        StartedNonRecoverableFadeout;       // +0x0c
    long        Effect;                             // +0x10
    long        Timer;                              // +0x14
    long        FadeOutTimer;                       // +0x18
    CCharString DeathSoundCriteria;                 // +0x1c
    bool        Pausable;                           // +0x20
    bool        Paused;                             // +0x21
    ~CTCEffectOnDie();
};

CTCEffectOnDie::~CTCEffectOnDie()
{
}