// CTCDParticleEmitter::CreateParticleWaitingFor() -- create the particle emitter the component has been waiting for from its pending creation parameters (interpolated or plain), copy the pending update parameters / creation-disabled / always-update state onto it, adopt it and drop the parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E14E0, 309 bytes (+ int3 padding), plain `ret`.
// Callees: CThing::GetPosition @ 0x004C73D0, CParticleEmitterManager::Get @ 0x00AEAAF0 (static), CreateInterpolatedEmitter(long,
//   long, float, const C3DVector&, const CRightHandedSet&, bool) @ 0x00AEB8C0, CreateEmitter(long, const C3DVector&, bool,
//   const CRightHandedSet&) @ 0x00AEB6D0, NParticleEngine::CParticleEmitter::SetUpdateParameters @ 0x00AF0610,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::IsAffectedByBulletTime @ 0x004C9920, SetPEmitter @
//   0x006E1080, the count block's destroy function (+4, the object in ecx) and operator delete @ 0x00BFE9BC.
// Layout = Ego_r.pdb CTCDParticleEmitter as the landed SetAlwaysUpdate 0x006E0590 / SetParticleCreationDisabled 0x006E0690:
//   ParentThing +0x04, ParticleTypeID +0x10, PAttachedToThing +0x1c, PCreationParameters {object +0x3c, count block +0x40},
//   flags +0x44 (WaitingForParticleToLoad bit 0), +0x45 (AttachedToThing bit 0, AlwaysUpdate bit 1);
//   CParticleCreationParameters (Ego_r.pdb): the four bools +0..+3, StartParticleID +4, EndParticleID +8, Interpolate +0xc,
//   RHSet +0x10, UpdateParameters +0x28; the retail emitter's bit groups at +0x40 (ParticleCreationDisabled bit 6,
//   WaitingToSleep bit 9) and +0x44 (AlwaysUpdate bit 4).
// Shape: the parent thing and the parameters are named locals read first; each emitter setup statement re-reads the
//   parameters through the counted pointer; the emitter's inline setters give the 16-bit insert and the `or 200h` on the
//   combined word; the always-update test is the inline alive test plus a third GetPItem; the parameters are dropped with
//   the counted pointer's inline clear (count block re-read after the decrement, block then object zeroed).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool IsAffectedByBulletTime();              // 0x004C9920
};
struct CParticleUpdateParameters { unsigned char _body[0x24]; };
struct CParticleCreationParameters {
    bool  InterpolatingParticle;                // +0x00
    bool  ParticleCreationDisabled;             // +0x01
    bool  UpdateParametersSet;                  // +0x02
    bool  AlwaysUpdateFlag;                     // +0x03
    long  StartParticleID;                      // +0x04
    long  EndParticleID;                        // +0x08
    float Interpolate;                          // +0x0c
    CRightHandedSet RHSet;                      // +0x10
    CParticleUpdateParameters UpdateParameters; // +0x28
};
namespace NParticleEngine {
struct CParticleEmitter {                       // retail layout
    unsigned char _pad00[0x40];
    unsigned short Priority : 3;                // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;    // bit 6
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;          // bit 9
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned char _pad42[2];
    bool PreWaterEffect : 1;                    // +0x44 bit 0
    bool WaterEffect : 1;
    bool ZBufferWriteable : 1;
    bool Visible : 1;
    bool AlwaysUpdate : 1;                      // +0x44 bit 4
    void SetParticleCreationDisabled(bool disabled) { ParticleCreationDisabled = disabled; }
    void Sleep() { WaitingToSleep = true; }
    void SetAlwaysUpdate(bool alwaysUpdate) { AlwaysUpdate = alwaysUpdate; }
    void SetUpdateParameters(const CParticleUpdateParameters& parameters);   // 0x00AF0610
};
}
struct CParticleEmitterManager {
    static CParticleEmitterManager* __fastcall Get();   // 0x00AEAAF0
    NParticleEngine::CParticleEmitter* CreateEmitter(long handle, const C3DVector& position, bool force,
                                                     const CRightHandedSet& orientation);   // 0x00AEB6D0
    NParticleEngine::CParticleEmitter* CreateInterpolatedEmitter(long fromHandle, long toHandle, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation, bool flag);   // 0x00AEB8C0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* PData;                                // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SCountBlock {
    long  m_references;                         // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};
struct CCreationParametersPointer {             // CCountedPointer<CParticleCreationParameters>
    CParticleCreationParameters* Object;        // +0x00
    SCountBlock*                 PCount;        // +0x04
    CParticleCreationParameters* operator->() const { return Object; }
    void Clear()
    {
        if (PCount) {
            --PCount->m_references;
            if (PCount->m_references == 0) {
                PCount->m_pfnDestroy(PCount->m_pObject);
                delete PCount;
            }
        }
        PCount = 0;
        Object = 0;
    }
};
struct CTCDParticleEmitter {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];          // CTCBase tail, EmitterName
    long ParticleTypeID;                        // +0x10
    CBaseIntelligentPointer PEmitter;           // +0x14
    CBaseIntelligentPointer PAttachedToThing;   // +0x1c
    unsigned char _pad24[0x3c - 0x24];          // AttachmentFlags .. FramesLeftToWaitForParticleToCreate
    CCreationParametersPointer PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;          // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    bool AttachedToThing : 1;                   // +0x45 bit 0
    bool AlwaysUpdate : 1;                      // +0x45 bit 1
    bool Asleep : 1;
    void SetPEmitter(NParticleEngine::CParticleEmitter* emitter);   // 0x006E1080
    void CreateParticleWaitingFor();
};

void CTCDParticleEmitter::CreateParticleWaitingFor()
{
    CThing* thing = ParentThing;
    CParticleCreationParameters* params = PCreationParameters.Object;
    NParticleEngine::CParticleEmitter* emitter;
    if (params->InterpolatingParticle)
        emitter = CParticleEmitterManager::Get()->CreateInterpolatedEmitter(params->StartParticleID, params->EndParticleID,
            params->Interpolate, thing->GetPosition(), params->RHSet, false);
    else
        emitter = CParticleEmitterManager::Get()->CreateEmitter(ParticleTypeID, thing->GetPosition(), false, params->RHSet);
    if (emitter) {
        if (PCreationParameters->UpdateParametersSet)
            emitter->SetUpdateParameters(PCreationParameters->UpdateParameters);
        emitter->SetParticleCreationDisabled(PCreationParameters->ParticleCreationDisabled);
        if (PCreationParameters->ParticleCreationDisabled)
            emitter->Sleep();
        if (AttachedToThing && PAttachedToThing.GetPItem() && !PAttachedToThing.GetPItem()->IsDead() &&
            !PAttachedToThing.GetPItem()->IsAffectedByBulletTime()) {
            emitter->SetAlwaysUpdate(true);
            AlwaysUpdate = true;
        } else
            emitter->SetAlwaysUpdate(PCreationParameters->AlwaysUpdateFlag);
        SetPEmitter(emitter);
        WaitingForParticleToLoad = false;
        PCreationParameters.Clear();
    }
}
