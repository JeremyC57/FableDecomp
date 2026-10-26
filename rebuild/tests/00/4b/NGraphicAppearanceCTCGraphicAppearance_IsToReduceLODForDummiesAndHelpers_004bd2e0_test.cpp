#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
};

inline float GetDistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    float s = dz * dz;
    s += dx * dx;
    s += dy * dy;
    return s;
}

class CThingBaseDef {
public:
    unsigned char _pad00[0x48];
    float RenderFadeDistance;                       // +0x48
};

class CThing {
public:
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;                  // +0x70 CDefPointer<const CThingBaseDef>::Object
};

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetViewPosition() const;      // slot 8 (+0x20)
};

C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

extern unsigned char g_fadeDistanceOverridden_013b86ca;     // 0x013B86CA
extern long g_fadeDistanceOverride_01375710;                // 0x01375710
extern float g_primitiveDrawDistanceScale;                  // 0x013B8774

namespace NGraphicAppearance {

class CTCGraphicAppearance {
public:
    void*         __vfptr;                          // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    bool WasRenderedLastFrame() const;              // 0x004BD200
    bool IsToReduceLODForDummiesAndHelpers() const;
};

bool CTCGraphicAppearance::IsToReduceLODForDummiesAndHelpers() const
{
    float fadeDistance = (g_fadeDistanceOverridden_013b86ca ? (float)g_fadeDistanceOverride_01375710
                                                           : ParentThing->PBaseDef->RenderFadeDistance)
                         * (g_primitiveDrawDistanceScale + 1.0f);
    const CWorld* world = GetWorld();
    if (GetDistanceSquared(world->GetViewPosition(), GetRenderPositionForThing(*ParentThing, 1.0f)) > fadeDistance * fadeDistance)
        return true;
    return !WasRenderedLastFrame();
}

}

// ---- test stubs for the retail callees ----
static C3DVector g_view, g_pos; static bool g_rendered; static float g_lastInterp = -1.0f;
unsigned char g_fadeDistanceOverridden_013b86ca; long g_fadeDistanceOverride_01375710; float g_primitiveDrawDistanceScale;
void CWorld::v00() {} void CWorld::v01() {} void CWorld::v02() {} void CWorld::v03() {}
void CWorld::v04() {} void CWorld::v05() {} void CWorld::v06() {} void CWorld::v07() {}
C3DVector CWorld::GetViewPosition() const { return g_view; }
C3DVector __fastcall GetRenderPositionForThing(const CThing&, float t) { g_lastInterp = t; return g_pos; }
static CWorld g_world;
CWorld* NGraphicAppearance::CTCGraphicAppearance::GetWorld() const { return &g_world; }
bool NGraphicAppearance::CTCGraphicAppearance::WasRenderedLastFrame() const { return g_rendered; }

int main()
{
    int ok = 1;
    CThingBaseDef def; def.RenderFadeDistance = 10.0f;
    CThing thing; thing.PBaseDef = &def;
    NGraphicAppearance::CTCGraphicAppearance ga; ga.ParentThing = &thing;
    g_primitiveDrawDistanceScale = 1.0f;           // fade 10 * 2 = 20
    g_pos.X = 0; g_pos.Y = 0; g_pos.Z = 0;
    g_view.X = 30; g_view.Y = 0; g_view.Z = 0;     // 30 > 20: reduce
    g_rendered = true;
    if (!ga.IsToReduceLODForDummiesAndHelpers() || g_lastInterp != 1.0f) ok = 0;
    g_view.X = 3; g_view.Y = 4; g_view.Z = 12;     // 13 < 20: rendered -> keep
    if (ga.IsToReduceLODForDummiesAndHelpers()) ok = 0;
    g_rendered = false;                            // not rendered -> reduce
    if (!ga.IsToReduceLODForDummiesAndHelpers()) ok = 0;
    g_rendered = true; g_fadeDistanceOverridden_013b86ca = 1; g_fadeDistanceOverride_01375710 = 5;   // fade 10: 13 > 10
    if (!ga.IsToReduceLODForDummiesAndHelpers()) ok = 0;
    if (!ok) { std::printf("004bd2e0_TEST FAIL\n"); return 1; }
    std::printf("004bd2e0_TEST PASS\n");
    return 0;
}