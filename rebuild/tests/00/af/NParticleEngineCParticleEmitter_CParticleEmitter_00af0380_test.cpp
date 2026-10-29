// Behaviour test for NParticleEngine::CParticleEmitter::CParticleEmitter (0x00AF0380): the default emitter has empty
// containers, identity transforms, the default orientation basis, zero position interpolation and motion, the default
// distances from the static getters, and its flag defaults (ReadZBuffer, DieIfOffscreen, EnableDithering,
// ClearedParticles, ClearedDecals set; layer 74; everything else clear).
#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Translation;
    __forceinline void SetIdentity()
    {
        Right.Y = Right.Z = Up.X = Up.Z = Forward.X = Forward.Y = Translation.X = Translation.Y = Translation.Z = 0.0f;
        Right.X = Up.Y = Forward.Z = 1.0f;
    }
};
class CRightHandedSet { public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    void SetDefault() { Up = C3DVector(0, 0, 1); Forward = C3DVector(0, 1, 0); }
};
class C3DBoundingSphere { public:
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
};
class CCharString { public:
    void* PStringData;
    CCharString() : PStringData(0) {}
};
class CBaseClass {
public:
    CBaseClass() {}
    virtual ~CBaseClass() {}
};
class CBaseObject { public:
    virtual ~CBaseObject() {}
    void* m_pIntelligentPointer;
    CBaseObject() : m_pIntelligentPointer(0) {}
};
template <class T> struct CAllocProxy {               // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CAllocProxy(T* p) : _M_data(p) {}
};
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> class CVectorMap { public:        // retail 16 bytes
    CVector<V> Data;
    char       Compare;                                // empty comparator
    bool       Dirty;                                  // +0x0d
    CVectorMap() : Dirty(false) {}
};
namespace NParticleEngine {
class CParticleSystem;
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
struct CParticleUpdateParamDesc { unsigned long a, b; };
struct CParticleUpdateParameters : CBaseClass {             // vtable 0x0125FFDC, retail 0x24 bytes
    CVectorMap<unsigned long, C3DVector>                VectorParams;   // +0x04
    CVectorMap<unsigned long, CParticleUpdateParamDesc> UpdateParams;   // +0x14
};
}
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass { public:                // vtable 0x0122F598
    CEnginePersistentPrimitiveBase* Data;              // +0x04 CIVCountedPointer
    CEnginePrimitiveHandle() : Data(0) {}
};
class C3DOrientationInterpolator : public CBaseClass { public:            // vtable 0x01239824
    CRightHandedSet StartOrientation;                  // +0x04
    CRightHandedSet EndOrientation;                    // +0x1c
};
class C3DPositionInterpolator : public CBaseClass { public:               // vtable 0x0123981C
    C3DVector StartPosition;                           // +0x04
    C3DVector EndPosition;                             // +0x10
};
namespace NParticleEngine {
class CParticleEmitter : public CBaseObject {
public:
    CVector<CParticleSystemPtr> ParticleSystems;       // +0x08
    CParticleUpdateParameters   UpdateParameters;      // +0x14
    CEnginePrimitiveHandle      PrimitiveGroupHandle;  // +0x38
    unsigned short Priority : 3;                       // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;                 // +0x42
    unsigned char PreWaterEffect : 1;                  // +0x44
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    unsigned char DieIfOffsceen : 1;
    unsigned char OffscreenUpdate : 1;
    unsigned char ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1;                 // +0x45
    unsigned char CalcBoundingSphereOnceOnly : 1;
    unsigned char BoundingSphereValid : 1;
    unsigned char ClearedParticles : 1;
    unsigned char ClearedDecals : 1;
#pragma pack(push, 2)
    struct { C3DVector OldPos; C3DVector Velocity; } Motion;   // +0x46 OldPos, +0x52 Velocity
#pragma pack(pop)
    C3DOrientationInterpolator OrientationInterpolator;        // +0x60
    C3DPositionInterpolator    PositionInterpolator;           // +0x94
    CMatrix3x4                 Transform;                      // +0xb0
    CMatrix3x4                 InverseTransform;               // +0xe0
    CMatrix3x4                 EndTransform;                   // +0x110
    CCharString                Name;                           // +0x140
    C3DBoundingSphere          BSphere;                        // +0x144
    float                      DistanceToCam;                  // +0x154
    float                      MaxSpawnDistance;               // +0x158
    float                      MaxDrawDistance;                // +0x15c
    float                      FadeOutStart;                   // +0x160
    float                      FadeInStart;                    // +0x164
    float                      FadeInEnd;                      // +0x168
    static float GetDefaultMaxSpawnDistance() { return 100.0f; }
    static float GetDefaultMaxDrawDistance() { return 200.0f; }
    static float GetDefaultFadeOutStart() { return 0.75f; }
    static float GetDefaultFadeInEnd() { return 0.5f; }
    static float GetDefaultFadeInStart() { return 0.5f; }
    CParticleEmitter();
};

CParticleEmitter::CParticleEmitter()
{
    IsScreenDisplacement = false;
    ReadZBuffer = true;
    Dead = false;
    ParticleCreationDisabled = false;
    WaitingToSleep = false;
    Sleeping = false;
    Transform.SetIdentity();
    InverseTransform.SetIdentity();
    EndTransform.SetIdentity();
    Emitter2DLayer = 74;
    ContinuousEmitter = false;
    Emitter2D = false;
    PreWaterEffect = false;
    WaterEffect = false;
    ZBufferWriteable = false;
    AlwaysUpdate = false;
    DieIfOffsceen = true;
    OffscreenUpdate = false;
    ClipEffectToWeatherMask = false;
    EnableDithering = true;
    CalcBoundingSphereOnceOnly = false;
    BoundingSphereValid = false;
    MaxSpawnDistance = GetDefaultMaxSpawnDistance();
    MaxDrawDistance = GetDefaultMaxDrawDistance();
    FadeOutStart = GetDefaultFadeOutStart();
    FadeInEnd = GetDefaultFadeInEnd();
    FadeInStart = GetDefaultFadeInStart();
    Priority = 0;
    Initialised = false;
    OrientationInterpolator.StartOrientation.SetDefault();
    OrientationInterpolator.EndOrientation.SetDefault();
    PositionInterpolator.StartPosition = C3DVector(0.0f, 0.0f, 0.0f);
    PositionInterpolator.EndPosition = C3DVector(0.0f, 0.0f, 0.0f);
    Motion.Velocity = C3DVector(0.0f, 0.0f, 0.0f);
    Motion.OldPos = C3DVector(0.0f, 0.0f, 0.0f);
    Visible = false;
    ClearedParticles = true;
    ClearedDecals = true;
}
}

using namespace NParticleEngine;
static int IsIdentity(const CMatrix3x4& m)
{
    return m.Right.X == 1 && m.Right.Y == 0 && m.Right.Z == 0 && m.Up.X == 0 && m.Up.Y == 1 && m.Up.Z == 0
        && m.Forward.X == 0 && m.Forward.Y == 0 && m.Forward.Z == 1
        && m.Translation.X == 0 && m.Translation.Y == 0 && m.Translation.Z == 0;
}
static int IsZero(const C3DVector& v) { return v.X == 0 && v.Y == 0 && v.Z == 0; }
int main()
{
    int ok = 1;
    unsigned char raw[sizeof(CParticleEmitter)];
    memset(raw, 0xff, sizeof(raw));                    // garbage underneath: every flag must be set explicitly
    CParticleEmitter* e = new (raw) CParticleEmitter;
    ok &= e->ParticleSystems._M_start == 0 && e->ParticleSystems._M_finish == 0 && e->ParticleSystems._M_end_of_storage._M_data == 0;
    ok &= e->UpdateParameters.VectorParams.Data._M_start == 0 && !e->UpdateParameters.VectorParams.Dirty;
    ok &= e->UpdateParameters.UpdateParams.Data._M_start == 0 && !e->UpdateParameters.UpdateParams.Dirty;
    ok &= e->PrimitiveGroupHandle.Data == 0 && e->Name.PStringData == 0;
    ok &= IsIdentity(e->Transform) && IsIdentity(e->InverseTransform) && IsIdentity(e->EndTransform);
    ok &= e->OrientationInterpolator.StartOrientation.Up.Z == 1 && e->OrientationInterpolator.EndOrientation.Forward.Y == 1;
    ok &= IsZero(e->PositionInterpolator.StartPosition) && IsZero(e->PositionInterpolator.EndPosition);
    ok &= IsZero(e->Motion.OldPos) && IsZero(e->Motion.Velocity);
    ok &= IsZero(e->BSphere.Center) && e->BSphere.Radius == 0;
    ok &= e->MaxSpawnDistance == 100.0f && e->MaxDrawDistance == 200.0f && e->FadeOutStart == 0.75f;
    ok &= e->FadeInStart == 0.5f && e->FadeInEnd == 0.5f;
    ok &= e->Priority == 0 && !e->IsScreenDisplacement && e->ReadZBuffer && !e->Dead && !e->ParticleCreationDisabled;
    ok &= !e->ContinuousEmitter && !e->Initialised && !e->WaitingToSleep && !e->Sleeping && !e->Emitter2D;
    ok &= e->Emitter2DLayer == 74;
    ok &= !e->PreWaterEffect && !e->WaterEffect && !e->ZBufferWriteable && !e->Visible && !e->AlwaysUpdate;
    ok &= e->DieIfOffsceen && !e->OffscreenUpdate && !e->ClipEffectToWeatherMask;
    ok &= e->EnableDithering && !e->CalcBoundingSphereOnceOnly && !e->BoundingSphereValid;
    ok &= e->ClearedParticles && e->ClearedDecals;
    printf("00af0380_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}