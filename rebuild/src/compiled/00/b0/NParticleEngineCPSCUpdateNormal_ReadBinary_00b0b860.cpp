// NParticleEngine::CPSCUpdateNormal::ReadBinary(CDataInputStream& in) -- vtable slot 15: read the normal-update settings back in WriteBinary order (fade integers, the twenty flag bits, the decal emitter name, fifteen floats, five vectors, the two parameter keys)
// Leaf: ?ReadBinary@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0B860, 3787 bytes, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ?ReadBinary@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the
//   CPSCUpdateNormal vtable 0x0129FC1C; the mirror of the landed CPSCUpdateNormal::WriteBinary 0x00B0B5C0.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (x37, the inlined bounds-checked Read),
//   CDataInputStream::ReadNullTerminatedString (virtual, vtable +0x18), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0,
//   CDataInputStream::Read3DVector 0x009950F0 (x5; FableWin ?Read3DVector@CDataInputStream@@QAE?AVC3DVector@@XZ, hidden result pointer).
// Inlined: CDataInputStream::ReadULONG / ReadFloat / ReadEBOOL (engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM; this chunk adds
//   the Read3DVector declaration to that branch).
// Layout: PDB (Ego_r) CPSCUpdateNormal, retail offsets equal the PDB up to +0x98 (the landed ctor 0x00B0C9C0 / DatabaseCopy model).
// Shape: one read per member in WriteBinary order (ParticleCollideWithAnything between ParticleCollideWithGround and
//   ParticleDieOnCollision); the last ULONG read's three exits are tail-duplicated by VC7.1 (four `ret 4`).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CDataInputStream.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);         // 0x0099EFB0
};
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void ReadBinary(CDataInputStream&) = 0;
};
class CPSCUpdateNormal : public CPSCBase {
public:
    unsigned long FadeInEndInteger : 10;           // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;
    unsigned long UseSystemLifeSecs : 1;
    unsigned long SystemAlphaFadeEnable : 1;
    unsigned long EmissionFadeEnable : 1;
    unsigned long ParticleCollideWithGround : 1;
    unsigned long ParticleDieOnCollision : 1;
    unsigned long UseAllAttractors : 1;
    unsigned long RandomisePosEnable : 1;
    bool RandomisePosDistVaryEnable : 1;           // +0x10
    bool SetOrientationFromDirection : 1;
    bool SetOrientationFromGame : 1;
    bool ParticleCreateDecal : 1;
    bool ParticleCreateDecalEmitter : 1;
    bool ParticleCollideWithAnything : 1;
    CCharString DecalEmitterName;                  // +0x14
    float SystemLifeSecs;                          // +0x18
    float ParticleLifeSecs;                        // +0x1c
    float WindFactor;                              // +0x20
    float GravityFactor;                           // +0x24
    float AirResistance;                           // +0x28
    float ParticleAccelerationScale;               // +0x2c
    float InitialRotationX;                        // +0x30
    float InitialRotationY;                        // +0x34
    float InitialRotationZ;                        // +0x38
    float RotationMinAngleSpeed;                   // +0x3c
    float RotationMaxAngleSpeed;                   // +0x40
    float ParticleBounce;                          // +0x44
    float SystemAlphaFadeMinimum;                  // +0x48
    float RandomisePosDistVaryScale;               // +0x4c
    float EmissionFadeMinimum;                     // +0x50
    C3DVector ParticleSystemOffset;                // +0x54
    C3DVector ParticleAcceleration;                // +0x60
    C3DVector RotationAxis;                        // +0x6c
    C3DVector RandomisePosSpeed;                   // +0x78
    C3DVector RandomisePosScale;                   // +0x84
    unsigned long AccelerationParam;               // +0x90
    unsigned long OrientFromGameParam;             // +0x94
    long LifeTimer;                                // +0x98
    void* DecalEmitterList;                        // +0x9c
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCUpdateNormal::ReadBinary(CDataInputStream& in)
{
    FadeInEndInteger = in.ReadULONG();
    FadeOutBeginInteger = in.ReadULONG();
    UseParticleLifeSecs = in.ReadEBOOL();
    UseRandomRotationAxis = in.ReadEBOOL();
    UseRandomInitialRotation = in.ReadEBOOL();
    UseAccelerationParam = in.ReadEBOOL();
    StayWithEmitter = in.ReadEBOOL();
    UseSystemLifeSecs = in.ReadEBOOL();
    SystemAlphaFadeEnable = in.ReadEBOOL();
    EmissionFadeEnable = in.ReadEBOOL();
    ParticleCollideWithGround = in.ReadEBOOL();
    ParticleCollideWithAnything = in.ReadEBOOL();
    ParticleDieOnCollision = in.ReadEBOOL();
    UseAllAttractors = in.ReadEBOOL();
    RandomisePosEnable = in.ReadEBOOL();
    RandomisePosDistVaryEnable = in.ReadEBOOL();
    SetOrientationFromDirection = in.ReadEBOOL();
    SetOrientationFromGame = in.ReadEBOOL();
    ParticleCreateDecal = in.ReadEBOOL();
    ParticleCreateDecalEmitter = in.ReadEBOOL();
    DecalEmitterName = in.ReadNullTerminatedString();
    SystemLifeSecs = in.ReadFloat();
    ParticleLifeSecs = in.ReadFloat();
    WindFactor = in.ReadFloat();
    GravityFactor = in.ReadFloat();
    AirResistance = in.ReadFloat();
    ParticleAccelerationScale = in.ReadFloat();
    InitialRotationX = in.ReadFloat();
    InitialRotationY = in.ReadFloat();
    InitialRotationZ = in.ReadFloat();
    RotationMinAngleSpeed = in.ReadFloat();
    RotationMaxAngleSpeed = in.ReadFloat();
    ParticleBounce = in.ReadFloat();
    SystemAlphaFadeMinimum = in.ReadFloat();
    RandomisePosDistVaryScale = in.ReadFloat();
    EmissionFadeMinimum = in.ReadFloat();
    ParticleSystemOffset = in.Read3DVector();
    ParticleAcceleration = in.Read3DVector();
    RotationAxis = in.Read3DVector();
    RandomisePosSpeed = in.Read3DVector();
    RandomisePosScale = in.Read3DVector();
    AccelerationParam = in.ReadULONG();
    OrientFromGameParam = in.ReadULONG();
}
}