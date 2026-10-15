// Behaviour test for CThing::IsRenderedThisFrame @ 0x004C78F0: drawable + appearance + (reasons pending or resend-every-frame).
#include <cstdio>
struct CTCGraphicAppearance {
    unsigned char UpdatePrimitiveReasons;
    unsigned char SelfIlluminating : 1;
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char ResendPrimsEveryRenderFrame : 1;
    long WasRendered() const
    {
        if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame)
            return 1;
        return UpdatePrimitiveReasons;
    }
};
struct CThing {
    bool drawable;
    CTCGraphicAppearance* PTCGraphicAppearance;
    virtual bool DrawIsDrawable() { return drawable; }
    bool IsRenderedThisFrame()
    {
        return DrawIsDrawable() && PTCGraphicAppearance && PTCGraphicAppearance->WasRendered();
    }
};
int main()
{
    CTCGraphicAppearance ga; ga.UpdatePrimitiveReasons = 0; ga.SelfIlluminating = 1; ga.ForceResendEveryRenderFrame = 1; ga.ResendPrimsEveryRenderFrame = 0;
    CThing t; t.drawable = true; t.PTCGraphicAppearance = &ga;
    if (t.IsRenderedThisFrame()) { std::printf("004c78f0_TEST FAIL\n"); return 1; }
    ga.ResendPrimsEveryRenderFrame = 1;
    if (!t.IsRenderedThisFrame()) { std::printf("004c78f0_TEST FAIL\n"); return 1; }
    ga.ResendPrimsEveryRenderFrame = 0; ga.UpdatePrimitiveReasons = 4;
    if (!t.IsRenderedThisFrame()) { std::printf("004c78f0_TEST FAIL\n"); return 1; }
    t.drawable = false;
    if (t.IsRenderedThisFrame()) { std::printf("004c78f0_TEST FAIL\n"); return 1; }
    t.drawable = true; t.PTCGraphicAppearance = 0;
    if (t.IsRenderedThisFrame()) { std::printf("004c78f0_TEST FAIL\n"); return 1; }
    std::printf("004c78f0_TEST PASS\n");
    return 0;
}
