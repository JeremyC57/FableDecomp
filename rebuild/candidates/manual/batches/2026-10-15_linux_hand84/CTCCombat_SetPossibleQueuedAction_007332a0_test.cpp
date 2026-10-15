#include <cstdio>
struct CCreatureAction { int id; };
struct CThingCreatureBase {
    int m_queued; CCreatureAction* m_last;
    bool HasQueuedActions() { return m_queued > 0; }
    void QueueAction(CCreatureAction* action) { ++m_queued; m_last = action; }
};
struct CTCCombat {
    CThingCreatureBase* ParentCreature;
    bool SetPossibleQueuedAction(CCreatureAction* action)
    {
        if (ParentCreature->HasQueuedActions()) {
            ParentCreature->QueueAction(action);
            return true;
        }
        return false;
    }
};
int main()
{
    CCreatureAction a = { 1 };
    CThingCreatureBase c = { 0, 0 };
    CTCCombat t; t.ParentCreature = &c;
    if (t.SetPossibleQueuedAction(&a) || c.m_queued != 0 || c.m_last != 0) { std::printf("007332a0_TEST FAIL\n"); return 1; }
    c.m_queued = 2;
    if (!t.SetPossibleQueuedAction(&a) || c.m_queued != 3 || c.m_last != &a) { std::printf("007332a0_TEST FAIL\n"); return 1; }
    std::printf("007332a0_TEST PASS\n");
    return 0;
}
