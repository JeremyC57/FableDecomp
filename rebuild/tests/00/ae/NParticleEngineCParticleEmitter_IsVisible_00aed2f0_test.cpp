// Behaviour test for 00aed2f0: final.cpp copied verbatim (32-bit `long` under g++; `g++ -fpermissive` not needed), every
// declared callee stubbed below so the fade, the visibility decisions and the rebuilt bounding sphere are observable.
// NParticleEngine::CParticleEmitter::IsVisible(float, const CEnginePrimitivePrepareInfo*)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define long int
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CParticleEmitter::IsVisible(float distance, const CEnginePrimitivePrepareInfo* info) -- decide whether the emitter draws this frame: work out the distance fade (2D emitters use distance 0), clear Visible, and unless sleeping either show a 2D emitter outright, or (within the draw range, past the fade-in start, with some alpha left) test the cached bounding sphere against the engine when it is valid, else rebuild it from every active system's world-space sphere (the emitter position with radius 0.1 when a system has none), test each one, and keep the new sphere when every system supplied one; each system gets the fade alpha and its own Visible flag
// Leaf: IsVisible
// Register plan: the 2D flag lives in bl (ebx pushed in the prologue) across the CalcFadeAlpha call. The same-TU stand-in
//   must keep its MEMORY knowledge (writes no emitter flag, so the flag word stays in edi across the call) without being
//   register-transparent: a pure x87 body preserves edx and VC7.1 then parks the flag in dl; the integer divide by a
//   constant in the stand-in (a mul that clobbers edx, no memory writes) restores retail's bl.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AED2F0, 778 bytes (+ int3 padding), `ret 8`.
// Callees: CParticleEmitter::CalcFadeAlpha(float) 0x00AECD70 (label "IsVisible_CalcFadeAlpha"; same TU, its body is
//   compiled above as the stand-in so VC7.1 knows it writes no emitter flags and keeps the flag word in edi across the
//   call, as retail does), CParticleSystem::GetBoundingSphereWorldSpace 0x00AF3FB0, CParticleEmitter::GetPosition 0x006E1B10
//   (ICF-shared with CTCCameraPointScripted's getter; Transform translation +0xd4), C3DBoundingSphere::AddToBounds
//   0x00ADDD80, the engine's virtual slot 21 (+0x54, IsSphereVisible). Floats 0.0 0x0122DEDC, 0.1 immediate.
// Layout: Ego_r.pdb CParticleEmitter minus 0x0c (the landed constructor 0x00AF0380): ParticleSystems +0x08, UpdateParameters
//   +0x14, flag word +0x40 (Sleeping bit 10, Emitter2D bit 11), flag bytes +0x44 (Visible bit 3) / +0x45
//   (CalcBoundingSphereOnceOnly bit 1, BoundingSphereValid bit 2), BSphere +0x144, DistanceToCam +0x154, MaxDrawDistance
//   +0x15c, FadeOutStart +0x160, FadeInStart +0x164, FadeInEnd +0x168. CParticleSystem: flag byte +0x50 (Enabled bit 1,
//   Dead bit 2, Visible bit 5), GlobalAlpha +0x54, FadeAlpha +0x58 (landed 0x00AF4660). CEnginePrimitivePrepareInfo engine
//   +0x34 (landed 0x00AEB2D0).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
    C3DBoundingSphere(const C3DVector& center, float radius) : Center(center), Radius(radius) {}
    void Set(const C3DVector& center, float radius) { Center = center; Radius = radius; }
    void AddToBounds(const C3DVector& center, float radius);   // 0x00ADDD80
};
struct CMatrix3x4 {
    C3DVector Right, Up, Forward, Translation;
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual bool IsSphereVisible(const C3DBoundingSphere& sphere);   // slot 21 (+0x54)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[4];
    float         AlwaysUpdateDistance;                // +0x04
    unsigned char _pad08[0x1c];
    float         Distance;                            // +0x24
    unsigned char _pad28[0xc];
    CIEngine*     Engine;                              // +0x34
};
namespace NParticleEngine {
struct CParticleUpdateParameters {
    void*         __vftable;                           // +0x00
    unsigned char _maps[0x20];                         // +0x04 VectorParams, UpdateParams
};
struct CParticleSystem {
    unsigned char _state00[0x50];
    bool Initialised : 1;                              // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                                 // +0x54
    float FadeAlpha;                                   // +0x58
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float distance, CParticleUpdateParameters* params);   // 0x00AF3FB0
};
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
class CParticleEmitter {
public:
    void*                       __vftable;             // +0x00
    void*                       m_pIntelligentPointer; // +0x04
    CVector<CParticleSystemPtr> ParticleSystems;       // +0x08
    CParticleUpdateParameters   UpdateParameters;      // +0x14
    unsigned char               PrimitiveGroupHandle[8];   // +0x38
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
    unsigned char               _state46[0x6a];        // +0x46 motion, interpolators
    CMatrix3x4                  Transform;             // +0xb0
    CMatrix3x4                  InverseTransform;      // +0xe0
    CMatrix3x4                  EndTransform;          // +0x110
    void*                       Name;                  // +0x140 CCharString
    C3DBoundingSphere           BSphere;               // +0x144
    float                       DistanceToCam;         // +0x154
    float                       MaxSpawnDistance;      // +0x158
    float                       MaxDrawDistance;       // +0x15c
    float                       FadeOutStart;          // +0x160
    float                       FadeInStart;           // +0x164
    float                       FadeInEnd;             // +0x168
    float CalcFadeAlpha(float distance) const;         // 0x00AECD70
    bool IsSleeping() const { return Sleeping; }
    __forceinline void UpdateSystemsVisibility(float alpha)
    {
        for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it) {
            CParticleSystem* system = it->Data;
            system->FadeAlpha = alpha;
            system->Visible = system->Enabled && !system->Dead && system->GlobalAlpha > 0.0f;
        }
    }
    __forceinline bool BuildBoundingSphere(float distance, const CEnginePrimitivePrepareInfo* info, float alpha)
    {
        bool allValid = true;
        C3DBoundingSphere sphere;
        for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it) {
            CParticleSystem* system = it->Data;
            bool visible = false;
            if (system->Enabled && !system->Dead && system->GlobalAlpha > 0.0f) {
                if (!system->GetBoundingSphereWorldSpace(&sphere, distance, &UpdateParameters)) {
                    sphere.Center = GetPosition();
                    sphere.Radius = 0.1f;
                    allValid = false;
                }
                visible = info->Engine->IsSphereVisible(sphere);
            }
            Visible |= visible;
            system->Visible = visible;
            system->FadeAlpha = alpha;
            BSphere.AddToBounds(sphere.Center, sphere.Radius);
        }
        return allValid;
    }
    C3DVector GetPosition() const;                     // 0x006E1B10
    bool IsVisible(float distance, const CEnginePrimitivePrepareInfo* info);
};

#pragma auto_inline(off)
// The stand-in body is given integer work (an unsigned divide by a constant = a mul that clobbers edx) so VC7.1 keeps
// its memory knowledge (no emitter flag is written) but not register transparency: retail's caller kept the 2D flag in
// bl, not in dl.
__declspec(noinline) float CParticleEmitter::CalcFadeAlpha(float distance) const
{
    unsigned long bits = *(const unsigned long*)&distance;
    if ((bits / 7u) & 1)
        bits = bits % 7u;
    if (distance < FadeInStart || distance > MaxDrawDistance)
        return 0.0f;
    if (distance < FadeInEnd)
        return (distance - FadeInStart) / (FadeInEnd - FadeInStart) + (float)bits * 0.0f;
    if (distance < FadeOutStart)
        return 1.0f;
    return 1.0f - (distance - FadeOutStart) / (MaxDrawDistance - FadeOutStart);
}
#pragma auto_inline(on)

bool CParticleEmitter::IsVisible(float distance, const CEnginePrimitivePrepareInfo* info)
{
    bool is2D = Emitter2D;
    float alpha = CalcFadeAlpha(is2D ? 0.0f : DistanceToCam);
    Visible = false;
    if (IsSleeping())
        return Visible;
    if (is2D) {
        Visible = true;
        UpdateSystemsVisibility(alpha);
        return Visible;
    }
    if (DistanceToCam <= MaxDrawDistance && DistanceToCam >= FadeInStart && alpha > 0.0f) {
        if (CalcBoundingSphereOnceOnly && BoundingSphereValid) {
            Visible = info->Engine->IsSphereVisible(BSphere);
            UpdateSystemsVisibility(alpha);
            return Visible;
        }
        BSphere.Set(C3DVector(0.0f, 0.0f, 0.0f), 0.0f);
        bool allValid = BuildBoundingSphere(distance, info, alpha);
        BoundingSphereValid = allValid & (BSphere.Radius > 0.0f);
    }
    return Visible;
}
}

// ---- stubs ----
using namespace NParticleEngine;
static int g_sphereQueries, g_boundsAdds, g_worldSpheres; static float g_lastQueryRadius; static C3DVector g_lastQueryCentre;
static bool g_sphereVisible[8]; static bool g_systemHasSphere[8]; static int g_addOrder;
static C3DVector g_addCentre[8]; static float g_addRadius[8];
static C3DVector g_position(10.0f, 20.0f, 30.0f);
void CIEngine::v00() { } void CIEngine::v01() { } void CIEngine::v02() { } void CIEngine::v03() { } void CIEngine::v04() { } void CIEngine::v05() { } void CIEngine::v06() { }
void CIEngine::v07() { } void CIEngine::v08() { } void CIEngine::v09() { } void CIEngine::v10() { } void CIEngine::v11() { } void CIEngine::v12() { } void CIEngine::v13() { }
void CIEngine::v14() { } void CIEngine::v15() { } void CIEngine::v16() { } void CIEngine::v17() { } void CIEngine::v18() { } void CIEngine::v19() { } void CIEngine::v20() { }
bool CIEngine::IsSphereVisible(const C3DBoundingSphere& sphere) { g_lastQueryCentre = sphere.Center; g_lastQueryRadius = sphere.Radius; return g_sphereVisible[g_sphereQueries++ % 8]; }
typedef CIEngine CTestEngine;
void C3DBoundingSphere::AddToBounds(const C3DVector& center, float radius) { if (g_boundsAdds < 8) { g_addCentre[g_boundsAdds] = center; g_addRadius[g_boundsAdds] = radius; } ++g_boundsAdds; if (radius > Radius) Radius = radius; }
bool CParticleSystem::GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float distance, CParticleUpdateParameters*)
{
    int i = g_worldSpheres++ % 8; if (!g_systemHasSphere[i]) return false;
    sphere->Center = C3DVector(distance, (float)i, 1.0f); sphere->Radius = 2.0f + (float)i; return true;
}
C3DVector CParticleEmitter::GetPosition() const { return g_position; }
struct Fixture {
    CParticleEmitter emitter; CParticleSystem systems[3]; CParticleSystemPtr ptrs[3]; CTestEngine engine; CEnginePrimitivePrepareInfo info;
    Fixture(int count)
    {
        std::memset(&emitter, 0, sizeof(emitter)); std::memset(systems, 0, sizeof(systems)); std::memset(&info, 0, sizeof(info));
        for (int i = 0; i < 3; ++i) { ptrs[i].Data = &systems[i]; ptrs[i].Info = 0; systems[i].Enabled = true; systems[i].GlobalAlpha = 1.0f; }
        emitter.ParticleSystems._M_start = ptrs; emitter.ParticleSystems._M_finish = ptrs + count; emitter.ParticleSystems._M_end_of_storage = ptrs + 3;
        emitter.FadeInStart = 10.0f; emitter.FadeInEnd = 20.0f; emitter.FadeOutStart = 80.0f; emitter.MaxDrawDistance = 100.0f;
        info.Engine = &engine;
        g_sphereQueries = g_boundsAdds = g_worldSpheres = 0; g_lastQueryRadius = -1.0f;
        for (int i = 0; i < 8; ++i) { g_sphereVisible[i] = true; g_systemHasSphere[i] = true; }
    }
};
static bool Near(float a, float b) { float d = a - b; return (d < 0 ? -d : d) < 1e-5f; }
int main()
{
    {   // 2D emitter: alpha from distance 0 (= 0 since below FadeInStart), visible outright, every system gets the alpha and its own flag
        Fixture f(3); f.emitter.Emitter2D = 1; f.emitter.DistanceToCam = 50.0f; f.systems[1].Dead = true; f.systems[2].GlobalAlpha = 0.0f;
        CHECK(f.emitter.IsVisible(50.0f, &f.info) == true && f.emitter.Visible);
        CHECK(f.systems[0].Visible && !f.systems[1].Visible && !f.systems[2].Visible && f.systems[0].FadeAlpha == 0.0f && g_sphereQueries == 0);
    }
    {   // sleeping: not visible, nothing touched
        Fixture f(2); f.emitter.Sleeping = 1; f.emitter.DistanceToCam = 50.0f; f.emitter.Visible = 1;
        CHECK(f.emitter.IsVisible(50.0f, &f.info) == false && !f.emitter.Visible && g_sphereQueries == 0 && f.systems[0].FadeAlpha == 0.0f);
    }
    {   // cached sphere: one engine query on the stored sphere, systems updated with the fade alpha (distance 50 -> alpha 1)
        Fixture f(2); f.emitter.DistanceToCam = 50.0f; f.emitter.CalcBoundingSphereOnceOnly = 1; f.emitter.BoundingSphereValid = 1;
        f.emitter.BSphere.Set(C3DVector(1.0f, 2.0f, 3.0f), 7.0f); g_sphereVisible[0] = false;
        CHECK(f.emitter.IsVisible(50.0f, &f.info) == false && g_sphereQueries == 1 && g_lastQueryRadius == 7.0f && g_lastQueryCentre.Y == 2.0f);
        CHECK(f.systems[0].FadeAlpha == 1.0f && f.systems[1].FadeAlpha == 1.0f && f.systems[0].Visible && g_boundsAdds == 0);
    }
    {   // rebuild: both systems supply spheres, the second is visible -> emitter visible, sphere rebuilt and marked valid
        Fixture f(2); f.emitter.DistanceToCam = 15.0f; g_sphereVisible[0] = false; g_sphereVisible[1] = true;
        CHECK(f.emitter.IsVisible(33.0f, &f.info) == true && f.emitter.Visible && g_sphereQueries == 2 && g_boundsAdds == 2);
        CHECK(!f.systems[0].Visible && f.systems[1].Visible && Near(f.systems[0].FadeAlpha, 0.5f) && Near(f.systems[1].FadeAlpha, 0.5f));
        CHECK(g_addCentre[0].X == 33.0f && g_addRadius[0] == 2.0f && g_addRadius[1] == 3.0f && f.emitter.BSphere.Radius == 3.0f && f.emitter.BoundingSphereValid);
    }
    {   // rebuild with a system that has no sphere: emitter position with radius 0.1, sphere not kept as valid; a dead system is skipped but still gets the alpha
        Fixture f(3); f.emitter.DistanceToCam = 90.0f; g_systemHasSphere[1] = false; f.systems[2].Dead = true;
        CHECK(f.emitter.IsVisible(5.0f, &f.info) == true && g_sphereQueries == 2 && g_boundsAdds == 3);
        CHECK(g_addCentre[1].X == 10.0f && g_addCentre[1].Z == 30.0f && Near(g_addRadius[1], 0.1f) && !f.emitter.BoundingSphereValid);
        CHECK(Near(f.systems[2].FadeAlpha, 0.5f) && !f.systems[2].Visible && g_addRadius[2] == 0.1f);
    }
    {   // out of range (beyond MaxDrawDistance) or before fade-in: nothing drawn, no queries
        Fixture f(2); f.emitter.DistanceToCam = 150.0f; CHECK(f.emitter.IsVisible(1.0f, &f.info) == false && g_sphereQueries == 0);
        Fixture g(2); g.emitter.DistanceToCam = 5.0f; CHECK(g.emitter.IsVisible(1.0f, &g.info) == false && g_sphereQueries == 0);
    }
    if (fails) { printf("00aed2f0_TEST FAIL (%d)\n", fails); return 1; }
    printf("00aed2f0_TEST PASS\n");
    return 0;
}