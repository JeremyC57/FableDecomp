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
// ---- the candidate model (copied from src/00b1b2e0.cpp) ----
// NParticleEngine::CPSCSpline::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: persist the spline base, then the speed-scaling and bounce flags and the anim speed by name
// Leaf: ?DatabaseSerialise@CPSCSpline@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B2E0, 105 bytes (+7 int3 padding).
// Name: FableWin.pdb ?DatabaseSerialise@CPSCSpline@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z;
// slot 17 of the vtable 0x012A0934 (rtti-vtables.tsv).
// Callees: CPSCSplineBase::DatabaseSerialise @ 0x00B22670, CPersistContext::Transfer(name, bool&, const bool&)
// @ 0x004045C0 (x2), CPersistContext::Transfer(name, float&, const float&) @ 0x00410620 (both as the landed
// CPSCAttractor::DatabaseSerialise 0x00B18110).
// Data: the three member names @ 0x012A0918, 0x012A0908, 0x012A08F8 (read from Fable.exe .rdata: "ScaleSplineSpeed",
// "SplineBounce", "SplineAnimSpeed").
// Layout: as the landed CPSCSpline::WriteBinary 0x00B1AFF0: SplineBounce +0x39, ScaleSplineSpeed +0x3A,
// SplineAnimSpeed +0x3C.
// Shape: each member is transferred in place by reference; the three default-value temporaries (false,
// false, 0.0f) are homed in the dead `info` argument slot ([esp+10h]) after the base call. `ret 8`.
class CPersistContext { public:
    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
};

namespace NParticleEngine {

class CParticleSystem;
class CEmitterDatabaseSerialiseInfo;

class CPSCBase { public:
    virtual void v00();
    CParticleSystem* ParticleSystem;        // +0x04
    bool UpdateStarted : 1;                 // +0x08
    bool Initialised : 1;
};

class CPSCSplineBase : public CPSCBase { public:
    unsigned char _body[0x34 - 0x0c];       // +0x0c SplineAnimSpeed, Spline, the two counted data pointers
    void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);   // 0x00B22670
};

class CPSCSpline : public CPSCSplineBase { public:        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};

void CPSCSpline::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    CPSCSplineBase::DatabaseSerialise(context, info);
    context.Transfer("ScaleSplineSpeed", ScaleSplineSpeed, false);    // 0x012A0918
    context.Transfer("SplineBounce", SplineBounce, false);            // 0x012A0908
    context.Transfer("SplineAnimSpeed", SplineAnimSpeed, 0.0f);       // 0x012A08F8
}

} // namespace NParticleEngine

// ---- stubs for the external callees ----

void NParticleEngine::CPSCBase::v00() {}

static int g_baseCalls = 0;
static int g_n = 0;
static const char* g_names[4];
static const void* g_values[4];
static float g_defaults[4];
void CPersistContext::Transfer(const char* name, bool& value, const bool& defaultValue)
{ g_names[g_n] = name; g_values[g_n] = &value; g_defaults[g_n] = defaultValue ? 1.0f : 0.0f; ++g_n; }
void CPersistContext::Transfer(const char* name, float& value, const float& defaultValue)
{ g_names[g_n] = name; g_values[g_n] = &value; g_defaults[g_n] = defaultValue; ++g_n; }
void NParticleEngine::CPSCSplineBase::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) { ++g_baseCalls; g_n = 0; }

static void run()
{

    int infoStorage = 0;
    const NParticleEngine::CEmitterDatabaseSerialiseInfo& info = *reinterpret_cast<const NParticleEngine::CEmitterDatabaseSerialiseInfo*>(&infoStorage);
    CPersistContext context;
    NParticleEngine::CPSCSpline s;
    g_n = 9;                                   // the base call resets it: proves it runs first
    s.DatabaseSerialise(context, info);
    CHECK(g_baseCalls == 1 && g_n == 3);
    CHECK(std::strcmp(g_names[0], "ScaleSplineSpeed") == 0 && g_values[0] == &s.ScaleSplineSpeed && g_defaults[0] == 0.0f);
    CHECK(std::strcmp(g_names[1], "SplineBounce") == 0 && g_values[1] == &s.SplineBounce && g_defaults[1] == 0.0f);
    CHECK(std::strcmp(g_names[2], "SplineAnimSpeed") == 0 && g_values[2] == &s.SplineAnimSpeed && g_defaults[2] == 0.0f);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b1b2e0_TEST FAIL\n"); return 1; }
    std::printf("00b1b2e0_TEST PASS\n");
    return 0;
}