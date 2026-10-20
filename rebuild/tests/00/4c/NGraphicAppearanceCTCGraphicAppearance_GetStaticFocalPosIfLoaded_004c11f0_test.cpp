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
class CCharString;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector operator+(const C3DVector& o) const
    {
        C3DVector result;
        result.X = X + o.X;
        result.Y = Y + o.Y;
        result.Z = Z + o.Z;
        return result;
    }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector result;
        result.X = X - o.X;
        result.Y = Y - o.Y;
        result.Z = Z - o.Z;
        return result;
    }
};

C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

extern CCharString g_CCharString_013bac48;          // 0x013BAC48 "focus_eye_01"

namespace NGraphicAppearance {

class CTCGraphicAppearance {
public:
    void*         __vfptr;                          // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x28 - 0x08];
    C3DVector     CachedStaticFocalPosOffset;       // +0x28
    unsigned char _pad34[0x59 - 0x34];
    unsigned char SelfIlluminating : 1;             // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char ResendPrimsEveryRenderFrame : 1;
    unsigned char DisableOcclusionTesting : 1;
    unsigned char DrawAfterWater : 1;
    unsigned char CachedStaticFocalPos : 1;         // bit 5
    unsigned char RenderedLastFrame : 1;
    unsigned char PrimitivesDrawn : 1;
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& position, float time) const;   // 0x004C0560
    bool GetStaticFocalPosIfLoaded(C3DVector& position) const;
};

bool CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector& position) const
{
    if (CachedStaticFocalPos) {
        position = GetRenderPositionForThing(*ParentThing, 1.0f) + CachedStaticFocalPosOffset;
        return true;
    }
    if (GetHelperPoint(g_CCharString_013bac48, 0, 6, position, 1.0f)) {
        CTCGraphicAppearance* self = const_cast<CTCGraphicAppearance*>(this);
        self->CachedStaticFocalPos = true;
        self->CachedStaticFocalPosOffset = position - GetRenderPositionForThing(*ParentThing, 1.0f);
        return true;
    }
    return false;
}

}

// ---- test stubs for the retail callees ----
class CCharString { public: const char* Text; };
CCharString g_CCharString_013bac48 = { "focus_eye_01" };
static C3DVector g_render; static bool g_helperFound; static C3DVector g_helperPos; static int g_helperCalls;
static const char* g_helperName; static long g_helperFlags;
C3DVector __fastcall GetRenderPositionForThing(const CThing&, float) { return g_render; }
bool NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& p, float t) const
{ ++g_helperCalls; g_helperName = name.Text; g_helperFlags = (long)flags + index; if (g_helperFound && t == 1.0f) p = g_helperPos; return g_helperFound; }
class CThing { public: int Id; };

int main()
{
    int ok = 1;
    CThing owner;
    NGraphicAppearance::CTCGraphicAppearance ga; std::memset(&ga, 0, sizeof(ga)); ga.ParentThing = &owner;
    g_render.X = 1; g_render.Y = 2; g_render.Z = 3;
    C3DVector out; out.X = out.Y = out.Z = 0;
    g_helperFound = false;
    if (ga.GetStaticFocalPosIfLoaded(out) || ga.CachedStaticFocalPos) ok = 0;                 // no helper
    g_helperFound = true; g_helperPos.X = 5; g_helperPos.Y = 7; g_helperPos.Z = 9;
    if (!ga.GetStaticFocalPosIfLoaded(out) || out.X != 5 || out.Z != 9 || !ga.CachedStaticFocalPos) ok = 0;
    if (ga.CachedStaticFocalPosOffset.X != 4 || ga.CachedStaticFocalPosOffset.Y != 5 || ga.CachedStaticFocalPosOffset.Z != 6) ok = 0;
    if (std::strcmp(g_helperName, "focus_eye_01") != 0 || g_helperFlags != 6) ok = 0;
    g_render.X = 10; g_render.Y = 20; g_render.Z = 30; g_helperFound = false;
    if (!ga.GetStaticFocalPosIfLoaded(out) || out.X != 14 || out.Y != 25 || out.Z != 36 || g_helperCalls != 2) ok = 0;   // cached path
    if (!ok) { std::printf("004c11f0_TEST FAIL\n"); return 1; }
    std::printf("004c11f0_TEST PASS\n");
    return 0;
}