#include <cstdio>

struct CAGraphicAppearanceComponent;
static CAGraphicAppearanceComponent* g_registered = 0; static int g_count = 0;
struct CTCGraphicAppearance {
    void RegisterComponent(CAGraphicAppearanceComponent* c) { g_registered = c; ++g_count; }
};
struct CThing { CTCGraphicAppearance* PTCGraphicAppearance; };
struct CTCBase {
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CAGraphicAppearanceComponent { virtual ~CAGraphicAppearanceComponent() { } };
struct CPrimitive;
struct CTCGraphicAppearanceSprite : CTCBase, CAGraphicAppearanceComponent {
    float RotationAmountPerFrame, RotationAngle, OldRotationAngle, AnimStage, LastAnimStage, AnimSpeed;
    bool FadeOutWithAnim; CPrimitive* PrimHandle;
    CTCGraphicAppearanceSprite(CThing* owner)
        : CTCBase(owner), CAGraphicAppearanceComponent(),
          RotationAmountPerFrame(0.0f), RotationAngle(0.0f), OldRotationAngle(0.0f), AnimStage(0.0f), LastAnimStage(0.0f),
          AnimSpeed(1.0f), FadeOutWithAnim(false), PrimHandle(0)
    {
        ParentThing->PTCGraphicAppearance->RegisterComponent(this);
    }
};

int main()
{
    CTCGraphicAppearance app; CThing owner; owner.PTCGraphicAppearance = &app;
    CTCGraphicAppearanceSprite s(&owner);
    CAGraphicAppearanceComponent* asComponent = &s;
    if (g_count != 1 || g_registered != asComponent || s.RotationAmountPerFrame != 0.0f || s.RotationAngle != 0.0f ||
        s.OldRotationAngle != 0.0f || s.AnimStage != 0.0f || s.LastAnimStage != 0.0f || s.AnimSpeed != 1.0f ||
        s.FadeOutWithAnim || s.PrimHandle)
        { std::printf("0077c3e0_TEST FAIL\n"); return 1; }
    std::printf("0077c3e0_TEST PASS\n");
    return 0;
}