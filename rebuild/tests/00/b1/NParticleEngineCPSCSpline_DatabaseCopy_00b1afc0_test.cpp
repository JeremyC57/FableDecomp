#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00b1afc0.cpp) ----
// NParticleEngine::CPSCSpline::DatabaseCopy(const CPSCBase& other) -- vtable slot 18: copy another spline component's database settings (the spline base, then speed scaling, anim speed and bounce)
// Leaf: ?DatabaseCopy@CPSCSpline@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1AFC0, 37 bytes (+11 int3 padding).
// Name: FableWin.pdb ?DatabaseCopy@CPSCSpline@NParticleEngine@@UAEXABVCPSCBase@2@@Z; slot 18 of the vtable 0x012A0934
// (rtti-vtables.tsv; slot 18 of CPSCUpdateNormal is the landed DatabaseCopy 0x00B0B370).
// Callees: CPSCSplineBase::DatabaseCopy(const CPSCBase*) @ 0x00B22530 (landed; non-virtual, pointer argument).
// Layout: as the landed CPSCSpline::WriteBinary 0x00B1AFF0 (retail offsets 4 below Ego_r.pdb):
// SplineCurrentPosition +0x34, ReverseSpline +0x38, SplineBounce +0x39, ScaleSplineSpeed +0x3A, SplineAnimSpeed +0x3C.
// Shape: the base copy first (`this` kept in esi, the source in edi), then three member copies in
// source order: the byte, the float (an integer move through ecx), the byte. SplineCurrentPosition and
// ReverseSpline are run-time state and are not copied. `ret 4`.
namespace NParticleEngine {

struct CParticleSystem;

struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;        // +0x04
    bool UpdateStarted : 1;                 // +0x08
    bool Initialised : 1;
};

struct CPSCSplineBase : CPSCBase {
    unsigned char _body[0x34 - 0x0c];       // +0x0c SplineAnimSpeed, Spline, the two counted data pointers
    void DatabaseCopy(const CPSCBase* other);   // 0x00B22530
};

struct CPSCSpline : CPSCSplineBase {        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    virtual void DatabaseCopy(const CPSCBase& other);
};

void CPSCSpline::DatabaseCopy(const CPSCBase& other)
{
    const CPSCSpline& source = static_cast<const CPSCSpline&>(other);
    CPSCSplineBase::DatabaseCopy(&other);
    ScaleSplineSpeed = source.ScaleSplineSpeed;
    SplineAnimSpeed = source.SplineAnimSpeed;
    SplineBounce = source.SplineBounce;
}

} // namespace NParticleEngine

// ---- stubs for the external callees ----

void NParticleEngine::CPSCBase::v00() {}

static int g_baseCalls = 0;
static const NParticleEngine::CPSCBase* g_baseOther = 0;
void NParticleEngine::CPSCSplineBase::DatabaseCopy(const CPSCBase* other) { ++g_baseCalls; g_baseOther = other; }

static void run()
{

    NParticleEngine::CPSCSpline a;
    NParticleEngine::CPSCSpline b;
    a.SplineCurrentPosition = 0.5f; a.ReverseSpline = true; a.SplineBounce = false; a.ScaleSplineSpeed = false; a.SplineAnimSpeed = 1.0f;
    b.SplineCurrentPosition = 0.25f; b.ReverseSpline = false; b.SplineBounce = true; b.ScaleSplineSpeed = true; b.SplineAnimSpeed = 6.5f;
    a.DatabaseCopy(b);
    CHECK(g_baseCalls == 1 && g_baseOther == &b);
    CHECK(a.ScaleSplineSpeed == true && a.SplineBounce == true && a.SplineAnimSpeed == 6.5f);
    CHECK(a.SplineCurrentPosition == 0.5f && a.ReverseSpline == true);   // runtime state is not copied
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b1afc0_TEST FAIL\n"); return 1; }
    std::printf("00b1afc0_TEST PASS\n");
    return 0;
}