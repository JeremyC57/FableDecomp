// Behaviour test for 0x007AA840 CTCAssassinRush::FinishMove: the creature's current action is finished,
// the FinalAction started when held, StartTrailDelay set and the effects cleaned up.
#include <cstdio>

struct CCreatureActionBase { long m_id; };
static int g_log[8];
static int g_n;
static CCreatureActionBase* g_started;

struct CThingCreatureBase {
    void FinishCurrentAction() { g_log[g_n++] = 1; }
    bool SetCurrentAction(CCreatureActionBase* action) { g_log[g_n++] = 2; g_started = action; return true; }
};
struct CCountedActionPointer { CCreatureActionBase* m_pObject; void* m_pCount; };
struct CTCAssassinRush {
    bool                  StartTrailDelay;
    CCountedActionPointer FinalAction;
    void CleanUpEffects() { g_log[g_n++] = StartTrailDelay ? 3 : 4; }
    void FinishMove(CThingCreatureBase* creature);
};

void CTCAssassinRush::FinishMove(CThingCreatureBase* creature)
{
    creature->FinishCurrentAction();
    if (FinalAction.m_pObject)
        creature->SetCurrentAction(FinalAction.m_pObject);
    StartTrailDelay = true;
    CleanUpEffects();
}

int main()
{
    CThingCreatureBase creature;
    CCreatureActionBase action;
    CTCAssassinRush rush;
    rush.StartTrailDelay = false;
    rush.FinalAction.m_pObject = &action;
    g_n = 0;
    rush.FinishMove(&creature);
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_started != &action || !rush.StartTrailDelay) { std::printf("007aa840_TEST FAIL\n"); return 1; }
    rush.StartTrailDelay = false;
    rush.FinalAction.m_pObject = 0;
    g_n = 0; g_started = 0;
    rush.FinishMove(&creature);
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 3 || g_started != 0) { std::printf("007aa840_TEST FAIL\n"); return 1; }
    std::printf("007aa840_TEST PASS\n");
    return 0;
}
