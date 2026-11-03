// Behaviour test for 0x00670D50 CThingCreatureDef::Validate: which checks run (results discarded) per graphic type,
// weapon def and mesh presence, and that every counted/def reference taken is released.
#include <cstdio>
#include <cstring>

static int g_static, g_animated, g_anim, g_dummy, g_helper, g_stats, g_released, g_defRelease;
static unsigned long g_lastDummyKey, g_lastHelperKey;

static unsigned long Calc(unsigned long seed, const char* p, unsigned long n)
{
    unsigned long h = seed;
    for (unsigned long i = 0; i < n; ++i) h = h * 31 + (unsigned char)p[i];
    return h;
}
struct Stats {
    bool HasDummyObject(unsigned long k) const { ++g_dummy; g_lastDummyKey = k; return true; }
    bool HasHelperPoint(unsigned long k) const { ++g_helper; g_lastHelperKey = k; return false; }
};
static Stats g_meshStats;
struct StatsPtr {                                   // the counted-pointer temporary
    const Stats* Data;
    StatsPtr() : Data(&g_meshStats) { ++g_stats; }
    ~StatsPtr() { ++g_released; }
    const Stats* operator->() const { return Data; }
};
struct MeshInfo { StatsPtr GetMeshStats() const { return StatsPtr(); } };
static MeshInfo g_mesh;
struct Bank {
    bool present;
    const MeshInfo* PeekStaticMesh(unsigned long) const { ++g_static; return present ? &g_mesh : 0; }
    const MeshInfo* PeekAnimatedMesh(unsigned long) const { ++g_animated; return present ? &g_mesh : 0; }
};
struct AppearancePtr { ~AppearancePtr() { ++g_defRelease; } };
struct Def { unsigned char Type; long PrimaryMeleeWeaponDef; unsigned long BankIndex; };

static const char* kRightHand = "RightHand";
static const char* kFocal = "Focal";
static void Validate(const Def& d, const Bank& bank)
{
    const MeshInfo* mesh = 0;
    if (d.Type == 5) mesh = bank.PeekStaticMesh(d.BankIndex);
    else if (d.Type == 4) mesh = bank.PeekAnimatedMesh(d.BankIndex);
    AppearancePtr appearance;
    d.Type != 4 || (++g_anim, true);
    if (d.PrimaryMeleeWeaponDef)
        !mesh || mesh->GetMeshStats()->HasDummyObject(Calc(0, kRightHand, (unsigned long)strlen(kRightHand)));
    !mesh || mesh->GetMeshStats()->HasHelperPoint(Calc(0, kFocal, (unsigned long)strlen(kFocal)));
}
static void reset() { g_static = g_animated = g_anim = g_dummy = g_helper = g_stats = g_released = g_defRelease = 0; }

int main()
{
    int ok = 1;
    Bank yes = { true }, no = { false };
    Def a = { 5, 1, 3 }; reset(); Validate(a, yes);
    ok &= g_static == 1 && g_animated == 0 && g_anim == 0 && g_dummy == 1 && g_helper == 1 && g_stats == 2 && g_released == 2 && g_defRelease == 1;
    ok &= g_lastDummyKey == Calc(0, "RightHand", 9) && g_lastHelperKey == Calc(0, "Focal", 5);
    Def b = { 4, 0, 3 }; reset(); Validate(b, yes);
    ok &= g_static == 0 && g_animated == 1 && g_anim == 1 && g_dummy == 0 && g_helper == 1 && g_stats == 1 && g_released == 1;
    Def c = { 4, 7, 3 }; reset(); Validate(c, no);
    ok &= g_animated == 1 && g_anim == 1 && g_dummy == 0 && g_helper == 0 && g_stats == 0 && g_defRelease == 1;
    Def e = { 1, 7, 3 }; reset(); Validate(e, yes);
    ok &= g_static == 0 && g_animated == 0 && g_anim == 0 && g_dummy == 0 && g_helper == 0;
    printf(ok ? "00670d50_TEST PASS\n" : "00670d50_TEST FAIL\n");
    return ok ? 0 : 1;
}