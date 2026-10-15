#include <cstdio>
// behaviour test for CThingCreatureBase::PerformBlockedReaction (0x00661EF0)
struct CThing;
static long g_lastBlock = -1;
static int g_calls = 0;
struct CCreatureActionBase {
    bool Finished;
    virtual void OnBlowBlocked(long blockType) { ++g_calls; g_lastBlock = blockType; }
    CCreatureActionBase() : Finished(false) {}
};
struct CThingCreatureBase {
    CCreatureActionBase* PCurrentAction;
    void PerformBlockedReaction(CThing* blocker, long blockType);
};
void CThingCreatureBase::PerformBlockedReaction(CThing* blocker, long blockType)
{
    CCreatureActionBase* action = PCurrentAction;
    if (action && !action->Finished)
        action->OnBlowBlocked(blockType);
}
int main()
{
    CThingCreatureBase c; CCreatureActionBase a;
    c.PCurrentAction = 0;
    c.PerformBlockedReaction(0, 5);
    if (g_calls != 0) { std::printf("00661ef0_TEST FAIL\n"); return 1; }
    c.PCurrentAction = &a;
    c.PerformBlockedReaction(0, 7);
    if (g_calls != 1 || g_lastBlock != 7) { std::printf("00661ef0_TEST FAIL\n"); return 1; }
    a.Finished = true;
    c.PerformBlockedReaction(0, 9);
    if (g_calls != 1 || g_lastBlock != 7) { std::printf("00661ef0_TEST FAIL\n"); return 1; }
    std::printf("00661ef0_TEST PASS\n");
    return 0;
}
