// NParticleEngine::CPSCUpdateNormal::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& alive) -- vtable slot 9: advance the system life / fade, then move, collide, decal and orient every particle, then update the decal emitters
// Grade: DIFFER(5601v5602) -- 418 masked lines (102 structural): frame 0x2d0 v 0x2dc (one 12-byte temporary short), x87 operand order in the perpendicular cross products (retail `fld st2; fmul mem`), the CreationTime fraction kept in a register (retail fstp + memory fmul), TransformPosIfLocal Z row order, slot scheduling of a few copies. Next idea: give the decal / orientation perpendicular its own inline (a CRightHandedSet or C3DVector member with an explicit reference vector) and find the 12-byte temporary (gravity / wind products as named C3DVector locals, or ToMatrix through a temporary CMatrix3x4).
// Leaf: ?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0DC20, 5602 bytes, thiscall, `ret 0Ch`. FableWin.pdb
//   ?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z, slot 9 of the
//   CPSCUpdateNormal vtable (CPSCBase slot 9 = Update; the landed CPSCAttractor::Update 0x00B179A0 has the same signature).
// Body: life time = LifeTimer * GetOneOverTickRate(); UseSystemLifeSecs ends the system (alive = false) or sets the clamped
//   normalised time and the alpha fade; air resistance pow(1 - AirResistance, frameTime); gravity / wind force from the
//   CAEnvironmentInteraction (rotated for 2D emitters); acceleration from the param or ParticleAcceleration; per active particle:
//   life-out delete, Perlin random offset (CRandomAcceleration), integrate (first frame scaled by 1 - CreationTime / 255), ground
//   collision (decal, decal emitter, die, bounce / reflect through the quaternion matrix), orientation update (spin, or from the
//   direction / game vector through CRightHandedSet + SetFromRotationMatrix), angle, position history, velocity, lifetimer;
//   then the decal-emitter list update / erase and ++LifeTimer.
// Callees (FableWin.pdb names / reloc bindings): GetOneOverTickRate 0x00AEAA00, GetEnvironmentInteraction 0x00AEAB10,
//   GetCurrentFadeValue 0x00B0A750, _CIpow, CMatrix3x4::SetFromVectorAndAngle 0x00A55F90, GetVectorParam 0x00B059F0, GetTickRate
//   0x00AEA9F0, GetRandomSeed 0x00AEAAD0, CParticleSystem::DeleteParticle 0x00AF5330, CRandomAcceleration::operator new 0x00AF8680 /
//   ctor 0x00B0A210, GetParticleEmitterManager 0x00AEAAF0, NFractal::CPerlinNoise::GetGaussian2DNoise 0x00F36540,
//   GetSystemTransform 0x00AF2250, AllocateDecal 0x00AF5380, CRightHandedSet::MakePerpendicular 0x00A22E10, GFROR13 0x00497890,
//   GetParticleEmitterDatabase 0x00AEAAE0, GetEmitterTemplateHandleFromName 0x00AE85B0, CRightHandedSet(up, forward) 0x00487050,
//   CreateDecalEmitter 0x00AEABE0, CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) 0x00655318, list push_back 0x00B0F920,
//   ~CBaseIntelligentPointer 0x00A01C10, GetInverseSystemTransform 0x00AF22C0, CQuaternion::SetFromAxisAngle 0x00A88D10 /
//   operator*= 0x00A88C10 / SetFromRotationMatrix 0x00A88DE0, PeekEmitterVelocity 0x00AF3DB0, AccurateNormalise 0x00A14440, memmove,
//   GetPItem 0x00A01B50, CParticleEmitter::Update 0x00AEE9F0, free.
// Helpers: none (no asm-only instruction in the body).
// Header debt: everything is a local stand-in (CParticleSystem list members, CParticle + CRandomAcceleration, CDecal, CQuaternion,
//   CMatrix3x4 TransformVector / SetFromVectorAndAngle, CRightHandedSet two-vector ctor, the environment / emitter-manager /
//   database classes, CIntelligentPointer out-of-line ctor, STLport list::erase). FableWin.pdb names the inline helpers used:
//   CParticleSystem::TransformVectorIfLocal / InverseTransformPosIfLocal / SetNormalisedTime(bool, float) / SetGlobalAlpha,
//   CParticleEmitterManager::GetEnableDecals, CQuaternion::ToMatrix, CParticle::UpdateOrientation / UpdateAngle / IncrementLifetimer /
//   HaveRandomAccelerationData / CreateRandomAccelerationData / GetOldRandomOffset / SetOldRandomOffset, the CDecal setters.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CBaseClass.h"
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(fabs)
long __fastcall GFROR13(unsigned long value);                                    // 0x00497890
__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % range;
}
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CMatrix3x4 { public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    CMatrix3x4() { }
    void SetFromVectorAndAngle(const C3DVector& axis, float angle);              // 0x00A55F90
    __forceinline void Set(const C3DVector& x, const C3DVector& y, const C3DVector& z, const C3DVector& t) {
        E11 = x.X; E12 = x.Y; E13 = x.Z; E21 = y.X; E22 = y.Y; E23 = y.Z; E31 = z.X; E32 = z.Y; E33 = z.Z; E41 = t.X; E42 = t.Y; E43 = t.Z;
    }
    __forceinline void TransformPoint(C3DVector& point) const {
        const float xx = point.X; const float yy = point.Y; const float zz = point.Z;
        point.X = E11 * xx + (E21 * yy + E31 * zz) + E41;
        point.Y = E12 * xx + (E22 * yy + E32 * zz) + E42;
        point.Z = E13 * xx + (E23 * yy + E33 * zz) + E43;
    }
    __forceinline void TransformVectorLocal(C3DVector& vector) const {
        const float xx = vector.X; const float yy = vector.Y; const float zz = vector.Z;
        vector.X = E11 * xx + (E21 * yy + E31 * zz);
        vector.Y = E12 * xx;
        vector.Y += E32 * zz;
        vector.Y += E22 * yy;
        vector.Z = E23 * yy;
        vector.Z += E13 * xx;
        vector.Z += E33 * zz;
    }
    __forceinline void TransformVector(C3DVector& vector) const {
        const float xx = vector.X; const float yy = vector.Y; const float zz = vector.Z;
        vector.X = E11 * xx + (E21 * yy + E31 * zz);
        vector.Y = E12 * xx;
        vector.Y += E32 * zz;
        vector.Y += E22 * yy;
        vector.Z = E13 * xx;
        vector.Z += E23 * yy;
        vector.Z += E33 * zz;
    }
};
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { }
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);              // 0x00487050
    void SetUp(const C3DVector& up) { Up = up; }
    void SetForward(const C3DVector& forward) { Forward = forward; }
    const C3DVector& GetUp() const { return Up; }
    const C3DVector& GetForward() const { return Forward; }
    C3DVector GetRight() const { return Forward ^ Up; }
    void MakePerpendicular();                                                    // 0x00A22E10
};
class CQuaternion { public:
    float X, Y, Z, W;
    CQuaternion() { }
    const CQuaternion& operator*=(const CQuaternion& q);                         // 0x00A88C10
    void SetFromAxisAngle(const C3DVector& axis, float angle);                   // 0x00A88D10
    void SetFromRotationMatrix(const CMatrix3x4& matrix);                        // 0x00A88DE0
    void SetIdentity() { X = 0.0f; Y = 0.0f; Z = 0.0f; W = 1.0f; }
    __forceinline void ToMatrix(CMatrix3x4& m, const C3DVector& translation) const {
        float norm = X * X + W * W + Z * Z + Y * Y;
        float s = (norm != 0.0f) ? 2.0f / norm : 1.0f;
        float xs = X * s, ys = Y * s, zs = Z * s;
        float wx = W * xs, wy = W * ys, wz = W * zs;
        float xx = X * xs, xy = X * ys, xz = X * zs;
        float yy = Y * ys, yz = Y * zs, zz = Z * zs;
        m.E11 = 1.0f - (yy + zz); m.E12 = xy - wz; m.E13 = xz + wy;
        m.E21 = xy + wz; m.E22 = 1.0f - (xx + zz); m.E23 = yz - wx;
        m.E31 = xz - wy; m.E32 = yz + wx; m.E33 = 1.0f - (xx + yy);
        m.E41 = translation.X; m.E42 = translation.Y; m.E43 = translation.Z;
    }
};
namespace NFractal {
class CPerlinNoise { public:
    float GetGaussian2DNoise(const C2DVector& position) const;                   // 0x00F36540
};
}
// The intelligent pointer and list stand-ins (header debt: engine/CIntelligentPointer.h declares CIntelligentPointer(T*) inline,
// retail calls it out of line at 0x00655318; the shared STLport list has no erase()).
class CBaseObject;
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                                          // 0x00A01C10
    CBaseObject* GetPItem() const;                                               // 0x00A01B50
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* pointer);                                             // 0x00655318
    T* Get() const { return (T*)GetPItem(); }
};
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : public _List_node_base { T _M_data; };
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node_base* node) : _M_node(node) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> class list { public:
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    void push_back(const T& value);                                              // 0x00B0F920 (CIntelligentPointer<CParticleEmitter>)
    iterator erase(iterator position) {
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node_base* next = position._M_node->_M_next;
        _List_node<T>* node = (_List_node<T>*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        node->_M_data.~T();
        free(node);
        return iterator(next);
    }
};
}
namespace NParticleEngine {
float __fastcall GetOneOverTickRate();                                           // 0x00AEAA00
long __fastcall GetTickRate();                                                   // 0x00AEA9F0
unsigned long& __fastcall GetRandomSeed();                                       // 0x00AEAAD0
class CParticleEmitter { public:
    unsigned char _pad[0x40];
    unsigned short Priority : 3, IsScreenDisplacement : 1, ReadZBuffer : 1, Dead : 1, ParticleCreationDisabled : 1, ContinuousEmitter : 1,
                   Initialised : 1, WaitingToSleep : 1, Sleeping : 1, Emitter2D : 1, Emitter2DLayer : 4;
    bool IsDead() const { return Dead; }
    bool IsEmitter2D() const { return Emitter2D; }
    void Update();                                                               // 0x00AEE9F0
};
class CAEnvironmentInteraction { public:
    virtual ~CAEnvironmentInteraction();
    virtual bool IsParticleCollidingAt(const C3DVector& position, const C3DVector& velocity, bool, C3DVector& collisionPosition,
                                       C3DVector& collisionNormal, bool& landscape) const = 0;
    virtual bool IsWindActive() const;
    virtual C3DVector GetWindVelocity() const = 0;
    virtual C3DVector GetGravityVector() const;
};
CAEnvironmentInteraction& __fastcall GetEnvironmentInteraction();               // 0x00AEAB10
class CParticleEmitterDatabase { public:
    long GetEmitterTemplateHandleFromName(const CCharString& name) const;        // 0x00AE85B0
};
CParticleEmitterDatabase& __fastcall GetParticleEmitterDatabase();              // 0x00AEAAE0
class CParticleEmitterManager { public:
    unsigned char ActiveEmitters[8];
    NFractal::CPerlinNoise PerlinNoise;                                          // +0x08
    unsigned char _pad[0x1010 - 0x9];
    bool Cleared : 1, EnableDecals : 1;                                          // +0x1010
    bool GetEnableDecals() const { return EnableDecals; }
    CParticleEmitter* CreateDecalEmitter(long handle, const C3DVector& position, bool, const CRightHandedSet& set);   // 0x00AEABE0
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();                // 0x00AEAAF0
#pragma pack(push, 1)
class CRandomAcceleration { public:
    static void* __fastcall operator new(size_t size);                           // 0x00AF8680
    static void __fastcall operator delete(void* p);
    CRandomAcceleration(unsigned long& seed);                                    // 0x00B0A210
    virtual ~CRandomAcceleration();
    unsigned char TimeOffsetX, TimeOffsetY, TimeOffsetZ;                         // +0x04
    unsigned char IndexX, IndexY, IndexZ;                                        // +0x07
    C3DVector OldOffset;                                                         // +0x0a
    unsigned char _pad_0x16[2];
};
#pragma pack(pop)
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
    CRandomAcceleration* RandomAcceleration;    // +0x80 boost::scoped_ptr<CRandomAcceleration>
    const C3DVector& GetPosition() const { return PositionBuffer[0]; }
    void SetPosition(const C3DVector& position) { memmove(&PositionBuffer[1], &PositionBuffer[0], sizeof(C3DVector) * 3); PositionBuffer[0] = position; }
    const C3DVector& GetVelocity() const { return Velocity; }
    void SetVelocity(const C3DVector& velocity) { Velocity = velocity; }
    const C3DVector& GetAcceleration() const { return Acceleration; }
    void ClearAcceleration() { Acceleration.X = Acceleration.Y = Acceleration.Z = 0.0f; }
    void SetOrientation(const CQuaternion& orientation) { Orientation = orientation; }
    void SetOrientationChange(const CQuaternion& change) { OrientationChange = change; }
    void UpdateOrientation() { Orientation *= OrientationChange; }
    void UpdateAngle() { Angle += AngleChange * (1.0f / 63.5f); }
    void IncrementLifetimer() { ++Lifetimer; }
    short GetLifetimer() const { return Lifetimer; }
    float GetCreationTime() const { return CreationTime * (1.0f / 255.0f); }
    bool HaveRandomAccelerationData() const { return RandomAcceleration != 0; }
    void CreateRandomAccelerationData(unsigned long& seed) {
        CRandomAcceleration* created = new CRandomAcceleration(seed);
        CRandomAcceleration* old = RandomAcceleration;
        RandomAcceleration = created;
        if (old != 0)
            delete old;
    }
    unsigned char GetRandomAccelerationTimeOffsetX() const { return RandomAcceleration->TimeOffsetX; }
    unsigned char GetRandomAccelerationTimeOffsetY() const { return RandomAcceleration->TimeOffsetY; }
    unsigned char GetRandomAccelerationTimeOffsetZ() const { return RandomAcceleration->TimeOffsetZ; }
    unsigned char GetRandomAccelerationIndexX() const { return RandomAcceleration->IndexX; }
    unsigned char GetRandomAccelerationIndexY() const { return RandomAcceleration->IndexY; }
    unsigned char GetRandomAccelerationIndexZ() const { return RandomAcceleration->IndexZ; }
    C3DVector GetOldRandomOffset() const { return RandomAcceleration->OldOffset; }
    void SetOldRandomOffset(const C3DVector& offset) { RandomAcceleration->OldOffset = offset; }
};
class CDecal { public:
    virtual void Delete();
    C3DVector Position;                         // +0x04
    CRightHandedSet RHSet;                      // +0x10
    long Lifetimer;                             // +0x28
    float CreationTime;                         // +0x2c
    long MaxLife;                               // +0x30
    unsigned char FlickerIndex;                 // +0x34
    unsigned char FlickerTimeOffset;            // +0x35
    float SizeFactor[2];                        // +0x38
    float AlphaFactor[2];                       // +0x40
    float GraphicFrame;                         // +0x48
    long UpdateTimer;                           // +0x4c
    float MinLife;                              // +0x50
    float MinSize;                              // +0x54
    bool Active : 1, SizeCollisionThisFrame : 1, AlphaCollisionThisFrame : 1, LandscapeOnlyDecal : 1;   // +0x58
    void SetPosition(const C3DVector& position) { Position = position; }
    void SetRightHandedSet(const CRightHandedSet& set) { RHSet = set; }
    void SetLandscapeOnlyDecal(bool landscape) { LandscapeOnlyDecal = landscape; }
    void SetLifetimer(long lifetimer) { Lifetimer = lifetimer; }
    void SetMaxLife(long maxLife) { MaxLife = maxLife; }
    void SetFlickerIndex(unsigned char index) { FlickerIndex = index; }
    void SetFlickerTimeOffset(unsigned char offset) { FlickerTimeOffset = offset; }
    void Activate() { Active = true; }
};
template <class K, class V> class CParticleUpdateParameters { public:
    bool GetVectorParam(C3DVector& result, const K& key) const;                  // 0x00B059F0
};
class CParticleSystem { public:
    const CParticleEmitter* PParentEmitter;     // +0x04
    void* PEngine;                              // +0x08
    unsigned char Components[16];               // +0x0c
    std::list<CParticle*> Particles;            // +0x1c
    std::list<CDecal*> Decals;                  // +0x20
    unsigned char UpdateParameters[36];         // +0x24
    long NoActiveParticles, NoActiveDecals;     // +0x48
    bool Initialised : 1, Enabled : 1, Dead : 1, NormalisedTimeEnabled : 1, UseAllAttractors : 1, Visible : 1, StayWithEmitter : 1, ScaleParticles : 1;
    float GlobalAlpha, FadeAlpha, NormalisedTime;   // +0x54
    virtual ~CParticleSystem();
    const CParticleEmitter& GetParentEmitter() const { return *PParentEmitter; }
    void GetSystemTransform(CMatrix3x4& m) const;                                // 0x00AF2250
    void GetInverseSystemTransform(CMatrix3x4& m) const;                         // 0x00AF22C0
    std::list<CParticle*>::iterator DeleteParticle(std::list<CParticle*>::iterator& it);   // 0x00AF5330
    CDecal* AllocateDecal();                                                     // 0x00AF5380
    const C3DVector& PeekEmitterVelocity() const;                                // 0x00AF3DB0
    void SetGlobalAlpha(float alpha) { GlobalAlpha = alpha; }
    void SetNormalisedTime(bool enabled, float time) { NormalisedTimeEnabled = enabled; NormalisedTime = time; }
    __forceinline void TransformVectorIfLocal(C3DVector& vector) const { if (StayWithEmitter) { CMatrix3x4 transform; GetSystemTransform(transform); transform.TransformVectorLocal(vector); } }
    __forceinline void TransformPosIfLocal(C3DVector& position) const { if (StayWithEmitter) { CMatrix3x4 transform; GetSystemTransform(transform); transform.TransformPoint(position); } }
    __forceinline void InverseTransformPosIfLocal(C3DVector& position) const { if (StayWithEmitter) { CMatrix3x4 transform; GetInverseSystemTransform(transform); transform.TransformPoint(position); } }
};
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4(); virtual void Slot5(); virtual void Slot6();
    virtual void Slot7(); virtual void Slot8();
    virtual void Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& alive) = 0;
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
    std::list<CIntelligentPointer<CParticleEmitter> > DecalEmitterList;   // +0x9c
    virtual void Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& alive);
protected:
    float GetCurrentFadeValue() const;                  // 0x00B0A750
};
// A vector perpendicular to axis, in the plane of axis and world Z (world X / Y when axis is nearly vertical).
static __forceinline C3DVector GetPerpendicularForDecal(const C3DVector& axis)
{
    float x = 0.0f, y = 0.0f, z = 1.0f;
    if (fabs(axis.Z) > 0.99f) {
        if (fabs(axis.X) >= fabs(axis.Y)) {
            x = 0.0f;
            y = 1.0f;
        } else {
            x = 1.0f;
            y = 0.0f;
        }
        z = 0.0f;
    }
    C3DVector reference(x, y, z);
    C3DVector side = axis ^ reference;
    return side ^ axis;
}
static __forceinline C3DVector GetPerpendicularForOrientation(const C3DVector& axis)
{
    float x = 0.0f, y = 0.0f, z = 1.0f;
    if (fabs(axis.Z) > 0.99f) {
        if (fabs(axis.X) > fabs(axis.Y)) {
            x = 0.0f;
            y = 1.0f;
        } else {
            x = 1.0f;
            y = 0.0f;
        }
        z = 0.0f;
    }
    C3DVector reference(x, y, z);
    C3DVector side = axis ^ reference;
    return side ^ axis;
}
void CPSCUpdateNormal::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& alive)
{
    float frameTime = GetOneOverTickRate();
    CAEnvironmentInteraction& environment = GetEnvironmentInteraction();
    float lifeTime = LifeTimer * frameTime;
    float normalisedTime = 0.0f;
    if (UseSystemLifeSecs) {
        if (lifeTime >= SystemLifeSecs) {
            alive = false;
            return;
        }
        if (SystemLifeSecs > 0.0f) {
            normalisedTime = lifeTime / SystemLifeSecs;
            if (normalisedTime < 0.0f)
                normalisedTime = 0.0f;
            else if (normalisedTime > 1.0f)
                normalisedTime = 1.0f;
        }
        if (SystemAlphaFadeEnable)
            system.SetGlobalAlpha(GetCurrentFadeValue() * (1.0f - SystemAlphaFadeMinimum) + SystemAlphaFadeMinimum);
    }
    system.SetNormalisedTime(UseSystemLifeSecs, normalisedTime);
    float airResistance;
    if (AirResistance > 0.0f)
        airResistance = (float)pow(1.0f - AirResistance, frameTime);
    else
        airResistance = 1.0f;
    C3DVector force(0.0f, 0.0f, 0.0f);
    if (fabs(GravityFactor) > 0.0f)
        force = environment.GetGravityVector() * GravityFactor * frameTime;
    if (fabs(WindFactor) > 0.0f && environment.IsWindActive())
        force += environment.GetWindVelocity() * WindFactor * frameTime;
    if (system.GetParentEmitter().IsEmitter2D()) {
        CMatrix3x4 rotation;
        rotation.SetFromVectorAndAngle(C3DVector(1.0f, 0.0f, 0.0f), -0.25f);
        rotation.TransformVector(force);
    }
    C3DVector acceleration(0.0f, 0.0f, 0.0f);
    if (!UseAccelerationParam || !params.GetVectorParam(acceleration, AccelerationParam))
        acceleration = ParticleAcceleration;
    acceleration *= ParticleAccelerationScale;
    long maxLife = (long)(GetTickRate() * ParticleLifeSecs);
    unsigned long& seed = GetRandomSeed();
    std::list<CParticle*>::iterator it = system.Particles.begin();
    while (it != system.Particles.end()) {
        CParticle* particle = *it;
        if (!particle->Active)
            continue;
        if (UseParticleLifeSecs && particle->GetLifetimer() >= maxLife) {
            it = system.DeleteParticle(it);
            continue;
        }
        C3DVector velocity = particle->GetVelocity();
        C3DVector change = (acceleration + particle->GetAcceleration()) * frameTime;
        C3DVector position = particle->GetPosition();
        C3DVector offset;
        if (RandomisePosEnable) {
            if (!particle->HaveRandomAccelerationData())
                particle->CreateRandomAccelerationData(seed);
            if (particle->HaveRandomAccelerationData()) {
                const NFractal::CPerlinNoise& noise = GetParticleEmitterManager().PerlinNoise;
                C2DVector noisePosition(lifeTime * RandomisePosSpeed.X + particle->GetRandomAccelerationTimeOffsetX(), particle->GetRandomAccelerationIndexX());
                offset.X = noise.GetGaussian2DNoise(noisePosition) * RandomisePosScale.X;
                noisePosition = C2DVector(lifeTime * RandomisePosSpeed.Y + particle->GetRandomAccelerationTimeOffsetY(), particle->GetRandomAccelerationIndexY());
                offset.Y = noise.GetGaussian2DNoise(noisePosition) * RandomisePosScale.Y;
                noisePosition = C2DVector(lifeTime * RandomisePosSpeed.Z + particle->GetRandomAccelerationTimeOffsetZ(), particle->GetRandomAccelerationIndexZ());
                offset.Z = noise.GetGaussian2DNoise(noisePosition) * RandomisePosScale.Z;
                C3DVector oldOffset = particle->GetOldRandomOffset();
                change.X -= oldOffset.X; change.Y -= oldOffset.Y; change.Z -= oldOffset.Z;
                change += offset;
            }
        }
        system.TransformVectorIfLocal(change);
        change += force;
        if (particle->GetLifetimer() >= 0) {
            velocity += change;
            position += velocity;
            velocity *= airResistance;
        } else {
            float fraction = 1.0f - particle->GetCreationTime();
            change *= fraction;
            velocity += change;
            position += velocity * fraction;
            offset *= fraction;
            velocity *= (float)pow(airResistance, fraction);
        }
        if (RandomisePosEnable && particle->HaveRandomAccelerationData())
            particle->SetOldRandomOffset(offset);
        C3DVector oldPosition = particle->GetPosition();
        system.TransformPosIfLocal(oldPosition);
        bool landscape = true;
        C3DVector collisionPosition;
        C3DVector collisionNormal;
        if (ParticleCollideWithGround && environment.IsParticleCollidingAt(oldPosition, velocity, false, collisionPosition, collisionNormal, landscape)) {
            if (GetParticleEmitterManager().GetEnableDecals() && ParticleCreateDecal) {
                CDecal* decal = system.AllocateDecal();
                if (decal != 0) {
                    decal->SetLandscapeOnlyDecal(landscape);
                    decal->SetMaxLife(-1);
                    decal->SetLifetimer(0);
                    decal->SetPosition(collisionPosition);
                    CRightHandedSet set;
                    set.SetUp(collisionNormal);
                    if (landscape) {
                        set.SetForward(GetPerpendicularForDecal(collisionNormal));
                        set.MakePerpendicular();
                    }
                    decal->SetRightHandedSet(set);
                    decal->Activate();
                    decal->SetFlickerIndex((unsigned char)GFRandom(256, &seed));
                    decal->SetFlickerTimeOffset((unsigned char)GFRandom(256, &seed));
                }
            }
            if (ParticleCreateDecalEmitter && !DecalEmitterName.IsEmpty()) {
                long handle = GetParticleEmitterDatabase().GetEmitterTemplateHandleFromName(DecalEmitterName);
                if (handle != 0) {
                    CParticleEmitter* emitter = GetParticleEmitterManager().CreateDecalEmitter(handle, collisionPosition, true,
                        CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
                    if (emitter != 0)
                        DecalEmitterList.push_back(CIntelligentPointer<CParticleEmitter>(emitter));
                }
            }
            if (ParticleDieOnCollision) {
                it = system.DeleteParticle(it);
                continue;
            }
            system.InverseTransformPosIfLocal(collisionPosition);
            position = collisionPosition;
            if (landscape) {
                velocity.X *= ParticleBounce;
                velocity.Y *= ParticleBounce;
                velocity.Z = -(velocity.Z * ParticleBounce);
            } else {
                CQuaternion rotation;
                rotation.SetFromAxisAngle(collisionNormal, 0.5f);
                CMatrix3x4 matrix;
                rotation.ToMatrix(matrix, C3DVector(0.0f, 0.0f, 0.0f));
                matrix.TransformVector(velocity);
                velocity = velocity * ParticleBounce;
            }
        }
        if (!SetOrientationFromDirection && !SetOrientationFromGame)
            particle->UpdateOrientation();
        else {
            C3DVector forward(0.0f, 0.0f, 0.0f);
            if (SetOrientationFromGame)
                params.GetVectorParam(forward, OrientFromGameParam);
            else {
                if (velocity.IsNonZero())
                    forward = velocity;
                if (StayWithEmitter)
                    forward += system.PeekEmitterVelocity();
            }
            if (forward.IsNonZero()) {
                forward.AccurateNormalise();
                CRightHandedSet set;
                set.SetForward(forward.AccurateNormalise());
                set.SetUp(GetPerpendicularForOrientation(forward));
                set.MakePerpendicular();
                CMatrix3x4 matrix;
                matrix.Set(set.GetRight(), set.GetForward(), set.GetUp(), C3DVector(0.0f, 0.0f, 0.0f));
                CQuaternion orientation;
                orientation.SetFromRotationMatrix(matrix);
                particle->SetOrientation(orientation);
                CQuaternion identity;
                identity.SetIdentity();
                particle->SetOrientationChange(identity);
            }
        }
        particle->UpdateAngle();
        particle->SetPosition(position);
        particle->SetVelocity(velocity);
        particle->ClearAcceleration();
        particle->IncrementLifetimer();
        ++it;
        alive = true;
    }
    std::list<CIntelligentPointer<CParticleEmitter> >::iterator emitter = DecalEmitterList.begin();
    while (emitter != DecalEmitterList.end()) {
        if ((*emitter).Get() != 0 && !(*emitter).Get()->IsDead()) {
            (*emitter).Get()->Update();
            ++emitter;
        } else
            emitter = DecalEmitterList.erase(emitter);
    }
    ++LifeTimer;
}
}
