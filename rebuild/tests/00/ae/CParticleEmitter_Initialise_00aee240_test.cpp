// 00aee240 behaviour test: NParticleEngine::CParticleEmitter::Initialise (match.cpp body, host-modelled)
#include <stdio.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class C3DVector { public: float X, Y, Z; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class CMatrix3x4 {
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    CMatrix3x4() {}
    void SetInverse(const CMatrix3x4& m)
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
class CEnginePrimitive { public: int Kind; };
struct CEnginePrimitiveParticleGroup : CEnginePrimitive {
    float Distance; const C3DBoundingSphere* Sphere; bool SD, RZ; EEngineEffectWaterStage Stage; bool ZW, Clip, Dith;
    CEnginePrimitiveParticleGroup(float d, const C3DBoundingSphere* s, bool sd, bool rz, EEngineEffectWaterStage st, bool zw, bool c, bool di)
        : Distance(d), Sphere(s), SD(sd), RZ(rz), Stage(st), ZW(zw), Clip(c), Dith(di) { Kind = 3; }
};
struct CEnginePrimitiveParticleGroup2D : CEnginePrimitive {
    long Layer; CMatrix3x4 T; float PX, PY; bool SD, RZ;
    CEnginePrimitiveParticleGroup2D(long l, const CMatrix3x4& t, const C2DVector& p, bool sd, bool rz)
        : Layer(l), T(t), PX(p.X), PY(p.Y), SD(sd), RZ(rz) { Kind = 2; }
};
class CEnginePrimitiveHandle { public: int Id; };
static CEnginePrimitiveParticleGroup g_last3d(0, 0, 0, 0, WATER_STAGE_POST, 0, 0, 0);
static CEnginePrimitiveParticleGroup2D g_last2d(0, CMatrix3x4(), C2DVector(0, 0), 0, 0);
class CIEngine { public:
    int adds; long lastFlags;
    void AddPrimitive(CEnginePrimitiveHandle* h, const CEnginePrimitive* p, long flags)
    {
        ++adds; lastFlags = flags; h->Id = 77;
        if (p->Kind == 2) g_last2d = *(const CEnginePrimitiveParticleGroup2D*)p;
        else g_last3d = *(const CEnginePrimitiveParticleGroup*)p;
    }
};
struct CParticleUpdateParameters { int x; };
struct CParticleSystem {
    bool ok; int calls; CIEngine* e; CEnginePrimitiveHandle* g;
    bool Initialise(CIEngine* engine, CParticleUpdateParameters*, CEnginePrimitiveHandle* group) { ++calls; e = engine; g = group; return ok; }
};
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
template <class T> struct CVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class C3DPositionInterpolator { public: C3DVector StartPosition; };
class CParticleEmitter {
public:
    CVector<CParticleSystemPtr> ParticleSystems;
    CParticleUpdateParameters UpdateParameters;
    CEnginePrimitiveHandle PrimitiveGroupHandle;
    unsigned short IsScreenDisplacement : 1, ReadZBuffer : 1, Initialised : 1, Emitter2D : 1, Emitter2DLayer : 7;
    unsigned char PreWaterEffect : 1, WaterEffect : 1, ZBufferWriteable : 1, ClipEffectToWeatherMask : 1, EnableDithering : 1;
    C3DPositionInterpolator PositionInterpolator;
    CMatrix3x4 Transform;
    C3DBoundingSphere BSphere;
    float DistanceToCam;
    bool Initialise(CIEngine* engine)
    {
        float u0 = 0.0f; float u1 = 0.0f; float u2 = 0.0f; float u3 = 0.0f; float u4 = 0.0f;
        if (!Initialised) {
        if (engine) {
            if (Emitter2D) {
                CMatrix3x4 inverse;
                inverse.SetInverse(Transform);
                CEnginePrimitiveParticleGroup2D group(Emitter2DLayer, inverse,
                    C2DVector(PositionInterpolator.StartPosition.X, PositionInterpolator.StartPosition.Y),
                    IsScreenDisplacement, ReadZBuffer);
                engine->AddPrimitive(&PrimitiveGroupHandle, &group, 0x42);
            } else {
                EEngineEffectWaterStage stage;
                if (PreWaterEffect)
                    stage = WATER_STAGE_PRE;
                else
                    stage = WaterEffect ? WATER_STAGE_WATER : WATER_STAGE_POST;
                CEnginePrimitiveParticleGroup group(DistanceToCam, &BSphere, IsScreenDisplacement, ReadZBuffer, stage,
                    ZBufferWriteable, ClipEffectToWeatherMask, EnableDithering);
                engine->AddPrimitive(&PrimitiveGroupHandle, &group, 2);
            }
            Initialised = true;
        }
        for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it)
            Initialised &= it->Data->Initialise(engine, &UpdateParameters, &PrimitiveGroupHandle);
        }
        return Initialised;
    }
};
static void reset(CParticleEmitter& e, CParticleSystemPtr* b, CParticleSystemPtr* f)
{
    e.ParticleSystems._M_start = b; e.ParticleSystems._M_finish = f; e.ParticleSystems._M_end_of_storage = f;
    e.IsScreenDisplacement = 1; e.ReadZBuffer = 0; e.Initialised = 0; e.Emitter2D = 0; e.Emitter2DLayer = 5;
    e.PreWaterEffect = 0; e.WaterEffect = 1; e.ZBufferWriteable = 1; e.ClipEffectToWeatherMask = 0; e.EnableDithering = 1;
    e.PositionInterpolator.StartPosition.X = 3; e.PositionInterpolator.StartPosition.Y = 4; e.PositionInterpolator.StartPosition.Z = 5;
    float v[12] = { 2,0,1, 1,3,0, 0,1,4, 5,6,7 }; CMatrix3x4 t; float* q = &t.E11; for (int i = 0; i < 12; ++i) q[i] = v[i]; e.Transform = t;
    e.DistanceToCam = 12.5f; e.PrimitiveGroupHandle.Id = 0;
}
int main()
{
    CParticleSystem s1 = { true, 0, 0, 0 }, s2 = { true, 0, 0, 0 };
    CParticleSystemPtr ps[2] = { { &s1, 0 }, { &s2, 0 } };
    CIEngine eng = { 0, 0 };
    CParticleEmitter e; reset(e, ps, ps + 2);
    CHECK(e.Initialise(&eng));
    CHECK(eng.adds == 1 && eng.lastFlags == 2 && g_last3d.Stage == WATER_STAGE_WATER && g_last3d.Distance == 12.5f && g_last3d.Sphere == &e.BSphere);
    CHECK(g_last3d.SD && !g_last3d.RZ && g_last3d.ZW && !g_last3d.Clip && g_last3d.Dith);
    CHECK(s1.calls == 1 && s2.calls == 1 && s1.g == &e.PrimitiveGroupHandle && e.PrimitiveGroupHandle.Id == 77);
    CHECK(e.Initialise(&eng) && eng.adds == 1 && s1.calls == 1);   // already initialised: nothing
    reset(e, ps, ps + 2); e.PreWaterEffect = 1; s2.ok = false;
    CHECK(!e.Initialise(&eng));
    CHECK(eng.adds == 2 && g_last3d.Stage == WATER_STAGE_PRE && s2.calls == 2);
    reset(e, ps, ps + 2); e.WaterEffect = 0; s2.ok = true; e.Initialise(&eng); CHECK(g_last3d.Stage == WATER_STAGE_POST);
    reset(e, ps, ps + 2); e.Emitter2D = 1;
    CHECK(e.Initialise(&eng) && eng.lastFlags == 0x42 && g_last2d.Layer == 5 && g_last2d.PX == 3 && g_last2d.PY == 4);
    const CMatrix3x4& A = e.Transform; const CMatrix3x4& B = g_last2d.T;
    float a[4][3] = { {A.E11,A.E12,A.E13},{A.E21,A.E22,A.E23},{A.E31,A.E32,A.E33},{A.E41,A.E42,A.E43} };
    float b[4][3] = { {B.E11,B.E12,B.E13},{B.E21,B.E22,B.E23},{B.E31,B.E32,B.E33},{B.E41,B.E42,B.E43} };
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) {   // point/affine composition A then B is identity
        float s = (i == 3 ? b[3][j] : 0.0f); for (int k = 0; k < 3; ++k) s += a[i][k] * b[k][j];
        CHECK(fabs(s - (i == j ? 1.0f : 0.0f)) < 1e-4f);
    }
    reset(e, ps, ps + 2); int before = eng.adds; s1.calls = 0;
    CHECK(!e.Initialise(0) && eng.adds == before && s1.calls == 1 && s1.e == 0);   // no engine: systems still initialised
    reset(e, ps, ps); CHECK(e.Initialise(&eng));
    if (!g_fail) printf("00aee240_TEST PASS\n");
    return g_fail;
}