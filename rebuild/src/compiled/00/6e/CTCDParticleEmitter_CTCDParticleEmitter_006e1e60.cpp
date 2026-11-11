// CTCDParticleEmitter::~CTCDParticleEmitter() -- release the pending creation parameters, then the member strings and intelligent pointers, then CTCBase
// Leaf: ??1CTCDParticleEmitter
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data, PBlock->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E1E60, length 92 bytes.
// Callees: the counted block's static deleter through its function pointer (`mov ecx,[eax+8] ; call [eax+4]`:
// CCountedPointer<CParticleCreationParameters>::DeleteData, a static __fastcall `SIXPAX` in the egor names);
// operator delete @ 0x00BFE9BC; CCharString::~CCharString @ 0x0099EAE0 (x2; manifest label "_Cons_val<...>" is
// propagated); CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x2); CTCBase::~CTCBase @ 0x00686830
// (tail jump; manifest label "~CChunkedFileChunk" is propagated).
// PDB CTCDParticleEmitter (0x48 bytes): EmitterName +0x0c, ParticleTypeID +0x10, PEmitter +0x14, PAttachedToThing
// +0x1c, AttachmentFlags +0x24, AttachmentPosName +0x28, ..., PCreationParameters (CCountedPointer: object +0x3c,
// block +0x40), bitfields +0x44/+0x45.
// Shape: no vtable store anywhere (the class is modelled without virtuals, the destructor is the plain
// `??1`); the counted pointer's inline destructor re-reads the block member after the decrement and
// after the deleter call (`--PBlock->Count; if (PBlock->Count == 0)`), deletes the POD block (no null
// test, `add esp,4`), and both members are zeroed after the null-test join; the members then die in
// reverse declaration order and the base destructor is tail-jumped.
struct CParticleCreationParameters;

// agentNF20 (2026-11-11): the base is the shared retail CTCBase model (engine/CTCBase.h, FABLE_RETAIL_CTCBASE: vptr +0x00, ParentThing
// +0x04, ToBeRemoved +0x08, IsInGlobalUpdateTCs +0x09, size 0x0C), so the tail binds ??1CTCBase@@UAE@XZ 0x00686830 (was the
// CTCBase_Calls stand-in). Its destructor is virtual, so this class's own one is the implicit ??1 (no vptr store, as retail),
// emitted by DestroyOne below.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                         // 0x0099EAE0
};

struct CIntelligentPointer {                                // vptr + PData; destroyed by the base destructor
    void* __vftable;
    void* PData;
    ~CIntelligentPointer();                                 // 0x00A01C10 (CBaseIntelligentPointer)
};

struct SCountedBlock {
    long  Count;                                            // +0x00
    void (__fastcall* DeleteData)(void* data);              // +0x04 static deleter
    void* Data;                                             // +0x08
};

#include "engine/CCountedPointer.h"

class CTCDParticleEmitter : public CTCBase { public:
    CCharString         EmitterName;                        // +0x0c
    long                ParticleTypeID;                     // +0x10
    CIntelligentPointer PEmitter;                           // +0x14 CIntelligentPointer<NParticleEngine::CParticleEmitter>
    CIntelligentPointer PAttachedToThing;                   // +0x1c CIntelligentPointer<CThing>
    unsigned long       AttachmentFlags;                    // +0x24
    CCharString         AttachmentPosName;                  // +0x28
    long                AttachmentPosIndex;                 // +0x2c
    float               FloatOffset;                        // +0x30
    long                DieOffTimer;                        // +0x34
    long                FramesLeftToWaitForParticleToCreate;// +0x38
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    unsigned char       Flags44;                            // +0x44 WaitingForParticleToLoad .. Moving
    unsigned char       Flags45;                            // +0x45 AttachedToThing, AlwaysUpdate, Asleep
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCDParticleEmitter* object)
{
    object->CTCDParticleEmitter::~CTCDParticleEmitter();
}