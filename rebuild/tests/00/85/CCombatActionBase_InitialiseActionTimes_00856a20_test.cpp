// Behaviour test for 0x00856A20 CCombatActionBase::InitialiseActionTimes.
#include <cstdio>
long g_framesPerSecond = 10;
struct CAnimManager {
    bool hasEvent; float eventTime;
    bool AnimHasEvent(unsigned long, const char*) { return hasEvent; }
    float GetEventTimeInSeconds(unsigned long, const char*) { return eventTime; }
};
static CAnimManager g_manager;
CAnimManager* GetAnimManager() { return &g_manager; }
struct CCreature { float duration; long lastAnim; float GetAnimDuration(long id) { lastAnim = id; return duration; } };
class CAnimationEntry { public: long AnimID; };
class CCombatActionBase { public:
    CCreature* creature; long TotalTime; long ActionTime; CAnimationEntry* PAnimEntry; bool HasPerformAction; long delay;
    long GetAnimDelayTime() { return delay; }
    void InitialiseActionTimes()
    {
        long framesPerSecond = g_framesPerSecond;
        TotalTime = 0;
        long animID = PAnimEntry->AnimID;
        CAnimManager* manager = GetAnimManager();
        if (manager->AnimHasEvent(animID, "PERFORM_ACTION")) {
            ActionTime = (long)(manager->GetEventTimeInSeconds(animID, "PERFORM_ACTION") * framesPerSecond) + TotalTime;
            HasPerformAction = true;
        }
        TotalTime = (long)(creature->GetAnimDuration(animID) * framesPerSecond + TotalTime);
        if (ActionTime >= 0) ActionTime += GetAnimDelayTime();
        TotalTime += GetAnimDelayTime();
    }
};
int main()
{
    CCreature cr; cr.duration = 2.0f; CAnimationEntry e = { 77 };
    CCombatActionBase a; a.creature = &cr; a.PAnimEntry = &e; a.TotalTime = 99; a.ActionTime = -1; a.HasPerformAction = false; a.delay = 3;
    g_manager.hasEvent = true; g_manager.eventTime = 0.5f;
    a.InitialiseActionTimes();
    if (a.TotalTime != 23 || a.ActionTime != 8 || !a.HasPerformAction || cr.lastAnim != 77) { std::printf("00856a20_TEST FAIL\n"); return 1; }
    g_manager.hasEvent = false; a.ActionTime = -1; a.HasPerformAction = false;
    a.InitialiseActionTimes();
    if (a.TotalTime != 23 || a.ActionTime != -1 || a.HasPerformAction) { std::printf("00856a20_TEST FAIL\n"); return 1; }
    std::printf("00856a20_TEST PASS\n");
    return 0;
}