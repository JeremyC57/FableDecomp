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
// ---- the candidate model (copied from src/00b163a0.cpp) ----
// NParticleEngine::CPSCOrbit::DatabaseCopy(const CPSCBase& other) -- vtable slot 18: copy another orbit component's database settings (the centre parameter and the three orbit settings records)
// Leaf: ?DatabaseCopy@CPSCOrbit@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B163A0, 58 bytes (+6 int3 padding).
// Name: FableWin.pdb ?DatabaseCopy@CPSCOrbit@NParticleEngine@@UAEXABVCPSCBase@2@@Z; slot 18 of the vtable 0x012A0374
// (rtti-vtables.tsv; slot 18 of CPSCUpdateNormal is the landed DatabaseCopy 0x00B0B370).
// Callees: none.
// Layout: as the landed CPSCOrbit::WriteBinary 0x00B163E0: CentreParam +0x0C, COrbitSettings Orbits[3] +0x10
// (48 bytes each); the run-time OrbitStates (+0xA0) are not copied.
// Shape: the dword first (through ecx), then each 48-byte POD record by its implicit copy (`rep movsd`, 12
// dwords) with its own pair of `lea` -- three separate assignments, `this` kept in eax. `ret 4`.
struct CBase {
    virtual ~CBase();
};

namespace NParticleEngine {
struct CParticleSystem;

struct COrbitSettings {                     // 48 bytes
    long  Type;
    bool  Enabled;
    float Radius;
    float Expand;
    bool  CycleFlag;
    float CycleTime;
    float SqueezeScale;
    float SqueezeAngle;
    float RotateSpeed;
    float RotateStart;
    float RotateSpeedRandom;
    float RotateStartRandom;
};

struct CPSCBase : CBase {                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

struct CPSCOrbit : CPSCBase {               // vtable 0x012A0374
    unsigned long  CentreParam;             // +0x0c
    COrbitSettings Orbits[3];               // +0x10
    virtual void DatabaseCopy(const CPSCBase& other);
};

void CPSCOrbit::DatabaseCopy(const CPSCBase& other)
{
    const CPSCOrbit& source = static_cast<const CPSCOrbit&>(other);
    CentreParam = source.CentreParam;
    Orbits[0] = source.Orbits[0];
    Orbits[1] = source.Orbits[1];
    Orbits[2] = source.Orbits[2];
}
} // namespace NParticleEngine

// ---- stubs for the external callees ----
CBase::~CBase() {}

static void run()
{

    NParticleEngine::CPSCOrbit a;
    NParticleEngine::CPSCOrbit b;
    a.CentreParam = 1;
    b.CentreParam = 77;
    int i = 0;
    while (i < 3) {
        NParticleEngine::COrbitSettings s;
        s.Type = i + 10; s.Enabled = (i != 1); s.Radius = 1.5f * (i + 1); s.Expand = 0.25f; s.CycleFlag = true;
        s.CycleTime = 2.0f; s.SqueezeScale = 0.5f; s.SqueezeAngle = 0.75f; s.RotateSpeed = 4.0f + i;
        s.RotateStart = 0.125f; s.RotateSpeedRandom = 1.0f; s.RotateStartRandom = 8.0f;
        b.Orbits[i] = s;
        NParticleEngine::COrbitSettings z;
        z.Type = 0; z.Enabled = false; z.Radius = 0.0f; z.Expand = 0.0f; z.CycleFlag = false; z.CycleTime = 0.0f;
        z.SqueezeScale = 0.0f; z.SqueezeAngle = 0.0f; z.RotateSpeed = 0.0f; z.RotateStart = 0.0f;
        z.RotateSpeedRandom = 0.0f; z.RotateStartRandom = 0.0f;
        a.Orbits[i] = z;
        ++i;
    }
    a.DatabaseCopy(b);
    CHECK(a.CentreParam == 77);
    i = 0;
    while (i < 3) {
        CHECK(a.Orbits[i].Type == i + 10 && a.Orbits[i].Enabled == (i != 1) && a.Orbits[i].Radius == 1.5f * (i + 1));
        CHECK(a.Orbits[i].RotateSpeed == 4.0f + i && a.Orbits[i].RotateStartRandom == 8.0f && a.Orbits[i].CycleFlag);
        ++i;
    }
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b163a0_TEST FAIL\n"); return 1; }
    std::printf("00b163a0_TEST PASS\n");
    return 0;
}