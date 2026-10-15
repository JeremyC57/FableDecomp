#include <cstdio>

struct CEnginePrimitivePrepareInfo; struct C3DPositionInterpolator; struct C3DOrientationInterpolator;
static bool g_LatchForceResendPrimitives = false;
static bool g_PreparePrimitivesEnabled = true;
static int g_set = 0;
struct CTCGraphicAppearance {
    unsigned char UpdatePrimitiveReasons;
    unsigned char SelfIlluminating : 1;
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char ResendPrimsEveryRenderFrame : 1;
    unsigned char _mid : 4;
    unsigned char PrimitivesDrawn : 1;
    unsigned long GetPrimitiveUpdateReasons() const
    {
        if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame)
            return 1;
        return UpdatePrimitiveReasons;
    }
    void SetPrimitive(CEnginePrimitivePrepareInfo*, C3DPositionInterpolator*, C3DOrientationInterpolator*) { ++g_set; }
    void PrepareForRenderIfNeeded(CEnginePrimitivePrepareInfo* info, C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation)
    {
        PrimitivesDrawn = 1;
        if (g_LatchForceResendPrimitives)
            ResendPrimsEveryRenderFrame = ForceResendEveryRenderFrame;
        if (g_PreparePrimitivesEnabled && GetPrimitiveUpdateReasons())
            SetPrimitive(info, position, orientation);
    }
};

int main()
{
    CTCGraphicAppearance a; a.UpdatePrimitiveReasons = 0; a.SelfIlluminating = 0; a.ForceResendEveryRenderFrame = 1;
    a.ResendPrimsEveryRenderFrame = 0; a._mid = 0; a.PrimitivesDrawn = 0;
    a.PrepareForRenderIfNeeded(0, 0, 0);               // no reasons, no resend: nothing
    if (!a.PrimitivesDrawn || g_set != 0) { std::printf("004be490_TEST FAIL\n"); return 1; }
    g_LatchForceResendPrimitives = true;
    a.PrepareForRenderIfNeeded(0, 0, 0);               // latch copies ForceResend -> ResendPrims: prepared
    if (!a.ResendPrimsEveryRenderFrame || g_set != 1) { std::printf("004be490_TEST FAIL\n"); return 1; }
    g_LatchForceResendPrimitives = false; a.ResendPrimsEveryRenderFrame = 0; a.UpdatePrimitiveReasons = 4;
    a.PrepareForRenderIfNeeded(0, 0, 0);
    if (g_set != 2) { std::printf("004be490_TEST FAIL\n"); return 1; }
    g_PreparePrimitivesEnabled = false;
    a.PrepareForRenderIfNeeded(0, 0, 0);
    if (g_set != 2) { std::printf("004be490_TEST FAIL\n"); return 1; }
    std::printf("004be490_TEST PASS\n");
    return 0;
}
