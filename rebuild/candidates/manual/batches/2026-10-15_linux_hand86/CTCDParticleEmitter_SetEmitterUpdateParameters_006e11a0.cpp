// CTCDParticleEmitter::SetEmitterUpdateParameters(const CParticleUpdateParameters<unsigned long, unsigned long>& parameters) -- hand the parameters to the live emitter, or stash them in the pending creation parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E11A0, length 76 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2); NParticleEngine::CParticleEmitter::SetUpdateParameters(const
// CParticleUpdateParameters&) @ 0x00AF0610; CParticleUpdateParameters<unsigned long, unsigned long>::operator= @ 0x006E2570
// (manifest label "CParticleEmitter::SetUpdateParameters" is propagated; it is called on PCreationParameters->UpdateParameters).
// Signature from ghidra_out/egor_signatures.tsv (`?SetEmitterUpdateParameters@CTCDParticleEmitter@@QAEXABV?$CParticleUpdateParameters@KK@NParticleEngine@@@Z`).
// PDB CTCDParticleEmitter: PEmitter +0x14, PCreationParameters (CCountedPointer: object +0x3c), bitfield byte +0x44
// (bit 0 WaitingForParticleToLoad). PDB CParticleCreationParameters: UpdateParametersSet +2, UpdateParameters +0x28.
// Shape: the argument is pushed before the GetPItem call that yields the emitter (plain chained call); the
// pending branch re-reads the counted pointer's object for the flag store after the assignment call.
namespace NParticleEngine {
template<class A, class B> struct CParticleUpdateParameters {
    unsigned char _body[0x20];                              // opaque: only its address is used here
    CParticleUpdateParameters& operator=(const CParticleUpdateParameters& other);   // 0x006E2570
};

struct CParticleEmitter {
    void SetUpdateParameters(const CParticleUpdateParameters<unsigned long, unsigned long>& parameters);   // 0x00AF0610
};
}

typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> CEmitterUpdateParameters;

struct CParticleCreationParameters {
    bool                     InterpolatingParticle;         // +0x00
    bool                     ParticleCreationDisabled;      // +0x01
    bool                     UpdateParametersSet;           // +0x02
    bool                     AlwaysUpdateFlag;              // +0x03
    unsigned char            _body[0x28 - 0x04];            // StartParticleID, EndParticleID, Interpolate, RHSet
    CEmitterUpdateParameters UpdateParameters;              // +0x28
};

struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    NParticleEngine::CParticleEmitter* GetPItem() const;    // 0x00A01B50
};

template<class T> struct CCountedPointer {
    T*    Object;
    long* PCount;
    T* operator->() const { return Object; }
};

struct CTCDParticleEmitter {
    virtual void v00();
    unsigned char           _base[0x8];                     // CTCBase: ParentThing +4, flags +8/+9
    void*                   EmitterName;                    // +0x0c CCharString
    long                    ParticleTypeID;                 // +0x10
    CBaseIntelligentPointer PEmitter;                       // +0x14
    unsigned char           _body[0x3c - 0x1c];             // PAttachedToThing .. FramesLeftToWaitForParticleToCreate
    CCountedPointer<CParticleCreationParameters> PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    void SetEmitterUpdateParameters(const CEmitterUpdateParameters& parameters);
};

void CTCDParticleEmitter::SetEmitterUpdateParameters(const CEmitterUpdateParameters& parameters)
{
    if (PEmitter.GetPItem() != 0) {
        PEmitter.GetPItem()->SetUpdateParameters(parameters);
    } else if (WaitingForParticleToLoad) {
        PCreationParameters->UpdateParameters = parameters;
        PCreationParameters->UpdateParametersSet = true;
    }
}
