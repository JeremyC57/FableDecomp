// CTCDParticleEmitter::SetAlwaysUpdate(bool alwaysUpdate) -- push the flag to the live emitter (or to the pending creation parameters) and keep the component's own copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0590, length 85 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2).
// PDB CTCDParticleEmitter (0x48 bytes): PEmitter (CIntelligentPointer<NParticleEngine::CParticleEmitter>) +0x14,
// PCreationParameters (CCountedPointer<CParticleCreationParameters>: object +0x3c, count block +0x40),
// bitfield byte +0x44 (bit 0 WaitingForParticleToLoad ... bit 3 ParticleCreationDisabled ... bit 6
// AttachedToCamera), byte +0x45 (bit 0 AttachedToThing, bit 1 AlwaysUpdate, bit 2 Asleep).
// PDB CParticleCreationParameters: AlwaysUpdateFlag +3. NParticleEngine::CParticleEmitter: the AlwaysUpdate bit
// is bit 4 of the PDB byte +0x50; the retail emitter sits 0xc lower (a 12-byte vector and two 12-byte vector
// maps in front of it), so it is bit 4 of +0x44 -- engine/CParticleEmitter.h carries the PDB offsets, so the
// emitter is modelled locally with the retail offset.
// Shape: the argument is loaded into bl after the first null test; each bool:1 bitfield store is the
// `shl ; xor ; and ; xor` insert; the creation-parameters byte is a plain bool. The emitter's bit goes through
// an inline setter on the emitter (NParticleEngine::CParticleEmitter::SetAlwaysUpdate): written as a direct
// bitfield store on the GetPItem() result the lane forms `add eax,44h` once and addresses [eax] twice.
struct CParticleCreationParameters {
    bool InterpolatingParticle;                             // +0x00
    bool ParticleCreationDisabled;                          // +0x01
    bool UpdateParametersSet;                               // +0x02
    bool AlwaysUpdateFlag;                                  // +0x03
};

namespace NParticleEngine {
class CParticleEmitter { public:                                   // retail layout (PDB - 0xc from +0x18 on)
    unsigned char _pad[0x44];
    bool PreWaterEffect : 1;                                // +0x44 bit 0
    bool WaterEffect : 1;
    bool ZBufferWriteable : 1;
    bool Visible : 1;
    bool AlwaysUpdate : 1;                                  // +0x44 bit 4
    void SetAlwaysUpdate(bool alwaysUpdate) { AlwaysUpdate = alwaysUpdate; }
};
}

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

template<class T> class CCountedPointer { public:
    T*    Object;
    long* PCount;
    T* operator->() const { return Object; }
};

class CTCDParticleEmitter { public:
    virtual void v00();
    unsigned char           _base[0x8];                     // CTCBase: ParentThing +4, flags +8/+9
    void*                   EmitterName;                    // +0x0c CCharString
    long                    ParticleTypeID;                 // +0x10
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;                       // +0x14
    CIntelligentPointer<NParticleEngine::CParticleEmitter> PAttachedToThing;               // +0x1c
    unsigned long           AttachmentFlags;                // +0x24
    void*                   AttachmentPosName;              // +0x28 CCharString
    long                    AttachmentPosIndex;             // +0x2c
    float                   FloatOffset;                    // +0x30
    long                    DieOffTimer;                    // +0x34
    long                    FramesLeftToWaitForParticleToCreate;   // +0x38
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;                      // bit 3
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;                              // bit 6
    bool Moving : 1;
    bool AttachedToThing : 1;                               // +0x45 bit 0
    bool AlwaysUpdate : 1;                                  // +0x45 bit 1
    bool Asleep : 1;
    void SetAlwaysUpdate(bool alwaysUpdate);
};

void CTCDParticleEmitter::SetAlwaysUpdate(bool alwaysUpdate)
{
    if (PEmitter.GetPItem() != 0)
        PEmitter.GetPItem()->SetAlwaysUpdate(alwaysUpdate);
    else if (WaitingForParticleToLoad)
        PCreationParameters->AlwaysUpdateFlag = alwaysUpdate;
    AlwaysUpdate = alwaysUpdate;
}