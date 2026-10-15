// Behaviour test for CTCSmashableBase::IsAvailableToHit (0x007EEBB0): available once more than one frame has passed.
#include <cstdio>

static long g_worldFrame = 0;
static int g_getWorldCalls = 0;

struct CWorld {
    static long GetWorldFrame() { return g_worldFrame; }
};
static CWorld g_world;

struct CTCBase {
    void* ParentThing;
    CWorld* GetWorld() const { ++g_getWorldCalls; return &g_world; }
};

struct CTCSmashableBase : CTCBase {
    long LastWorldFrameHit;
    bool Smashed;
    bool IsAvailableToHit() const;
};

bool CTCSmashableBase::IsAvailableToHit() const
{
    return GetWorld()->GetWorldFrame() - LastWorldFrameHit > 1 ? true : false;
}

int main()
{
    CTCSmashableBase s;
    s.LastWorldFrameHit = 100;
    g_worldFrame = 100; bool a = s.IsAvailableToHit();
    g_worldFrame = 101; bool b = s.IsAvailableToHit();
    g_worldFrame = 102; bool c = s.IsAvailableToHit();
    g_worldFrame = 50;  bool d = s.IsAvailableToHit();
    if (a || b || !c || d || g_getWorldCalls != 4) { std::printf("007eebb0_TEST FAIL\n"); return 1; }
    std::printf("007eebb0_TEST PASS\n");
    return 0;
}
