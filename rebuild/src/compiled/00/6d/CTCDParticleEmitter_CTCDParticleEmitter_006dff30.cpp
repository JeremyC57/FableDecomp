// CTCDParticleEmitter::CTCDParticleEmitter(CThing* parent) -- particle-emitter driver component: CTCDriverBase(parent), empty names, null emitter / attached-thing pointers, zeroed attachment data, no die-off timer, no creation parameters; only the reset-position/orientation flags set
// Leaf: ??0CTCDParticleEmitter
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
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
class CThing;
class CTCDriverBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCDriverBase(CThing* parent);              // 0x00771200
};
class CCharString { public:
    void* PStringData;
    CCharString();                              // 0x0099E4B0
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerEmitter : CBaseIntelligentPointer {   // vtable 0x0125A26C
    virtual void v01();
    CIntelligentPointerEmitter() {}
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // vtable 0x01238C6C
    virtual void v01();
    CIntelligentPointerThing() {}
};
#include "engine/CCountedPointer.h"
struct CParticleCreationParameters;
class CTCDParticleEmitter : public CTCDriverBase { public:
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