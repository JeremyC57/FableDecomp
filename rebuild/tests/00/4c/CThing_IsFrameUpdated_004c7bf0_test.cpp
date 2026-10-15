// Behaviour test for CThing::IsFrameUpdated @ 0x004C7BF0: no def, always-update def, rendered this frame or forced -> true.
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
struct CThingBaseDef { unsigned char AvailableInEditor : 1; unsigned char AlwaysFrameUpdate : 1; };
struct CThing {
    bool drawable;
    CTCGraphicAppearance* PTCGraphicAppearance;
    CThingBaseDef* PBaseDef;
    unsigned char ForceFrameUpdate : 1;
    virtual bool DrawIsDrawable() { return drawable; }
    bool IsRenderedThisFrame()
    {
        return DrawIsDrawable() && PTCGraphicAppearance && PTCGraphicAppearance->WasRendered();
    }
    bool IsFrameUpdated()
    {
        return !PBaseDef || PBaseDef->AlwaysFrameUpdate || IsRenderedThisFrame() || ForceFrameUpdate;
    }
};
int main()
{
    CTCGraphicAppearance ga; ga.UpdatePrimitiveReasons = 0; ga.SelfIlluminating = 0; ga.ForceResendEveryRenderFrame = 0; ga.ResendPrimsEveryRenderFrame = 0;
    CThingBaseDef def; def.AvailableInEditor = 1; def.AlwaysFrameUpdate = 0;
    CThing t; t.drawable = true; t.PTCGraphicAppearance = &ga; t.PBaseDef = &def; t.ForceFrameUpdate = 0;
    if (t.IsFrameUpdated()) { std::printf("004c7bf0_TEST FAIL\n"); return 1; }
    t.ForceFrameUpdate = 1; if (!t.IsFrameUpdated()) { std::printf("004c7bf0_TEST FAIL\n"); return 1; }
    t.ForceFrameUpdate = 0; ga.UpdatePrimitiveReasons = 1; if (!t.IsFrameUpdated()) { std::printf("004c7bf0_TEST FAIL\n"); return 1; }
    ga.UpdatePrimitiveReasons = 0; def.AlwaysFrameUpdate = 1; if (!t.IsFrameUpdated()) { std::printf("004c7bf0_TEST FAIL\n"); return 1; }
    def.AlwaysFrameUpdate = 0; t.PBaseDef = 0; if (!t.IsFrameUpdated()) { std::printf("004c7bf0_TEST FAIL\n"); return 1; }
    std::printf("004c7bf0_TEST PASS\n");
    return 0;
}