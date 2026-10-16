// behaviour test for CThingCreatureBase::IsFreeToPerformAction (0x00662210)
#include <cstdio>
struct CCreatureActionBase {
    bool overrideable, ready;
    int InterruptionGroupID, InterruptionPriority;
    bool Finished;
    virtual bool IsOverrideableByActionOfSamePriority() { return overrideable; }
    virtual bool IsActionReadyForInterruption() { return ready; }
};
struct CThingCreatureBase {
    bool Dead; CCreatureActionBase* cur;
    bool IsFreeToPerformAction(CCreatureActionBase* action)
    {
        if (Dead) return false;
        if (cur == 0 || cur->Finished) return true;
        int group = action->InterruptionGroupID, currentGroup = cur->InterruptionGroupID;
        if (group > currentGroup) return true;
        if (cur->IsActionReadyForInterruption()) return true;
        if (group == currentGroup) {
            int priority = action->InterruptionPriority, currentPriority = cur->InterruptionPriority;
            if (cur->IsOverrideableByActionOfSamePriority()) return priority >= currentPriority;
            return priority > currentPriority;
        }
        return false;
    }
};
static CCreatureActionBase mk(int g, int p, bool fin, bool ov, bool rd) { CCreatureActionBase a; a.InterruptionGroupID = g; a.InterruptionPriority = p; a.Finished = fin; a.overrideable = ov; a.ready = rd; return a; }
int main()
{
    CCreatureActionBase act = mk(2, 5, false, false, false);
    CThingCreatureBase c; c.Dead = true; c.cur = 0;
    int bad = c.IsFreeToPerformAction(&act) != false;
    c.Dead = false; if (!c.IsFreeToPerformAction(&act)) bad = 1;
    CCreatureActionBase cur = mk(3, 1, true, false, false); c.cur = &cur; if (!c.IsFreeToPerformAction(&act)) bad = 1;
    cur = mk(1, 9, false, false, false); if (!c.IsFreeToPerformAction(&act)) bad = 1;
    cur = mk(3, 1, false, false, true); if (!c.IsFreeToPerformAction(&act)) bad = 1;
    cur = mk(3, 1, false, false, false); if (c.IsFreeToPerformAction(&act)) bad = 1;
    cur = mk(2, 5, false, false, false); if (c.IsFreeToPerformAction(&act)) bad = 1;
    cur = mk(2, 5, false, true, false); if (!c.IsFreeToPerformAction(&act)) bad = 1;
    cur = mk(2, 4, false, false, false); if (!c.IsFreeToPerformAction(&act)) bad = 1;
    if (bad) { std::printf("00662210_TEST FAIL\n"); return 1; }
    std::printf("00662210_TEST PASS\n");
    return 0;
}
