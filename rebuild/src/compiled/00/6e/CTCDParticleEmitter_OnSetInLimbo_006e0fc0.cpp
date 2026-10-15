// CTCDParticleEmitter::OnSetInLimbo(bool inLimbo) -- sync the particle-creation switch with the component's state, clearing the live emitter's particles and decals when it gets disabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0FC0, length 81 bytes.
// Callees: CTCDParticleEmitter::IsParticleCreationDisabled() const @ 0x006E0DF0; ::SetParticleCreationDisabled(bool)
// @ 0x006E0690 (x2); CBaseIntelligentPointer::GetPItem @ 0x00A01B50; NParticleEngine::CParticleEmitter::ClearAllParticles(CIEngine*)
// @ 0x00AED140 and ::ClearAllDecals(CIEngine*) @ 0x00AED1E0 (both labelled ClearAllDecals in the manifest; the egor
// function order ClearAllPrimitives, ClearAllParticles, ClearAllDecals, EnterTimelineEnd matches the retail order
// ..., 0x00AED140, 0x00AED1E0, 0x00AED250).
// Signatures from ghidra_out/egor_signatures.tsv (`?OnSetInLimbo@CTCDParticleEmitter@@UAEX_N@Z`,
// `?IsParticleCreationDisabled@CTCDParticleEmitter@@IBE_NXZ`, `?SetParticleCreationDisabled@CTCDParticleEmitter@@IAEX_N@Z`).
// PDB CTCDParticleEmitter: PEmitter +0x14, bitfield byte +0x44 (bit 3 ParticleCreationDisabled).
// Shape: the flag byte is loaded once after the predicate's `test al,al` and tested in both arms; the
// emitter pointer is taken once into esi (`this` is not needed after it) for the two clears; the
// argument is unused (`ret 4`).
struct CIEngine;

namespace NParticleEngine {
struct CParticleEmitter {
    void ClearAllParticles(CIEngine* engine);               // 0x00AED140
    void ClearAllDecals(CIEngine* engine);                  // 0x00AED1E0
};
}

struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    NParticleEngine::CParticleEmitter* GetPItem() const;    // 0x00A01B50
};

struct CTCDParticleEmitter {
    virtual void v00();
    unsigned char           _base[0x8];                     // CTCBase: ParentThing +4, flags +8/+9
    void*                   EmitterName;                    // +0x0c CCharString
    long                    ParticleTypeID;                 // +0x10
    CBaseIntelligentPointer PEmitter;                       // +0x14
    unsigned char           _body[0x44 - 0x1c];             // PAttachedToThing .. PCreationParameters
    bool WaitingForParticleToLoad : 1;                      // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;                      // bit 3
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool IsParticleCreationDisabled() const;                // 0x006E0DF0
    void SetParticleCreationDisabled(bool disabled);        // 0x006E0690
    void OnSetInLimbo(bool inLimbo);
};

void CTCDParticleEmitter::OnSetInLimbo(bool inLimbo)
{
    if (IsParticleCreationDisabled()) {
        if (!ParticleCreationDisabled) {
            SetParticleCreationDisabled(true);
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            if (emitter != 0) {
                emitter->ClearAllParticles(0);
                emitter->ClearAllDecals(0);
            }
        }
    } else if (ParticleCreationDisabled) {
        SetParticleCreationDisabled(false);
    }
}