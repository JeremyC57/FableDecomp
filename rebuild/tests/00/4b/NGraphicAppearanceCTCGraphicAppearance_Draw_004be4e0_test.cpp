// Behaviour test for 0x004BE4E0 CTCGraphicAppearance::Draw(info): builds the parent thing's position and orientation
// interpolators, marks the primitives drawn, latches ForceResend into ResendPrims when the global latch is set, and calls
// SetPrimitive(info, position, orientation) only when preparation is enabled and an update reason exists (a pending
// reasons byte, or ResendPrimsEveryRenderFrame).
#include <cstdio>

struct C3DPositionInterpolator { int tag; };
struct C3DOrientationInterpolator { int tag; };
struct CTCPhysicsBase { int orientTag; void GetOrientationInterpolator(C3DOrientationInterpolator& o) const { o.tag = orientTag; } };
struct CThing { int posTag; CTCPhysicsBase* PhysicsTC; };
static C3DPositionInterpolator GetRenderPositionInterpolateForThing(const CThing& t) { C3DPositionInterpolator p; p.tag = t.posTag; return p; }
struct CEnginePrimitivePrepareInfo { int id; };

static bool g_LatchForceResendPrimitives = false;
static bool g_PreparePrimitivesEnabled = false;
static int g_calls = 0, g_lastPos = 0, g_lastOrient = 0, g_lastInfo = 0;

struct CTCGraphicAppearance {
    CThing* ParentThing;
    unsigned char UpdatePrimitiveReasons;
    bool ForceResendEveryRenderFrame, ResendPrimsEveryRenderFrame, PrimitivesDrawn;
    unsigned long GetPrimitiveUpdateReasons() const { if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame) return 1; return UpdatePrimitiveReasons; }
    void SetPrimitive(const CEnginePrimitivePrepareInfo& i, const C3DPositionInterpolator& p, const C3DOrientationInterpolator& o)
    { ++g_calls; g_lastInfo = i.id; g_lastPos = p.tag; g_lastOrient = o.tag; }
    void Draw(const CEnginePrimitivePrepareInfo& info)
    {
        const CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        C3DPositionInterpolator position = GetRenderPositionInterpolateForThing(*ParentThing);
        C3DOrientationInterpolator orientation;
        physics->GetOrientationInterpolator(orientation);
        PrimitivesDrawn = true;
        if (g_LatchForceResendPrimitives) ResendPrimsEveryRenderFrame = ForceResendEveryRenderFrame;
        if (g_PreparePrimitivesEnabled && GetPrimitiveUpdateReasons()) SetPrimitive(info, position, orientation);
    }
};

int main()
{
    CTCPhysicsBase phys = { 77 };
    CThing thing = { 55, &phys };
    CTCGraphicAppearance a = { &thing, 0, true, false, false };
    CEnginePrimitivePrepareInfo info = { 9 };
    a.Draw(info);                                           // disabled: no call
    if (!a.PrimitivesDrawn || g_calls != 0 || a.ResendPrimsEveryRenderFrame) { std::printf("004be4e0_TEST FAIL\n"); return 1; }
    g_PreparePrimitivesEnabled = true;
    a.Draw(info);                                           // no reasons, no resend: no call
    if (g_calls != 0) { std::printf("004be4e0_TEST FAIL\n"); return 1; }
    g_LatchForceResendPrimitives = true;
    a.Draw(info);                                           // resend latched from ForceResend: call
    if (!a.ResendPrimsEveryRenderFrame || g_calls != 1 || g_lastPos != 55 || g_lastOrient != 77 || g_lastInfo != 9) { std::printf("004be4e0_TEST FAIL\n"); return 1; }
    a.ForceResendEveryRenderFrame = false; a.UpdatePrimitiveReasons = 4;
    a.Draw(info);                                           // latch clears resend, but a reason is pending: call
    if (a.ResendPrimsEveryRenderFrame || g_calls != 2) { std::printf("004be4e0_TEST FAIL\n"); return 1; }
    std::printf("004be4e0_TEST PASS\n");
    return 0;
}