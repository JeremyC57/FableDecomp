#include <cstdio>
class CCharString { };
CCharString g_startRunningEventName_013bac8c;
struct CAnimManager_Calls {
    bool m_hasEvent;
    float m_eventTime;
    bool  AnimHasEvent(unsigned long, const CCharString&) { return m_hasEvent; }
    float GetEventTimeInSeconds(unsigned long, const CCharString&) { return m_eventTime; }
};
static CAnimManager_Calls g_manager;
CAnimManager_Calls* __fastcall GetAnimManager_006f90b0() { return &g_manager; }
struct CCreatureActionBase {
    virtual long GetAnimID() = 0;
    virtual float GetActionRunningTimeSeconds() = 0;
    float GetActionStartRunningTimeSeconds();
};
float CCreatureActionBase::GetActionStartRunningTimeSeconds()
{
    long animID = GetAnimID();
    if (animID > 0) {
        CAnimManager_Calls* manager = GetAnimManager_006f90b0();
        if (manager->AnimHasEvent(animID, g_startRunningEventName_013bac8c)) {
            float runningTime = GetActionRunningTimeSeconds();
            return manager->GetEventTimeInSeconds(animID, g_startRunningEventName_013bac8c) * runningTime;
        }
    }
    return -1.0f;
}
struct CTestAction : CCreatureActionBase {
    long m_anim; float m_running;
    virtual long GetAnimID() { return m_anim; }
    virtual float GetActionRunningTimeSeconds() { return m_running; }
};
int main()
{
    CTestAction a; a.m_anim = 5; a.m_running = 4.0f;
    g_manager.m_hasEvent = true; g_manager.m_eventTime = 0.5f;
    if (a.GetActionStartRunningTimeSeconds() != 2.0f) { std::printf("006923a0_TEST FAIL\n"); return 1; }
    g_manager.m_hasEvent = false;
    if (a.GetActionStartRunningTimeSeconds() != -1.0f) { std::printf("006923a0_TEST FAIL\n"); return 1; }
    g_manager.m_hasEvent = true; a.m_anim = 0;
    if (a.GetActionStartRunningTimeSeconds() != -1.0f) { std::printf("006923a0_TEST FAIL\n"); return 1; }
    std::printf("006923a0_TEST PASS\n");
    return 0;
}