// Behaviour test for 007ea490: src/007ea490.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCTargeted::GetTargetPoint(long index, float radius) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTargeted::GetTargetPoint(long index, float radius) const -- the indexed targeting helper point of the owner's graphic appearance when there is one, else the generic fire point at the owner
// Leaf: GetTargetPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EA490, length 116 bytes (`ret 0Ch`, C3DVector through the hidden pointer).
// Callees: CTCGraphicAppearance::GetHelperPoint(const CCharString&, long, unsigned long, C3DVector*, float) @ 0x004C0560 (the global helper
// name @ 0x013BAC50, flags 0x20, 1.0f), CTCBase::GetWorld @ 0x00686A80, NTargeting::GetTargetingPositionOnThing(const CWorld&,
// const CThing&, float) @ 0x007EA2F0 (`__fastcall`, hidden C3DVector result in ecx, world in edx, `ret 8`; manifest "Creature::GetTargetFirePoint").
// Layout: CThing::PTCGraphicAppearance at retail +0x64 (PDB +0x74). CTCTargeted per engine/CTCTargeted.h (members unused here).
// Shape: the appearance is a named local read before the pushes; the helper point is a local returned through the user copy ctor (`fld X ; fstp [eax]`, Y/Z dword moves); the fallback passes the
// hidden pointer straight to the helper (args right to left: radius, thing, then the GetWorld call).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CCharString { void* m_pData; };
extern CCharString g_TargetHelperName;          // 0x013BAC50
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float scale);   // 0x004C0560
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64 (PDB +0x74)
};
struct CWorld;
namespace NTargeting {
C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height);   // 0x007EA2F0
}
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    const CWorld& GetWorld() const;             // 0x00686A80
};
struct CTCTargeted : CTCBase {
    unsigned long TargetingFlags;               // +0x0c
    bool Targetable;                            // +0x10
    bool UseTargetGlow;                         // +0x11
    bool ValidTargetWithoutLOS;                 // +0x12
    void* PTargetingDef;                        // +0x14
    C3DVector GetTargetPoint(long index, float radius) const;
};

C3DVector CTCTargeted::GetTargetPoint(long index, float radius) const
{
    if (index >= 0) {
        CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
        C3DVector point;
        if (appearance->GetHelperPoint(g_TargetHelperName, index, 0x20, &point, 1.0f))
            return point;
    }
    const CThing& thing = *ParentThing;
    return NTargeting::GetTargetingPositionOnThing(GetWorld(), thing, radius);
}

// ---- stubs and driver ----
CCharString g_TargetHelperName = { (void*)0x5151 };
static bool g_haveHelper = true; static const CCharString* g_name; static long g_index; static unsigned long g_flags; static float g_scale; static CTCGraphicAppearance* g_app;
bool CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* out, float scale)
{
    g_app = this; g_name = &name; g_index = index; g_flags = flags; g_scale = scale;
    if (!g_haveHelper) return false;
    out->X = 10.0f + index; out->Y = 20.0f; out->Z = 30.0f; return true;
}
static CWorld* const WORLD = (CWorld*)0x7070;
const CWorld& CTCBase::GetWorld() const { return *WORLD; }
static const CWorld* g_world; static const CThing* g_thing; static float g_height; static int g_fallbacks = 0;
C3DVector __fastcall NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float height)
{
    g_world = &world; g_thing = &thing; g_height = height; ++g_fallbacks;
    C3DVector r; r.X = 1.0f; r.Y = 2.0f; r.Z = height; return r;
}
void CTCBase::v00() {}
int main()
{
    static CThing owner; memset(&owner, 0, sizeof owner);
    CTCGraphicAppearance app; owner.PTCGraphicAppearance = &app;
    CTCTargeted t; t.ParentThing = &owner;
    C3DVector p = t.GetTargetPoint(3, 5.0f);
    CHECK(p.X == 13.0f && p.Y == 20.0f && p.Z == 30.0f); CHECK(g_app == &app); CHECK(g_name == &g_TargetHelperName);
    CHECK(g_index == 3); CHECK(g_flags == 0x20); CHECK(g_scale == 1.0f); CHECK(g_fallbacks == 0);
    g_haveHelper = false;
    p = t.GetTargetPoint(2, 6.0f);
    CHECK(p.X == 1.0f && p.Y == 2.0f && p.Z == 6.0f); CHECK(g_fallbacks == 1); CHECK(g_world == WORLD); CHECK(g_thing == &owner); CHECK(g_height == 6.0f);
    g_haveHelper = true; g_index = -5;
    p = t.GetTargetPoint(-1, 7.0f);                       // negative index: straight to the fallback
    CHECK(p.Z == 7.0f); CHECK(g_fallbacks == 2); CHECK(g_index == -5);
    if (fails == 0) printf("007ea490_TEST PASS\n"); else printf("007ea490_TEST FAIL\n");
    return fails;
}