// NParticleEngine::CPSCUpdateNormal::UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector position, C3DVector direction, float creationTime) -- vtable slot 12: spawn one particle (emission fade gate, lifetime, position history, creation time, initial orientation from the fixed / random rotation or from the direction / game vector, rotation axis and speed, flicker bytes)
// Grade: DIFFER(2191v2191) -- 349 masked diff lines: register allocation (retail keeps the random seed in ebp, ours spills it and CSEs the constant 1 into ecx), the frame slots of the C3DVector / CQuaternion temporaries (retail 0x48 above the orientation at 0x38, ours below), x87 stack order in the forward ^ up cross product. Next idea: find the shape that keeps the seed in a callee-saved register (e.g. the seed read through GetRandomSeed() at each use, or an inline CParticle::Initialise that takes the seed).
// Leaf: ?UpdateAddParticle@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0CAD0, 2191 bytes, thiscall, `ret 24h`.
// Identity: FableWin.pdb ?UpdateAddParticle@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z;
//   slot 12 of the CPSCUpdateNormal vtable 0x0129FC1C (CPSCBase slot 12 0x00AF2A00 is the empty base hook; ret 24h = 4+4+12+12+4 bytes).
// Callees: NParticleEngine::GetRandomSeed 0x00AEAAD0, CPSCUpdateNormal::GetCurrentFadeValue 0x00B0A750 (landed), GFFloatRandom 0x004978A0,
//   CParticleSystem::AllocateParticle 0x00AF5010, NParticleEngine::GetTickRate 0x00AEA9F0, memmove (0x00BFEAE6 thunk), __ftol2,
//   C3DVector::GetRandomUnitVector 0x00A145C0, CQuaternion::SetFromAxisAngle 0x00A88D10, CQuaternion::operator* 0x00A88B60,
//   CParticleUpdateParameters<K,V>::GetVectorParam 0x00B059F0, CParticleSystem::PeekEmitterVelocity 0x00AF3DB0,
//   C3DVector::AccurateNormalise 0x00A14440, CRightHandedSet::MakePerpendicular 0x00A22E10, CQuaternion::SetFromRotationMatrix 0x00A88DE0,
//   GFROR13 0x00497890 (inside the inline GFRandom).
// Helpers: GFFastFloatToLong (engine/GFFastFloatToLong.h) in the inline CParticle::SetCreationTime / SetAngleChange (out-of-line copies
//   0x00B0A4A0 / 0x00B0A420 landed): retail 0x00B0CBD0 / 0x00B0D310 `fistp dword ptr [edx]`, a bare dword fistp no VC7.1 C++ form emits.
// Layout: CParticle (Ego_r.pdb): Orientation +0x04, OrientationChange +0x14, Angle +0x24, AngleChange +0x28, PositionBuffer[4] +0x29,
//   Velocity +0x59, Acceleration +0x65 (1-aligned C3DVector: the vectors under pack(1)), Lifetimer +0x72, CreationTime +0x74, MaxLife +0x76,
//   FlickerIndex / FlickerTimeOffset +0x78 / +0x79, Active +0x7a, ClosestAttractorDistance +0x7c. CPSCUpdateNormal as the landed ctor.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CPersistContext.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#include "engine/CParticleSystem.h"
#include "engine/GFFastFloatToLong.h"
#include <math.h>
#include <string.h>
#include <float.h>
#pragma intrinsic(fabs)
long __fastcall GFROR13(unsigned long value);                                    // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);                // 0x004978A0
inline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % range;
}
class CQuaternion { public:
    float X, Y, Z, W;
    CQuaternion() { }
    CQuaternion operator*(const CQuaternion& q) const;                           // 0x00A88B60
    void SetFromAxisAngle(const C3DVector& axis, float angle);                   // 0x00A88D10
    void SetFromRotationMatrix(const CMatrix3x4& matrix);                        // 0x00A88DE0
    void SetIdentity() { X = 0.0f; Y = 0.0f; Z = 0.0f; W = 1.0f; }
};
namespace NParticleEngine {
long __fastcall GetTickRate();                                                   // 0x00AEA9F0
unsigned long& __fastcall GetRandomSeed();                                       // 0x00AEAAD0
template <class K, class V> class CParticleUpdateParameters { public:
    bool GetVectorParam(C3DVector& result, const K& key) const;                  // 0x00B059F0
};
class CParticle : public CBaseClassNonCopyable { public:
    CQuaternion   Orientation;                  // +0x04
    CQuaternion   OrientationChange;            // +0x14
    float         Angle;                        // +0x24
    signed char   AngleChange;                  // +0x28
#pragma pack(push, 1)
    C3DVector     PositionBuffer[4];            // +0x29
    C3DVector     Velocity;                     // +0x59
    C3DVector     Acceleration;                 // +0x65
#pragma pack(pop)
    short         Lifetimer;                    // +0x72
    unsigned char CreationTime;                 // +0x74
    short         MaxLife;                      // +0x76
    unsigned char FlickerIndex;                 // +0x78
    unsigned char FlickerTimeOffset;            // +0x79
    bool          Active;                       // +0x7a
    float         ClosestAttractorDistance;     // +0x7c
    void SetClosestAttractorDistance(float distance) { ClosestAttractorDistance = distance; }
    void SetPosition(const C3DVector& position) { memmove(&PositionBuffer[1], &PositionBuffer[0], sizeof(C3DVector) * 3); PositionBuffer[0] = position; }
    void ClearAcceleration() { Acceleration.X = Acceleration.Y = Acceleration.Z = 0.0f; }
    void SetMaxLife(short maxLife) { MaxLife = maxLife; }
    void SetCreationTime(float creationTime) { long time; time = GFFastFloatToLong(creationTime * 255.0f); CreationTime = (unsigned char)time; }
    void SetLifetimer(short lifetimer) { Lifetimer = lifetimer; }
    __forceinline void Activate() { Active = true; PositionBuffer[1] = PositionBuffer[0]; PositionBuffer[2] = PositionBuffer[0]; PositionBuffer[3] = PositionBuffer[0]; }
    void SetVelocity(const C3DVector& velocity) { Velocity = velocity; }
    void SetOrientation(const CQuaternion& orientation) { Orientation = orientation; }
    void SetOrientationChange(const CQuaternion& change) { OrientationChange = change; }
    void SetAngle(float angle) { Angle = angle; }
    void SetAngleChange(float angleChange) { long change; change = GFFastFloatToLong(angleChange * 63.5f); AngleChange = (signed char)change; }
    void SetFlickerIndex(unsigned char index) { FlickerIndex = index; }
    void SetFlickerTimeOffset(unsigned char offset) { FlickerTimeOffset = offset; }
};
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void UpdateAddParticle(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, C3DVector, C3DVector, float) = 0;
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
    virtual void UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                   C3DVector position, C3DVector direction, float creationTime);
protected:
    float GetCurrentFadeValue() const;                  // 0x00B0A750
};
void CPSCUpdateNormal::UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                         C3DVector position, C3DVector direction, float creationTime)
{
    unsigned long& seed = GetRandomSeed();
    if (EmissionFadeEnable) {
        float fade = GetCurrentFadeValue();
        float threshold = EmissionFadeMinimum + (1.0f - EmissionFadeMinimum) * fade;
        if (GFFloatRandom(1.0f, &seed) > threshold)
            return;
    }
    CParticle* particle = system.AllocateParticle();
    if (particle == 0)
        return;
    float tickRate = (float)GetTickRate();
    particle->SetClosestAttractorDistance(FLT_MAX);
    particle->SetPosition(position);
    particle->ClearAcceleration();
    short maxLife = (short)(tickRate * ParticleLifeSecs);
    if (maxLife == 0)
        maxLife = 1;
    particle->SetMaxLife(maxLife);
    particle->SetLifetimer(-1);
    particle->SetCreationTime(creationTime);
    particle->Activate();
    particle->SetVelocity(direction);

    CQuaternion orientation;
    float angle = 0.0f;
    if (!SetOrientationFromDirection && !SetOrientationFromGame) {
        if (UseRandomInitialRotation) {
            C3DVector axis = C3DVector::GetRandomUnitVector(&seed);
            angle = GFFloatRandom(1.0f, &seed);
            orientation.SetFromAxisAngle(axis, angle);
        } else {
            angle = InitialRotationX;
            CQuaternion rotationX;
            rotationX.SetFromAxisAngle(C3DVector(1.0f, 0.0f, 0.0f), angle);
            CQuaternion rotationY;
            rotationY.SetFromAxisAngle(C3DVector(0.0f, 1.0f, 0.0f), InitialRotationY);
            CQuaternion rotationZ;
            rotationZ.SetFromAxisAngle(C3DVector(0.0f, 0.0f, 1.0f), InitialRotationZ);
            orientation = rotationX * rotationY * rotationZ;
        }
    } else {
        C3DVector forward(0.0f, 0.0f, 0.0f);
        if (SetOrientationFromGame) {
            params.GetVectorParam(forward, OrientFromGameParam);
        } else {
            if (direction.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
                forward = direction;
            if (StayWithEmitter)
                forward += system.PeekEmitterVelocity();
        }
        if (forward.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            forward.AccurateNormalise();
            CRightHandedSet set;
            set.SetForward(forward.AccurateNormalise());
            float upX = 0.0f;
            float upY = 0.0f;
            float upZ = 1.0f;
            if (fabs(forward.Z) > 0.99f) {
                if (fabs(forward.X) > fabs(forward.Y)) {
                    upX = 0.0f;
                    upY = 1.0f;
                    upZ = 0.0f;
                } else {
                    upX = 1.0f;
                    upY = 0.0f;
                    upZ = 0.0f;
                }
            }
            C3DVector right(forward.Y * upZ - forward.Z * upY, forward.Z * upX - forward.X * upZ, forward.X * upY - forward.Y * upX);
            set.SetUp(right ^ forward);
            set.MakePerpendicular();
            CMatrix3x4 matrix;
            matrix.Set(set.GetRight(), set.GetForward(), set.GetUp(), C3DVector(0.0f, 0.0f, 0.0f));
            orientation.SetFromRotationMatrix(matrix);
        } else {
            orientation.SetIdentity();
        }
    }
    float random = GFFloatRandom(1.0f, &seed);
    float angleSpeed = RotationMinAngleSpeed + (RotationMaxAngleSpeed - RotationMinAngleSpeed) * random;
    C3DVector rotationAxis;
    CQuaternion orientationChange;
    if (SetOrientationFromDirection || SetOrientationFromGame) {
        orientationChange.SetIdentity();
        float lengthSquared = orientation.X * orientation.X + orientation.W * orientation.W + orientation.Z * orientation.Z + orientation.Y * orientation.Y;
        float scale = lengthSquared == 0.0f ? 1.0f : 2.0f / lengthSquared;
        float xs = orientation.X * scale;
        float zs = orientation.Z * scale;
        float ys = orientation.Y * scale;
        rotationAxis = C3DVector(orientation.X * ys + orientation.W * zs, 1.0f - (orientation.Z * zs + orientation.X * xs), orientation.Y * zs - orientation.W * xs);
    }
    if (UseRandomRotationAxis) {
        rotationAxis = C3DVector::GetRandomUnitVector(&seed);
        orientationChange.SetFromAxisAngle(rotationAxis, angleSpeed);
    } else if (RotationAxis.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        rotationAxis = RotationAxis;
        rotationAxis.Normalise();
        orientationChange.SetFromAxisAngle(rotationAxis, angleSpeed);
    } else {
        orientationChange.SetIdentity();
    }
    particle->SetOrientation(orientation);
    particle->SetOrientationChange(orientationChange);
    particle->SetAngle(angle);
    particle->SetAngleChange(angleSpeed);
    particle->SetFlickerIndex((unsigned char)GFRandom(256, &seed));
    particle->SetFlickerTimeOffset((unsigned char)GFRandom(256, &seed));
}
}
