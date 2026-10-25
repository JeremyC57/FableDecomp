#include <cstdio>

class CEnginePrimitivePrepareInfo { public: int Tag; }; class C3DPositionInterpolator { public: int Tag; }; class C3DOrientationInterpolator { public: int Tag; };
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
    unsigned long GetUpdatePrimitiveReasons() const
    {
        if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame)
            return 1;
        return UpdatePrimitiveReasons;
    }
    void SetPrimitive(const CEnginePrimitivePrepareInfo&, const C3DPositionInterpolator&, const C3DOrientationInterpolator&) { ++g_set; }
    void Draw(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation)
    {
        PrimitivesDrawn = 1;
        if (g_LatchForceResendPrimitives)
            ResendPrimsEveryRenderFrame = ForceResendEveryRenderFrame;
        if (g_PreparePrimitivesEnabled && GetUpdatePrimitiveReasons())
            SetPrimitive(info, position, orientation);
    }
};

int main()
{
    CEnginePrimitivePrepareInfo info = { 1 }; C3DPositionInterpolator position = { 2 }; C3DOrientationInterpolator orientation = { 3 };
    CTCGraphicAppearance a; a.UpdatePrimitiveReasons = 0; a.SelfIlluminating = 0; a.ForceResendEveryRenderFrame = 1;
    a.ResendPrimsEveryRenderFrame = 0; a._mid = 0; a.PrimitivesDrawn = 0;
    a.Draw(info, position, orientation);               // no reasons, no resend: nothing
    if (!a.PrimitivesDrawn || g_set != 0) { std::printf("004be490_TEST FAIL\n"); return 1; }
    g_LatchForceResendPrimitives = true;
    a.Draw(info, position, orientation);               // latch copies ForceResend -> ResendPrims: prepared
    if (!a.ResendPrimsEveryRenderFrame || g_set != 1) { std::printf("004be490_TEST FAIL\n"); return 1; }
    g_LatchForceResendPrimitives = false; a.ResendPrimsEveryRenderFrame = 0; a.UpdatePrimitiveReasons = 4;
    a.Draw(info, position, orientation);
    if (g_set != 2) { std::printf("004be490_TEST FAIL\n"); return 1; }
    g_PreparePrimitivesEnabled = false;
    a.Draw(info, position, orientation);
    if (g_set != 2) { std::printf("004be490_TEST FAIL\n"); return 1; }
    std::printf("004be490_TEST PASS\n");
    return 0;
}