// CTCDParticleEmitter::SetParticleCreationDisabled(bool disabled) -- when the flag changes: record it, and push it to the live emitter (disabling also sends the emitter to sleep, enabling wakes it) or to the pending creation parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0690, length 145 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3).
// Layout: CTCDParticleEmitter as in the landed SetAlwaysUpdate 0x006E0590 (ParticleCreationDisabled = bit 3 of +0x44, Waiting... bit 0,
// PCreationParameters object +0x3C, its ParticleCreationDisabled byte +1). NParticleEngine::CParticleEmitter: the PDB `ushort` bit
// fields at +0x4C sit at +0x40 in retail (0xC lower, see 0x006E0590): Priority:3, IsScreenDisplacement, ReadZBuffer, Dead,
// ParticleCreationDisabled (bit 6), ContinuousEmitter, Initialised, WaitingToSleep (bit 9), Sleeping (bit 10), Emitter2D.
// Shape: the change test compares the extracted bit with the argument; each emitter statement re-fetches GetPItem(); the emitter's
// inline setters compile to the 16-bit insert for the bool and byte `or`/`and` for the constant sleep bits.
struct CParticleCreationParameters {
    bool InterpolatingParticle;                 // +0x00
    bool ParticleCreationDisabled;              // +0x01
    bool UpdateParametersSet;                   // +0x02
    bool AlwaysUpdateFlag;                      // +0x03
};
namespace NParticleEngine {
class CParticleEmitter { public:                       // retail layout
    unsigned char _pad00[0x40];
    unsigned short Priority : 3;                // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;    // bit 6
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;             // bit 8
    unsigned short WaitingToSleep : 1;          // bit 9
    unsigned short Sleeping : 1;                // bit 10
    unsigned short Emitter2D : 1;
    void SetParticleCreationDisabled(bool disabled) { ParticleCreationDisabled = disabled; }
    void Sleep() { WaitingToSleep = true; }
    void WakeUp() { WaitingToSleep = false; Sleeping = false; }
};
}
struct CBaseIntelligentPointer {                // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                // +0x04
    NParticleEngine::CParticleEmitter* GetPItem() const;   // 0x00A01B50
};
template<class T> class CCountedPointer { public:
    T*    Object;
    long* PCount;
    T* operator->() const { return Object; }
};
class CTCDParticleEmitter { public:
    virtual void v00();
    unsigned char           _base[0x8];         // CTCBase: ParentThing +4, flags +8/+9
    void*                   EmitterName;        // +0x0c CCharString
    long                    ParticleTypeID;     // +0x10
    CBaseIntelligentPointer PEmitter;           // +0x14
    CBaseIntelligentPointer PAttachedToThing;   // +0x1c
    unsigned char           _body[0x3c - 0x24]; // AttachmentFlags .. FramesLeftToWaitForParticleToCreate
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;          // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;          // bit 3
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    void SetParticleCreationDisabled(bool disabled);
};

void CTCDParticleEmitter::SetParticleCreationDisabled(bool disabled)
{
    if (ParticleCreationDisabled != disabled) {
        ParticleCreationDisabled = disabled;
        if (PEmitter.GetPItem()) {
            PEmitter.GetPItem()->SetParticleCreationDisabled(disabled);
            if (disabled)
                PEmitter.GetPItem()->Sleep();
            else
                PEmitter.GetPItem()->WakeUp();
        } else if (WaitingForParticleToLoad) {
            PCreationParameters->ParticleCreationDisabled = disabled;
        }
    }
}