// NParticleEngine::CPSCEmitterGeneric::GenerateParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float creationTime) -- emit one particle: pick a position / normal on the emitter shape, scale it, choose its direction and speed, move both into world space and hand them to the system's update component
// Grade: DIFFER(4280v4276) with /d2inlT170 -- wfE8 (78 normalised hunks): (1) GetUnitFloat takes `const unsigned long&` (FITTED helper signature): every retail unsigned-conversion site (test/jge/fadd 2^32) now matches, incl. the word-load fields +0x3E / +0x48; (2) both direction transforms are CMatrix3x4::TransformVector(direction) in place (FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR, retail 0x00B119A1 / 0x00B11A2B keep old X/Y on the x87 stack), which VC7.1 calls out of line unless /d2inlT>=~150 (T140 4224 bytes, T170/T200 4280). Residue: case 2 (0x00B1118F) retail copies the GetRandomUnitVector result three times through the returned pointer (alias barriers between copies), ours copies from the slot; a few frame-slot / scheduling hunks.
// Next idea: decode each field through a by-value unsigned long parameter of an inline accessor so VC7.1 loses the bit-field range (test / jge / fadd 2^32), and spell IsNonZero as GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO on a copy H40k (1 grade): baseline 788, unchanged.
// Leaf: ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z
// Flags: /d2inlT170
// Inline-budget: /d2inlT170 -- CMatrix3x4::TransformVector (FableWin ?TransformVector@CMatrix3x4@@QBEXAAVC3DVector@@@Z, no retail out-of-line copy) is expanded at both retail sites; VC7.1 calls it at T<=140 (row not yet matched: smallest N unproven)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B10B10, 4276 bytes, thiscall, `ret 0Ch`. Private: FableWin.pdb
//   ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z.
// Body: decode the quantised emitter fields (RoundToDecimals(unit * range [- offset], 2)), switch on EmitterType (0 point, 1 random 2D
//   ring, 2 random 3D sphere, 3 spline point), radius scaled by GetRandomRadius when Solid, ChangeOrientation of direction / normal /
//   up, non-uniform scaling, direction from Random2D / Random3D / Outward / Forward (param callback) / Param (VectorParams) / custom,
//   OppositeDirection, angular perturbation, speed in [MinSpeed, MaxSpeed], emitter-position offset callback, world transforms,
//   then GetComponentOfType(PSC_TYPE_UPDATE, 0)->UpdateAddParticle (CPSCBase slot 12).
// Callees: GetRandomSeed 0x00AEAAD0, C2DVector::GetRandomUnitVector 0x00A13EB0, C3DVector::GetRandomUnitVector 0x00A145C0,
//   GetRandomPointWithinUnitSphere 0x00A146C0, CCardinalSpline::GetSplinePosition 0x00A567A0, GFFloatRandom 0x004978A0,
//   GetRandomRadius 0x00B102C0, ChangeOrientation 0x00B10360, CVectorMap::LowerBound 0x00B05890, GetSystemTransform 0x00AF2250,
//   GetInverseSystemTransform 0x00AF22C0, GetComponentOfType 0x00AF4360.
// Helpers: GFFloatToLongNear (engine/GFFloatToLongNear.h) inside RoundToDecimals -- bare `fistp dword`, proven impossible from C++
//   (same helper as the landed CPSCEmitterGeneric InterpolateTo / WriteBinary rows).
// Header debt: local CParticleSystem / CVectorMap / CParticleUpdateParameters stand-ins (no engine header carries the members used).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_EMITTER
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_RANDOMPOINTINSPHERE
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#include "engine/CMatrix3x4.h"
#include "engine/CBaseClass.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "engine/GFFloatToLongNear.h"
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
class C2DVector { public:
    float X, Y;
    static C2DVector __fastcall GetRandomUnitVector(unsigned long* seed);      // 0x00A13EB0
};
class CCardinalSpline { public:
    void GetSplinePosition(C3DVector& position, float t) const;              // 0x00A567A0
};
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return GFFloatToLongNear(scaled) / scale;
}
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class K, class V> class CVectorMap {            // sorted vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* _M_start; value_type* _M_finish; value_type* _M_end_of_storage;   // CArray<pair<K,V> > Data
    bool Compare; bool Dirty;
    const_iterator end() const { return _M_finish; }
    const_iterator LowerBound(const K& key) const;        // 0x00B05890
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || key < it->first)
            return last;
        return it;
    }
};
namespace NParticleEngine {
unsigned long* __cdecl GetRandomSeedPtr();                                   // 0x00AEAAD0
class CParticleSystem;
class CPSCBase;
template <class K, class V> class CParticleUpdateParameters;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { public:
    virtual bool GetUpdateParam(C3DVector& value, K index, float time, bool, const CParticleUpdateParameters<K, V>& params, K) = 0;
};
template <class K, class V> class CParticleUpdateParamDesc : public CBaseClass { public:
    CParticleUpdateParamCallback<K, V>* Callback;   // +0x04
    K Index;                                        // +0x08
};
template <class K, class V> class CParticleUpdateParameters : public CBaseClass { public:
    CVectorMap<K, C3DVector> VectorParams;          // +0x04
};
enum EPSCType { PSC_TYPE_UPDATE = 4 };
class CParticleSystem { public:
    unsigned char _pad[0x50];
    bool Initialised : 1, Enabled : 1, Dead : 1, NormalisedTimeEnabled : 1, UseAllAttractors : 1, Visible : 1, StayWithEmitter : 1, ScaleParticles : 1;
    void GetSystemTransform(CMatrix3x4& m) const;            // 0x00AF2250
    void GetInverseSystemTransform(CMatrix3x4& m) const;     // 0x00AF22C0
    CPSCBase* GetComponentOfType(EPSCType type, unsigned long index);   // 0x00AF4360
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void Slot2(); virtual void Slot3(); virtual void Slot4(); virtual void Slot5(); virtual void Slot6(); virtual void Slot7();
    virtual void Slot8(); virtual void Slot9(); virtual void Slot10(); virtual void Slot11();
    virtual void UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector position, C3DVector direction, float creationTime) = 0;
};
class CPSCEmitterGeneric : public CPSCBase { public:
    CParticleUpdateParamDesc<unsigned long, unsigned long> EmitterPosParamDesc;   // +0x0c
    CCountedPointer<CCardinalSpline> Spline;            // +0x18
    unsigned long EmitterPosParam;                      // +0x20
    unsigned long DirectionParamName;                   // +0x24
    long LifeTimer;                                     // +0x28
    float NextGenerateTime;                             // +0x2c
    unsigned long NoParticlesToStart : 10;              // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;      // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;                     // +0x38
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;                 // +0x3c
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;                 // +0x40
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                        // +0x44
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;             // +0x48
    unsigned long StartedTimer : 1;                     // +0x4c
    unsigned long FinishedGenerating : 1;
private:
    C3DVector ChangeOrientation(const C3DVector& v);                        // 0x00B10360
    float GetRandomRadius(unsigned long* seed) const;                       // 0x00B102C0
    void GenerateParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float creationTime);
};
static __forceinline float GetUnitFloat(const unsigned long& value, float oneOverMax) { return (float)value * oneOverMax; }
#define DECOMPRESS(value, field, oneOverMax, range, offset) { float unit = GetUnitFloat(field, oneOverMax); value = RoundToDecimals(unit * (range) offset, 2); }
static __forceinline bool IsNonZeroV(const C3DVector& v) { float s = v.X * v.X; s += v.Y * v.Y; s += v.Z * v.Z; return DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO < s; }
void CPSCEmitterGeneric::GenerateParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float creationTime)
{
    float emitterTimelineSecs; DECOMPRESS(emitterTimelineSecs, EmitterTimelineSecs, 1.0f / 32767.0f, 300.0f, );
    float emitterSize; DECOMPRESS(emitterSize, EmitterSize, 1.0f / 1023.0f, 10.0f, );
    float radialBias; DECOMPRESS(radialBias, RadialBias, 1.0f / 1023.0f, 10.0f, - 5.0f);
    float emitterLifeSecs; DECOMPRESS(emitterLifeSecs, EmitterLifeSecs, 1.0f / 32767.0f, 300.0f, );
    float emitterStartTime; DECOMPRESS(emitterStartTime, EmitterStartTime, 1.0f / 32767.0f, 300.0f, );
    float particlesPerSecond; DECOMPRESS(particlesPerSecond, ParticlesPerSecond, 1.0f / 16383.0f, 100.0f, );
    float minSpeed; DECOMPRESS(minSpeed, MinSpeed, 1.0f / 1023.0f, 10.0f, );
    float maxSpeed; DECOMPRESS(maxSpeed, MaxSpeed, 1.0f / 1023.0f, 10.0f, );
    C3DVector customDirection;
    DECOMPRESS(customDirection.Z, CustomDirectionZ, 1.0f / 255.0f, 2.0f, - 1.0f);
    DECOMPRESS(customDirection.Y, CustomDirectionY, 1.0f / 255.0f, 2.0f, - 1.0f);
    DECOMPRESS(customDirection.X, CustomDirectionX, 1.0f / 255.0f, 2.0f, - 1.0f);
    C3DVector scaling;
    DECOMPRESS(scaling.Z, NonUniformScalingZ, 1.0f / 2047.0f, 20.0f, - 10.0f);
    DECOMPRESS(scaling.Y, NonUniformScalingY, 1.0f / 2047.0f, 20.0f, - 10.0f);
    DECOMPRESS(scaling.X, NonUniformScalingX, 1.0f / 2047.0f, 20.0f, - 10.0f);
    C3DVector direction(0.0f, 0.0f, 0.0f);
    unsigned long* seed = GetRandomSeedPtr();
    float radius = emitterSize * 0.5f;
    C3DVector position;
    C3DVector normal;
    C3DVector up(0.0f, 0.0f, 1.0f);
    switch (EmitterType) {
    case 0:
        position.Clear();
        normal.Clear();
        break;
    case 1: {
        C2DVector random = C2DVector::GetRandomUnitVector(seed);
        normal = C3DVector(random.X, random.Y, 0.0f);
        position = normal;
        float r = Solid ? GetRandomRadius(seed) * radius : radius;
        position *= r;
        direction = ChangeOrientation(direction);
        normal = ChangeOrientation(normal);
        up = ChangeOrientation(up);
        break;
    }
    case 2: {
        C3DVector random = C3DVector::GetRandomUnitVector(seed);
        position = random;
        normal = random;
        up = random;
        float r = Solid ? GetRandomRadius(seed) * radius : radius;
        position *= r;
        direction = ChangeOrientation(direction);
        normal = ChangeOrientation(normal);
        up = ChangeOrientation(up);
        break;
    }
    case 3:
        Spline->GetSplinePosition(position, GFFloatRandom(1.0f, seed));
        normal = position;
        up = position;
        direction = ChangeOrientation(direction);
        normal = ChangeOrientation(normal);
        up = ChangeOrientation(up);
        break;
    }
    bool hasParamDirection = false;
    position = C3DVector(scaling.X * position.X, position.Y * scaling.Y, position.Z * scaling.Z);
    normal = C3DVector(scaling.X * normal.X, normal.Y * scaling.Y, normal.Z * scaling.Z);
    if (IsNonZeroV(normal))
        normal.Normalise();
    up = C3DVector(up.X * scaling.X, up.Y * scaling.Y, up.Z * scaling.Z);
    if (UseRandom2DDirection) {
        direction = C3DVector::GetRandomUnitVector(seed);
        float dot = up.X * direction.X + up.Z * direction.Z + up.Y * direction.Y;
        direction = direction - up * dot;
        if (IsNonZeroV(direction))
            direction.Normalise();
    } else if (UseRandom3DDirection)
        direction = C3DVector::GetRandomUnitVector(seed);
    else if (UseOutwardDirection)
        direction = normal;
    else if (UseForwardDirection) {
        if (EmitterPosParamDesc.Callback == 0 || !EmitterPosParamDesc.Callback->GetUpdateParam(direction, EmitterPosParamDesc.Index, creationTime, true, params, 1))
            direction = C3DVector(0.0f, 0.0f, 0.0f);
    } else {
        const std::pair<unsigned long, C3DVector>* it;
        if (UseParamDirection && (it = params.VectorParams.Find(DirectionParamName)) != params.VectorParams.end()) {
            direction = it->second;
            hasParamDirection = true;
        } else
            direction = customDirection;
    }
    if (OppositeDirection)
        direction = -direction;
    if (AngularPerturbationInteger != 0) {
        float length = direction.GetMagnitude();
        float perturbation = (float)AngularPerturbationInteger * 0.001f;
        C3DVector random = C3DVector::GetRandomPointWithinUnitSphere(seed);
        float dot = random.X * direction.X + random.Z * direction.Z + random.Y * direction.Y;
        random = random - direction * dot;
        direction += random * perturbation;
        if (IsNonZeroV(direction)) {
            direction.Normalise();
            direction *= length;
        }
    }
    float speedRange = maxSpeed - minSpeed;
    if (!(speedRange > 0.0f))
        speedRange = 0.0f;
    direction *= GFFloatRandom(speedRange, seed) + minSpeed;
    C3DVector offset;
    if (EmitterPosParamDesc.Callback != 0 && EmitterPosParamDesc.Callback->GetUpdateParam(offset, EmitterPosParamDesc.Index, creationTime, false, params, 1))
        position += offset;
    if (hasParamDirection) {
        if (system.StayWithEmitter) {
            CMatrix3x4 transform;
            system.GetInverseSystemTransform(transform);
            transform.TransformVector(direction);
        }
    } else if (!system.StayWithEmitter) {
        CMatrix3x4 transform;
        system.GetSystemTransform(transform);
        transform.TransformVector(direction);
    }
    if (!system.StayWithEmitter) {
        CMatrix3x4 transform;
        system.GetSystemTransform(transform);
        transform.TransformPoint(position);
    }
    CPSCBase* update = system.GetComponentOfType(PSC_TYPE_UPDATE, 0);
    if (update != 0)
        update->UpdateAddParticle(system, params, position, direction, creationTime);
}
}
