// Behaviour test for CTCDegradableBase::IsAvailableToHit() const @ 0x008EC7E0: available only when more than one world
// frame has passed since LastWorldFrameHit.
#include <cstdio>
static long g_frame = 0;
static int g_worldCalls = 0;
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
class CTCDegradableBase { public:
    virtual void v00() { }
    unsigned char _base[0x8];
    long          LastWorldFrameHit;
    void*         PDefinition;
    void*         PCurrentDegradableInfo;
    void*         PParentThingPhysical;
    const CWorld* GetWorld() const { ++g_worldCalls; return &g_world; }
    bool IsAvailableToHit() const;
};
bool CTCDegradableBase::IsAvailableToHit() const
{
    bool available = false;
    if (GetWorld()->GetWorldFrame() - LastWorldFrameHit > 1)
        available = true;
    return available;
}
int main()
{
    CTCDegradableBase d; d.LastWorldFrameHit = 100;
    g_frame = 100; bool a = d.IsAvailableToHit();
    g_frame = 101; bool b = d.IsAvailableToHit();
    g_frame = 102; bool c = d.IsAvailableToHit();
    g_frame = 500; bool e = d.IsAvailableToHit();
    if (a || b || !c || !e || g_worldCalls != 4) { std::printf("008ec7e0_TEST FAIL\n"); return 1; }
    std::printf("008ec7e0_TEST PASS\n");
    return 0;
}