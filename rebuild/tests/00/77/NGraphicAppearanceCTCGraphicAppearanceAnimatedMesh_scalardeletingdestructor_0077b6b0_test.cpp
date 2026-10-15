#include <cstdio>

struct CAGraphicAppearanceComponent;
static CAGraphicAppearanceComponent* g_unregistered = 0; static int g_unregCount = 0; static int g_baseDtor = 0;
struct CTCGraphicAppearance {
    void UnregisterComponent(CAGraphicAppearanceComponent* c) { g_unregistered = c; ++g_unregCount; }
};
struct CThing { CTCGraphicAppearance* PTCGraphicAppearance; unsigned char Dead : 1; unsigned char _f : 7; };
struct CTCBase {
    virtual ~CTCBase() { ++g_baseDtor; }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CAGraphicAppearanceComponent { virtual void v00() { } };
struct CPrimitive;
struct CTCGraphicAppearanceAnimatedMesh : CTCBase, CAGraphicAppearanceComponent {
    CPrimitive* PrimHandle;
    CTCGraphicAppearanceAnimatedMesh(CThing* owner) : CTCBase(owner), PrimHandle(0) { }
    virtual ~CTCGraphicAppearanceAnimatedMesh()
    {
        if (!ParentThing->Dead)
            ParentThing->PTCGraphicAppearance->UnregisterComponent(this);
    }
};

int main()
{
    CTCGraphicAppearance app; CThing owner; owner.PTCGraphicAppearance = &app; owner.Dead = 0; owner._f = 0;
    CTCGraphicAppearanceAnimatedMesh* m = new CTCGraphicAppearanceAnimatedMesh(&owner);
    CAGraphicAppearanceComponent* asComponent = m;
    CTCBase* asBase = m;
    delete asBase;                                // scalar deleting destructor through the CTCBase vtable
    if (g_unregCount != 1 || g_unregistered != asComponent || g_baseDtor != 1) { std::printf("0077b6b0_TEST FAIL\n"); return 1; }
    owner.Dead = 1;
    m = new CTCGraphicAppearanceAnimatedMesh(&owner);
    delete m;
    if (g_unregCount != 1 || g_baseDtor != 2) { std::printf("0077b6b0_TEST FAIL\n"); return 1; }
    std::printf("0077b6b0_TEST PASS\n");
    return 0;
}