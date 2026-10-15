#include <cstdio>
// behaviour test for CThingCreatureBase::SetLastActionHandedness (0x006619F0)
static long g_frame = 0;
static int g_getWorldCalls = 0;
struct CWorld {
    static long GetWorldFrame() { return g_frame; }
};
static CWorld g_world;
struct CThingCreatureBase {
    unsigned char _pad000[0x138];
    long PreviousActionHandedness;
    long PreviousActionHandednessWF;
    CWorld* GetWorld() { ++g_getWorldCalls; return &g_world; }
    void SetLastActionHandedness(long handedness);
};
void CThingCreatureBase::SetLastActionHandedness(long handedness)
{
    PreviousActionHandedness = handedness;
    PreviousActionHandednessWF = GetWorld()->GetWorldFrame();
}
int main()
{
    static CThingCreatureBase c;
    g_frame = 1234;
    c.SetLastActionHandedness(2);
    if (c.PreviousActionHandedness != 2 || c.PreviousActionHandednessWF != 1234 || g_getWorldCalls != 1) { std::printf("006619f0_TEST FAIL\n"); return 1; }
    g_frame = 99;
    c.SetLastActionHandedness(1);
    if (c.PreviousActionHandedness != 1 || c.PreviousActionHandednessWF != 99) { std::printf("006619f0_TEST FAIL\n"); return 1; }
    std::printf("006619f0_TEST PASS\n");
    return 0;
}