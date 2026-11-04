// NParticleEngine::CPSCUpdateNormal::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend the normal-update settings towards another update component (packed unsigned bit-fields lerped and rounded, floats and vectors lerped, bool flags / parameter keys / decal emitter name taken from the target past the halfway point)
// Leaf: ?InterpolateTo@CPSCUpdateNormal@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A920, 2635 bytes, thiscall, `ret 8`.
// Retail-asm: 0x00B0A982 `fistp dword ptr [esp+10h]` (and every later unsigned GFInterpolate rounding of this body) -- the engine's
//   inline-asm float-to-long helper GFFloatToLongNear (shared engine/GFFloatToLongNear.h); VC7.1 emits no bare dword fistp from C++.
//   This row carries no asm of its own.
// Identity: FableWin.pdb ?InterpolateTo@CPSCUpdateNormal@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the CPSCUpdateNormal vtable
//   0x0129FC1C (the CPSC family's slot 14, as the landed 0x00B062A0 / 0x00B1BDA0 / 0x00B18C60). CPSCUpdateNormal derives from CPSCBase
//   directly (no base InterpolateTo call).
// Callees: CCharString(const CCharString&) 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0 (the by-value
//   GFInterpolate(CCharString) result). Inlined: the NParticleEngine::GFInterpolate overloads of FableWin.pdb (__fastcall: MMMM float,
//   KKKM unsigned long, _N_N0M bool, ?AVC3DVector@@ABV2@0M vector -- out of line at 0x00AF3CF0 but expanded here, __forceinline --,
//   ?AVCCharString@@ABV2@0M name) and GFInterpolateUpdateParams (KKKM), as in the landed CPSCDecalRenderer::InterpolateTo 0x00B1BDA0.
// Constants: 1.0f 0x0122DED8, 0.5f 0x0122F59C, 2^32 0x0122DCB4 (unsigned -> float fix-up).
// Layout: PDB (Ego_r) CPSCUpdateNormal; retail offsets equal the PDB up to +0x98 (the landed ctor 0x00B0C9C0 / DatabaseCopy 0x00B0B370
//   model): ulong bit-field word +0x0c, bool bit-field byte +0x10, DecalEmitterName +0x14, floats +0x18..+0x50, vectors +0x54..+0x84,
//   AccelerationParam / OrientFromGameParam +0x90 / +0x94, LifeTimer +0x98 (not interpolated), DecalEmitterList +0x9c (4-byte STLport list).
// Shape: one statement per member in the editor's transfer order (the DatabaseCopy / DatabaseSerialise order); every 1-bit unsigned flag
//   and the two 10-bit fade integers go through the unsigned long overload (`fild ; jge ; fadd 2^32`), `1 - t` is computed once.
#include "engine/GFFloatToLongNear.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                          // 0x0099EC30
    ~CCharString();                                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);         // 0x0099EFB0
};
namespace NParticleEngine {
inline float __fastcall GFInterpolate(float a, float b, float t) { return a * (1.0f - t) + b * t; }
inline unsigned long __fastcall GFInterpolate(unsigned long a, unsigned long b, float t) { return GFFloatToLongNear((1.0f - t) * a + b * t); }
inline bool __fastcall GFInterpolate(bool a, bool b, float t) { return t > 0.5f ? b : a; }
inline unsigned long __fastcall GFInterpolateUpdateParams(unsigned long a, unsigned long b, float t) { return t > 0.5f ? b : a; }
__forceinline C3DVector __fastcall GFInterpolate(const C3DVector& a, const C3DVector& b, float t) { return a * (1.0f - t) + b * t; }
inline CCharString __fastcall GFInterpolate(const CCharString& a, const CCharString& b, float t) { return t > 0.5f ? b : a; }
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void InterpolateTo(const CPSCBase&, float) = 0;
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
    virtual void InterpolateTo(const CPSCBase& other, float t);
};
void CPSCUpdateNormal::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCUpdateNormal& source = static_cast<const CPSCUpdateNormal&>(other);
    UseParticleLifeSecs = GFInterpolate((unsigned long)UseParticleLifeSecs, (unsigned long)source.UseParticleLifeSecs, t);
    ParticleLifeSecs = GFInterpolate(ParticleLifeSecs, source.ParticleLifeSecs, t);
    UseSystemLifeSecs = GFInterpolate((unsigned long)UseSystemLifeSecs, (unsigned long)source.UseSystemLifeSecs, t);
    SystemLifeSecs = GFInterpolate(SystemLifeSecs, source.SystemLifeSecs, t);
    ParticleSystemOffset = GFInterpolate(ParticleSystemOffset, source.ParticleSystemOffset, t);
    WindFactor = GFInterpolate(WindFactor, source.WindFactor, t);
    GravityFactor = GFInterpolate(GravityFactor, source.GravityFactor, t);
    AirResistance = GFInterpolate(AirResistance, source.AirResistance, t);
    ParticleAcceleration = GFInterpolate(ParticleAcceleration, source.ParticleAcceleration, t);
    ParticleAccelerationScale = GFInterpolate(ParticleAccelerationScale, source.ParticleAccelerationScale, t);
    UseAccelerationParam = GFInterpolate((unsigned long)UseAccelerationParam, (unsigned long)source.UseAccelerationParam, t);
    AccelerationParam = GFInterpolateUpdateParams(AccelerationParam, source.AccelerationParam, t);
    UseAllAttractors = GFInterpolate((unsigned long)UseAllAttractors, (unsigned long)source.UseAllAttractors, t);
    UseRandomInitialRotation = GFInterpolate((unsigned long)UseRandomInitialRotation, (unsigned long)source.UseRandomInitialRotation, t);
    InitialRotationX = GFInterpolate(InitialRotationX, source.InitialRotationX, t);
    InitialRotationY = GFInterpolate(InitialRotationY, source.InitialRotationY, t);
    InitialRotationZ = GFInterpolate(InitialRotationZ, source.InitialRotationZ, t);
    UseRandomRotationAxis = GFInterpolate((unsigned long)UseRandomRotationAxis, (unsigned long)source.UseRandomRotationAxis, t);
    RotationAxis = GFInterpolate(RotationAxis, source.RotationAxis, t);
    RotationMinAngleSpeed = GFInterpolate(RotationMinAngleSpeed, source.RotationMinAngleSpeed, t);
    RotationMaxAngleSpeed = GFInterpolate(RotationMaxAngleSpeed, source.RotationMaxAngleSpeed, t);
    ParticleCollideWithGround = GFInterpolate((unsigned long)ParticleCollideWithGround, (unsigned long)source.ParticleCollideWithGround, t);
    ParticleCollideWithAnything = GFInterpolate(ParticleCollideWithAnything, source.ParticleCollideWithAnything, t);
    ParticleBounce = GFInterpolate(ParticleBounce, source.ParticleBounce, t);
    ParticleDieOnCollision = GFInterpolate((unsigned long)ParticleDieOnCollision, (unsigned long)source.ParticleDieOnCollision, t);
    FadeInEndInteger = GFInterpolate((unsigned long)FadeInEndInteger, (unsigned long)source.FadeInEndInteger, t);
    FadeOutBeginInteger = GFInterpolate((unsigned long)FadeOutBeginInteger, (unsigned long)source.FadeOutBeginInteger, t);
    SystemAlphaFadeEnable = GFInterpolate((unsigned long)SystemAlphaFadeEnable, (unsigned long)source.SystemAlphaFadeEnable, t);
    SystemAlphaFadeMinimum = GFInterpolate(SystemAlphaFadeMinimum, source.SystemAlphaFadeMinimum, t);
    EmissionFadeEnable = GFInterpolate((unsigned long)EmissionFadeEnable, (unsigned long)source.EmissionFadeEnable, t);
    EmissionFadeMinimum = GFInterpolate(EmissionFadeMinimum, source.EmissionFadeMinimum, t);
    RandomisePosEnable = GFInterpolate((unsigned long)RandomisePosEnable, (unsigned long)source.RandomisePosEnable, t);
    RandomisePosSpeed = GFInterpolate(RandomisePosSpeed, source.RandomisePosSpeed, t);
    RandomisePosScale = GFInterpolate(RandomisePosScale, source.RandomisePosScale, t);
    RandomisePosDistVaryEnable = GFInterpolate(RandomisePosDistVaryEnable, source.RandomisePosDistVaryEnable, t);
    RandomisePosDistVaryScale = GFInterpolate(RandomisePosDistVaryScale, source.RandomisePosDistVaryScale, t);
    StayWithEmitter = GFInterpolate((unsigned long)StayWithEmitter, (unsigned long)source.StayWithEmitter, t);
    SetOrientationFromDirection = GFInterpolate(SetOrientationFromDirection, source.SetOrientationFromDirection, t);
    SetOrientationFromGame = GFInterpolate(SetOrientationFromGame, source.SetOrientationFromGame, t);
    OrientFromGameParam = GFInterpolateUpdateParams(OrientFromGameParam, source.OrientFromGameParam, t);
    ParticleCreateDecal = GFInterpolate(ParticleCreateDecal, source.ParticleCreateDecal, t);
    ParticleCreateDecalEmitter = GFInterpolate(ParticleCreateDecalEmitter, source.ParticleCreateDecalEmitter, t);
    DecalEmitterName = GFInterpolate(DecalEmitterName, source.DecalEmitterName, t);
}
}