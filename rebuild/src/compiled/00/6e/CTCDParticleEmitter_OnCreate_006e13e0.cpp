// CTCDParticleEmitter::OnCreate() -- mark a dependent emitter's thing as not game-state persisted, clear the attachments, then create the particle emitter at the thing's position; if the manager cannot create it yet, wait ~0.9 s for the particle to load, keeping the creation parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E13E0, 251 bytes (+ int3 padding), plain `ret`.
// Callees: CThing::SetAsGameStatePersisted(bool) @ 0x004C9C10, CTCDParticleEmitter::ClearAttachments @ 0x006E0AF0,
//   CThing::GetPos @ 0x004C73D0, CParticleEmitterManager::Get @ 0x00AEAAF0 (static; manifest "GFGetPreMainMemoryUsed"
//   is a byte twin), CParticleEmitterManager::CreateEmitter(long, const C3DVector&, bool, const CRightHandedSet&) @
//   0x00AEB6D0, SetPEmitter @ 0x006E1080, __ftol2 @ 0x00BFEA70, operator new @ 0x00BFEA1A, CParticleCreationParameters(
//   long, const CRightHandedSet&) @ 0x006E25C0, CCountedPointer<CParticleCreationParameters>::Reset @ 0x006E2710.
// Globals: the particles-enabled flag @ 0x01375754, g_framesPerSecond @ 0x01375550, the identity CRightHandedSet @
//   0x01380104 (descriptive names), 0.9f @ 0x0125FEDC.
// Layout = Ego_r.pdb CTCDParticleEmitter (matches retail): ParentThing +0x04, ParticleTypeID +0x10,
//   FramesLeftToWaitForParticleToCreate +0x38, PCreationParameters +0x3c, the flag bits of +0x44
//   (WaitingForParticleToLoad 0, ForceParticleToLoad 1, IndependantObject 2).
// Shape: the parent thing is a named local read before the orientation (its `mov ecx,[esi+4]` precedes the bit read;
//   read inline in the call it is loaded after the force bit and the type before GetPos); the orientation is a
//   named identity CRightHandedSet (six immediate stores interleaved with the argument
//   pushes); CreateEmitter's arguments evaluate right to left (the set, the force bit, GetPos, the type) before
//   the static manager getter; the new creation record is `p = new T(..)` handed to Reset (the null arm pushes eax=0).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward)
        : Up(up.X, up.Y, up.Z), Forward(forward.X, forward.Y, forward.Z) { }
};
extern long g_framesPerSecond;                  // 0x01375550
extern bool g_bParticlesEnabled;                // 0x01375754
extern const CRightHandedSet g_IdentityRHSet;   // 0x01380104
class CThing { public:
    void SetAsGameStatePersisted_004c9c10(bool persisted);   // 0x004C9C10
    const C3DVector& GetPos() const;       // 0x004C73D0
};
namespace NParticleEngine {
class CParticleEmitter;
}
struct CParticleEmitterManager {
    static CParticleEmitterManager* __fastcall Get();   // 0x00AEAAF0
    NParticleEngine::CParticleEmitter* CreateEmitter(long handle, const C3DVector& position, bool force,
                                                     const CRightHandedSet& orientation);   // 0x00AEB6D0
};
struct CParticleCreationParameters {            // 0x4c bytes
    unsigned char _body[0x4c];
    CParticleCreationParameters(long particleID, const CRightHandedSet& rhs);   // 0x006E25C0
};
struct CCreationParametersPointer {             // CCountedPointer<CTCDParticleEmitter::CParticleCreationParameters>
    CParticleCreationParameters* Data;
    void* Info;
    void Reset(CParticleCreationParameters* object);   // 0x006E2710
};
class CTCDParticleEmitter { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];          // CTCBase tail, EmitterName
    long ParticleTypeID;                        // +0x10
    unsigned char _pad14[0x38 - 0x14];          // PEmitter .. DieOffTimer
    long FramesLeftToWaitForParticleToCreate;   // +0x38
    CCreationParametersPointer PCreationParameters;   // +0x3c
    bool WaitingForParticleToLoad : 1;          // +0x44 bit 0
    bool ForceParticleToLoad : 1;               // bit 1
    bool IndependantObject : 1;                 // bit 2
    bool ParticleCreationDisabled : 1;
    bool ResetEmitterPosition : 1;
    bool ResetEmitterOrientation : 1;
    bool AttachedToCamera : 1;
    bool Moving : 1;
    void ClearAttachments();                    // 0x006E0AF0
    void SetPEmitter(NParticleEngine::CParticleEmitter* emitter);   // 0x006E1080
    void OnCreate();
};

void CTCDParticleEmitter::OnCreate()
{
    if (!IndependantObject)
        ParentThing->SetAsGameStatePersisted_004c9c10(false);
    ClearAttachments();
    if (g_bParticlesEnabled && ParticleTypeID) {
        CThing* thing = ParentThing;
        CRightHandedSet orientation(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f));
        NParticleEngine::CParticleEmitter* emitter = CParticleEmitterManager::Get()->CreateEmitter(
            ParticleTypeID, thing->GetPos(), ForceParticleToLoad, orientation);
        if (emitter) {
            SetPEmitter(emitter);
            return;
        }
        WaitingForParticleToLoad = true;
        FramesLeftToWaitForParticleToCreate = (long)(g_framesPerSecond * 0.9f);
        PCreationParameters.Reset(new CParticleCreationParameters(ParticleTypeID, g_IdentityRHSet));
    }
}