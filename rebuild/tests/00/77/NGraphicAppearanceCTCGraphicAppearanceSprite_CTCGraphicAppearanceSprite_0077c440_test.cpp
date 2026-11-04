// Behaviour test for NGraphicAppearance::CTCGraphicAppearanceSprite::~CTCGraphicAppearanceSprite (0x0077c440): a model of the source's logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static int g_unregisterCalls = 0, g_baseDtors = 0;
static void* g_lastComponent = 0;
struct CAGraphicAppearanceComponent { virtual void OnAppearanceClear() { } };
struct CTCGraphicAppearance {
    void UnregisterComponent(CAGraphicAppearanceComponent& c) { ++g_unregisterCalls; g_lastComponent = &c; }   // stub for 0x004C0000
};
struct CThing { CTCGraphicAppearance* PTCGraphicAppearance; bool Dead; bool IsDead() const { return Dead; } };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* t) : ParentThing(t) { } virtual ~CTCBase() { ++g_baseDtors; } };
struct CTCGraphicAppearanceSprite : CTCBase, CAGraphicAppearanceComponent {
    void* PrimHandle;
    CTCGraphicAppearanceSprite(CThing* t) : CTCBase(t), PrimHandle(0) { }
    virtual ~CTCGraphicAppearanceSprite()
    {
        if (!ParentThing->IsDead())
            ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
    }
};
static void RunTest()
{
    CTCGraphicAppearance appearance;
    CThing alive = { &appearance, false };
    CTCGraphicAppearanceSprite* c = new CTCGraphicAppearanceSprite(&alive);
    CAGraphicAppearanceComponent* sub = c;
    CHECK((void*)sub != (void*)c);                                // the component subobject is the argument, not `this`
    delete c;
    CHECK(g_unregisterCalls == 1 && g_lastComponent == (void*)sub && g_baseDtors == 1);
    CThing dead = { &appearance, true };
    c = new CTCGraphicAppearanceSprite(&dead);
    delete c;
    CHECK(g_unregisterCalls == 1 && g_baseDtors == 2);            // a dead owner: no unregister, still ~CTCBase
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("0077c440_TEST FAIL\n"); return 1; }
    std::printf("0077c440_TEST PASS\n");
    return 0;
}