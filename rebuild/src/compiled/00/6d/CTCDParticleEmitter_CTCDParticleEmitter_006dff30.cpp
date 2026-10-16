// CTCDParticleEmitter::CTCDParticleEmitter(CThing* parent) -- particle-emitter driver component: CTCDriverBase(parent), empty names, null emitter / attached-thing pointers, zeroed attachment data, no die-off timer, no creation parameters; only the reset-position/orientation flags set
// Leaf: ??0CTCDParticleEmitter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DFF30, length 114 bytes (+ int3 padding). Vtable 0x0125FEEC (CTCDParticleEmitter).
// Callees: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200, CCharString::CCharString() @ 0x0099E4B0 (x2),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (x2: then the CIntelligentPointer<NParticleEngine::CParticleEmitter>
// vtable 0x0125A26C and the CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout: Ego_r.pdb CTCDParticleEmitter (0x48, retail identical): EmitterName +0xC, ParticleTypeID +0x10, PEmitter +0x14,
// PAttachedToThing +0x1C, AttachmentFlags +0x24, AttachmentPosName +0x28, AttachmentPosIndex +0x2C, FloatOffset +0x30,
// DieOffTimer +0x34, FramesLeftToWaitForParticleToCreate +0x38, PCreationParameters +0x3C (CCountedPointer), flag bytes +0x44/+0x45.
// Shape: initialisers in declaration order with one edi zero register (VC7.1 sinks the -1 below the next zero store); the eleven
// bool:1 flags are body assignments: the full +0x44 byte folds into one `mov byte ptr [esi+44h],30h`, the three +0x45 bits into
// one `and byte ptr [esi+45h],0F8h` emitted first.
struct CThing;
struct CTCDriverBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCDriverBase(CThing* parent);              // 0x00771200
};
struct CCharString {
    void* PStringData;
    CCharString();                              // 0x0099E4B0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerEmitter : CBaseIntelligentPointer {   // vtable 0x0125A26C
    virtual void v01();
    CIntelligentPointerEmitter() {}
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // vtable 0x01238C6C
    virtual void v01();
    CIntelligentPointerThing() {}
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
struct CParticleCreationParameters;
struct CTCDParticleEmitter : CTCDriverBase {
    CCharString EmitterName;                    // +0x0c
    long ParticleTypeID;                        // +0x10
    CIntelligentPointerEmitter PEmitter;        // +0x14
    CIntelligentPointerThing PAttachedToThing;  // +0x1c
    unsigned long AttachmentFlags;              // +0x24
    CCharString AttachmentPosName;              // +0x28
    long AttachmentPosIndex;                    // +0x2c
    float FloatOffset;                          // +0x30
    long DieOffTimer;                           // +0x34
    long FramesLeftToWaitForParticleToCreate;   // +0x38
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;          // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool AttachedToThing : 1;                   // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;
    virtual void v01();
    CTCDParticleEmitter(CThing* parent);
};

CTCDParticleEmitter::CTCDParticleEmitter(CThing* parent)
    : CTCDriverBase(parent)
    , ParticleTypeID(0)
    , AttachmentFlags(0)
    , AttachmentPosIndex(0)
    , FloatOffset(0)
    , DieOffTimer(-1)
    , FramesLeftToWaitForParticleToCreate(0)
{
    WaitingForParticleToLoad = false;
    ForceParticleToLoad = false;
    IndependantObject = false;
    ParticleCreationDisabled = false;
    ResetEmitterPosition = true;
    ResetEmitterOrientation = true;
    AttachedToCamera = false;
    Moving = false;
    AttachedToThing = false;
    AlwaysUpdate = false;
    Asleep = false;
}