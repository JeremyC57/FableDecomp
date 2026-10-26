#include <cstdio>

// Model of NGraphicAppearance::CTCGraphicAppearanceSprite::`scalar deleting destructor' @ 0x0077c7e0: unless the owner thing is dead the
// component unregisters its CAGraphicAppearanceComponent subobject from the owner's graphic appearance, then the
// CTCBase destructor runs and the block is freed (flag bit 0).
static int g_baseDtorCalls = 0;
static void* g_unregistered = 0;
static int g_unregisterCalls = 0;

struct CAGraphicAppearanceComponent { virtual void OnAppearanceDraw() { } virtual ~CAGraphicAppearanceComponent() { } };
struct CTCGraphicAppearance {
    void UnregisterComponent(CAGraphicAppearanceComponent* component) { g_unregistered = component; ++g_unregisterCalls; }
};
class CThing { public:
    CTCGraphicAppearance* PTCGraphicAppearance;
    unsigned char Dead : 1;
};
class CTCBase { public:
    CThing* ParentThing;
    CTCBase(CThing* thing) : ParentThing(thing) { }
    virtual ~CTCBase() { ++g_baseDtorCalls; }
};
struct CComponent : CTCBase, CAGraphicAppearanceComponent {
    void* PrimHandle;
    CComponent(CThing* thing) : CTCBase(thing), PrimHandle(0) { }
    virtual ~CComponent()
    {
        if (!ParentThing->Dead)
            ParentThing->PTCGraphicAppearance->UnregisterComponent(this);
    }
};

int main()
{
    CTCGraphicAppearance appearance;
    CThing thing; thing.PTCGraphicAppearance = &appearance; thing.Dead = 0;
    CComponent* c = new CComponent(&thing);
    CAGraphicAppearanceComponent* secondary = c;
    CTCBase* asBase = c;
    delete asBase;                                   // virtual deleting destructor through the primary base
    if (g_unregisterCalls != 1 || g_unregistered != (void*)secondary || g_baseDtorCalls != 1) { std::printf("0077c7e0_TEST FAIL\n"); return 1; }
    thing.Dead = 1;
    asBase = new CComponent(&thing);
    delete asBase;                                   // dead owner: no unregister, base destructor still runs
    if (g_unregisterCalls != 1 || g_baseDtorCalls != 2) { std::printf("0077c7e0_TEST FAIL\n"); return 1; }
    std::printf("0077c7e0_TEST PASS\n");
    return 0;
}