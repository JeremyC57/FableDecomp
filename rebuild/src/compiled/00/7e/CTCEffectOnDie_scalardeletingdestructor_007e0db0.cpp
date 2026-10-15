// CTCEffectOnDie::`scalar deleting destructor'(unsigned int) -- stores the CTCEffectOnDie vtable, destroys the death-sound criteria string (+0x1c), runs CTCBase::~CTCBase and frees on bit 0
// Leaf: ??_GCTCEffectOnDie
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0DB0, length 46 bytes (vtable 0x0126FEB4 slot 0).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>", propagated), CTCBase::~CTCBase
// @ 0x00686830 (manifest "~CChunkedFileChunk", propagated), operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CTCEffectOnDie vtable 0x0126FEB4.
// Layout: PDB CTCEffectOnDie (36 bytes): StartedNonRecoverableFadeout +0x0c, Effect +0x10, Timer +0x14,
// FadeOutTimer +0x18, DeathSoundCriteria (CCharString) +0x1c, Pausable +0x20, Paused +0x21.
// Shape: own vtable store (scheduled after the member `lea`), the string destructor, the base destructor.
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

struct CTCBase {                                   // engine/CTCBase.h offsets, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing& owner);                        // 0x00686800
    ~CTCBase();                                    // 0x00686830
};

struct CTCEffectOnDie : CTCBase {                  // vtable 0x0126FEB4
    bool        StartedNonRecoverableFadeout;      // +0x0c
    long        Effect;                            // +0x10
    long        Timer;                             // +0x14
    long        FadeOutTimer;                      // +0x18
    CCharString DeathSoundCriteria;                // +0x1c
    bool        Pausable;                          // +0x20
    bool        Paused;                            // +0x21
    CTCEffectOnDie(CThing& owner) : CTCBase(owner) { }
    __forceinline virtual ~CTCEffectOnDie() { }
};

CTCEffectOnDie* Create_CTCEffectOnDie(CThing& owner)
{
    return new CTCEffectOnDie(owner);
}