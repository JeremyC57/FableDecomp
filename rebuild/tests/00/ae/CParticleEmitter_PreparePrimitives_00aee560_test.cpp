// Behaviour test for 00aee560: final.cpp body copied verbatim; the engine calls take the address of a temporary group
// (VC accepts it), so the g++ run needs `g++ -std=c++98 -w -fpermissive`.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// NParticleEngine::CParticleEmitter::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info) -- unless sleeping, (re)submit the emitter's primitive group to the engine (a 2D group with the inverse emitter transform and the start position for 2D emitters, else the 3D particle group with the camera distance, bounding sphere, draw flags and water stage), let every visible particle system prepare its primitives into it, and clear the cleared-particles/decals flags
// Leaf: PreparePrimitives
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEE560, 854 bytes (+ int3 padding), `ret 8`.
// Callees: CEnginePrimitiveParticleGroup(...) 0x00AF08D0, the engine's virtual slot 23 (+0x5c) through info->Engine,
//   CParticleSystem::PreparePrimitives 0x00AF40B0. The 2D group constructor (out of line at 0x00AF0880) and
//   CMatrix3x4::SetInverse are inline here. 1.0f 0x0122DED8.
// Layout: as the Initialise row 0x00AEE240 (Ego_r.pdb CParticleEmitter minus 0x0c); CParticleSystem Visible is bit 5 of
//   +0x50; CEnginePrimitivePrepareInfo engine +0x34 (landed 0x00AEB2D0). CEnginePrimitiveParticleGroup2D (0x4c bytes):
//   type 0x20, layer +0x0c, screen displacement +0x10, z read +0x11, transform +0x14, position +0x44.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class C3DVector { public:
    float X, Y, Z;
};
class C3DBoundingSphere { public:
    C3DVector Center;
    float     Radius;
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() {}
    __forceinline void SetInverse(const CMatrix3x4& m)
    {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
};
enum EEngineEffectWaterStage { WATER_STAGE_PRE, WATER_STAGE_WATER, WATER_STAGE_POST };
class CEnginePrimitive { public:
    unsigned long Type;                            // +0x00
    unsigned long Unknown04;                       // +0x04
    bool          Flag08, Flag09, Flag0a;          // +0x08
    CEnginePrimitive(unsigned long type) : Type(type), Unknown04(0), Flag08(false), Flag09(false), Flag0a(false) {}
};
struct CEnginePrimitiveParticleGroup : CEnginePrimitive {
    unsigned char _body0c[0x20];
    CEnginePrimitiveParticleGroup(float distance, const C3DBoundingSphere* sphere, bool screenDisplacement, bool readZBuffer, EEngineEffectWaterStage waterStage, bool zBufferWritable, bool clipToWeatherMask, bool enableDithering);   // 0x00AF08D0
};
struct CEnginePrimitiveParticleGroup2D : CEnginePrimitive {
    long       Layer;                              // +0x0c
    bool       ScreenDisplacement;                 // +0x10
    bool       ReadZBuffer;                        // +0x11
    CMatrix3x4 Transform;                          // +0x14
    C2DVector  Position;                           // +0x44
    CEnginePrimitiveParticleGroup2D(long layer, const CMatrix3x4& transform, const C2DVector& position, bool screenDisplacement, bool readZBuffer)
        : CEnginePrimitive(0x20), Layer(layer), ScreenDisplacement(screenDisplacement), ReadZBuffer(readZBuffer), Transform(transform), Position(position) {}   // out of line at 0x00AF0880
};
class CEnginePrimitiveHandle { public:
    void* __vftable;
    void* Data;
};
class CIEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive* primitive, long flags);   // slot 23 (+0x5c)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                          // +0x34
};
namespace NParticleEngine {
struct CParticleUpdateParameters {
    void*         __vftable;                       // +0x00
    unsigned char _maps[0x20];                     // +0x04
};
class CParticleSystem { public:
    unsigned char _state00[0x50];
    bool Initialised : 1;                          // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info, CEnginePrimitiveHandle* group);   // 0x00AF40B0
};
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
struct C3DPositionInterpolator {
    void*     __vftable;                           // +0x00
    C3DVector StartPosition;                       // +0x04
    C3DVector EndPosition;                         // +0x10
};
class CParticleEmitter {
public:
    void*                       __vftable;             // +0x00
    void*                       m_pIntelligentPointer; // +0x04
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
    unsigned char               _state46[0x4e];        // +0x46 OldPos, Velocity, OrientationInterpolator
    C3DPositionInterpolator     PositionInterpolator;  // +0x94
    CMatrix3x4                  Transform;             // +0xb0
    CMatrix3x4                  InverseTransform;      // +0xe0
    CMatrix3x4                  EndTransform;          // +0x110
    void*                       Name;                  // +0x140
    C3DBoundingSphere           BSphere;               // +0x144
    float                       DistanceToCam;         // +0x154
    bool IsSleeping() const { return Sleeping; }
    bool IsEmitter2D() const { return Emitter2D; }
    void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info);
};

void CParticleEmitter::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info)
{
    if (IsSleeping())
        return;
    if (IsEmitter2D()) {
        CMatrix3x4 inverse;
        inverse.SetInverse(Transform);
        info->Engine->AddPrimitive(&PrimitiveGroupHandle,
            &CEnginePrimitiveParticleGroup2D(Emitter2DLayer, inverse,
                C2DVector(PositionInterpolator.StartPosition.X, PositionInterpolator.StartPosition.Y),
                IsScreenDisplacement, ReadZBuffer), 0x42);
    } else {
        EEngineEffectWaterStage stage;
        if (PreWaterEffect)
            stage = WATER_STAGE_PRE;
        else
            stage = WaterEffect ? WATER_STAGE_WATER : WATER_STAGE_POST;
        info->Engine->AddPrimitive(&PrimitiveGroupHandle,
            &CEnginePrimitiveParticleGroup(DistanceToCam, &BSphere, IsScreenDisplacement, ReadZBuffer, stage,
                ZBufferWriteable, ClipEffectToWeatherMask, EnableDithering), 2);
    }
    for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it) {
        CParticleSystem* system = it->Data;
        if (system->Visible)
            system->PreparePrimitives(distance, info, &PrimitiveGroupHandle);
    }
    ClearedParticles = false;
    ClearedDecals = false;
}
}
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static int g_adds = 0; static long g_flags = 0; static unsigned long g_type = 0;
static CEnginePrimitiveParticleGroup2D g_group2D(0, CMatrix3x4(), C2DVector(0, 0), false, false);
static CEnginePrimitiveHandle* g_handle = 0;
static float g_dist = 0; static int g_stage = -1; static const C3DBoundingSphere* g_sphere = 0;
static int g_sysCalls = 0; static float g_sysDist = 0;
void CIEngine::v00() {} void CIEngine::v01() {} void CIEngine::v02() {} void CIEngine::v03() {} void CIEngine::v04() {}
void CIEngine::v05() {} void CIEngine::v06() {} void CIEngine::v07() {} void CIEngine::v08() {} void CIEngine::v09() {}
void CIEngine::v10() {} void CIEngine::v11() {} void CIEngine::v12() {} void CIEngine::v13() {} void CIEngine::v14() {}
void CIEngine::v15() {} void CIEngine::v16() {} void CIEngine::v17() {} void CIEngine::v18() {} void CIEngine::v19() {}
void CIEngine::v20() {} void CIEngine::v21() {} void CIEngine::v22() {}
void CIEngine::AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive* primitive, long flags)
{
    ++g_adds; g_flags = flags; g_type = primitive->Type; g_handle = handle;
    if (primitive->Type == 0x20) g_group2D = *(const CEnginePrimitiveParticleGroup2D*)primitive;
}
CEnginePrimitiveParticleGroup::CEnginePrimitiveParticleGroup(float distance, const C3DBoundingSphere* sphere, bool, bool, EEngineEffectWaterStage waterStage, bool, bool, bool)
    : CEnginePrimitive(0x1f) { g_dist = distance; g_sphere = sphere; g_stage = waterStage; }
void NParticleEngine::CParticleSystem::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo*, CEnginePrimitiveHandle*)
{ ++g_sysCalls; g_sysDist = distance; }
static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    using namespace NParticleEngine;
    static CParticleEmitter em; static CIEngine engine; static CEnginePrimitivePrepareInfo info;
    unsigned char* raw = (unsigned char*)&em; for (unsigned i = 0; i < sizeof(em); ++i) raw[i] = 0;
    info.Engine = &engine;
    static CParticleSystem sys[2];
    sys[0].Visible = 1; sys[1].Visible = 0;
    CParticleSystemPtr ptrs[2] = { { &sys[0], 0 }, { &sys[1], 0 } };
    em.ParticleSystems._M_start = ptrs; em.ParticleSystems._M_finish = ptrs + 2; em.ParticleSystems._M_end_of_storage = ptrs + 2;
    // Transform: rotation about Z by 90 deg scaled by 2, translation (5, 6, 7)
    CMatrix3x4& t = em.Transform;
    t.E11 = 0; t.E12 = 2; t.E13 = 0; t.E21 = -2; t.E22 = 0; t.E23 = 0; t.E31 = 0; t.E32 = 0; t.E33 = 2;
    t.E41 = 5; t.E42 = 6; t.E43 = 7;
    em.PositionInterpolator.StartPosition.X = 11; em.PositionInterpolator.StartPosition.Y = 12;
    em.DistanceToCam = 33;
    // sleeping -> nothing
    em.Sleeping = 1; em.ClearedParticles = 1;
    em.PreparePrimitives(1.0f, &info);
    Check(g_adds == 0 && g_sysCalls == 0 && em.ClearedParticles == 1, "sleeping");
    em.Sleeping = 0;
    // 2D emitter
    em.Emitter2D = 1; em.Emitter2DLayer = 9; em.IsScreenDisplacement = 1; em.ReadZBuffer = 0; em.ClearedDecals = 1;
    em.PreparePrimitives(4.0f, &info);
    Check(g_adds == 1 && g_flags == 0x42 && g_type == 0x20 && g_handle == &em.PrimitiveGroupHandle, "2D add");
    Check(g_group2D.Layer == 9 && g_group2D.ScreenDisplacement && !g_group2D.ReadZBuffer, "2D fields");
    Check(g_group2D.Position.X == 11 && g_group2D.Position.Y == 12, "2D position");
    const CMatrix3x4& v = g_group2D.Transform;
    // inverse of [R*2 | T]: rotation transposed / 2, translation -(T * inv)
    Check(Near(v.E11, 0) && Near(v.E12, -0.5f) && Near(v.E21, 0.5f) && Near(v.E22, 0) && Near(v.E33, 0.5f), "inverse rotation");
    Check(Near(v.E13, 0) && Near(v.E23, 0) && Near(v.E31, 0) && Near(v.E32, 0), "inverse zeros");
    // point (5,6,7) maps to origin: E41 = -(5*E11 + 6*E21 + 7*E31) = -3, E42 = -(5*-0.5) = 2.5, E43 = -3.5
    Check(Near(v.E41, -3) && Near(v.E42, 2.5f) && Near(v.E43, -3.5f), "inverse translation");
    Check(g_sysCalls == 1 && g_sysDist == 4.0f, "visible system only");
    Check(em.ClearedParticles == 0 && em.ClearedDecals == 0, "flags cleared");
    // 3D emitter, water stages
    em.Emitter2D = 0; em.PreWaterEffect = 0; em.WaterEffect = 1;
    em.PreparePrimitives(2.0f, &info);
    Check(g_adds == 2 && g_flags == 2 && g_type == 0x1f && g_dist == 33 && g_sphere == &em.BSphere && g_stage == WATER_STAGE_WATER, "3D water");
    em.WaterEffect = 0;
    em.PreparePrimitives(2.0f, &info);
    Check(g_stage == WATER_STAGE_POST, "3D post");
    em.PreWaterEffect = 1; em.WaterEffect = 1;
    em.PreparePrimitives(2.0f, &info);
    Check(g_stage == WATER_STAGE_PRE && g_sysCalls == 4, "3D pre");
    printf(g_fail ? "00aee560_TEST FAIL\n" : "00aee560_TEST PASS\n");
    return g_fail;
}