#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CThing { public: int m_cleared; CThing() : m_cleared(0) { } void ClearCurrentAction() { ++m_cleared; } };
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { } CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CThing* GetPItem() const { return m_pItem; } };
static long g_framesPerSecond = 30;
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase() { }
    virtual void FinishAction() { ++m_finishCalls; }
    virtual long GetStartDelayFrames() { return m_startDelay; }
    virtual float GetFrameTimeStep() { return m_timeStep; }
    int m_finishCalls; long m_startDelay; float m_timeStep; int m_movementCalls;
    CBaseIntelligentPointer PCreature;
    long  TotalTime;
    long  ActionTime;
    long  AnimDelayTime;
    long  FramesRunning;
    float RunningTimeSeconds;
    float LastRunningTimeSeconds;
    long  StartFrame;
    bool  Finished;
    CCreatureActionBase() : m_finishCalls(0), m_startDelay(0), m_timeStep(0.0f), m_movementCalls(0), TotalTime(-1), ActionTime(0), AnimDelayTime(0),
        FramesRunning(0), RunningTimeSeconds(0.0f), LastRunningTimeSeconds(0.0f), StartFrame(0), Finished(false) { }
    void UpdateActionRunningTime() { RunningTimeSeconds += GetFrameTimeStep(); }
    void UpdateActionMovement() { ++m_movementCalls; }
    virtual void EditUpdate(float timeStep);
};
void CCreatureActionBase::EditUpdate(float timeStep)
{
    LastRunningTimeSeconds = RunningTimeSeconds;
    if (Finished) {
        PCreature.GetPItem()->ClearCurrentAction();
        return;
    }
    UpdateActionRunningTime();
    if (FramesRunning >= GetStartDelayFrames() + StartFrame) {
        if (!Finished)
            UpdateActionMovement();
    }
    if (TotalTime >= 0 && FramesRunning >= TotalTime) {
        if (!Finished) {
            FinishAction();
            return;
        }
    }
    FramesRunning = (long)(RunningTimeSeconds * g_framesPerSecond);
    if (TotalTime >= 0 && FramesRunning > TotalTime)
        FramesRunning = TotalTime;
}
int main()
{
    CThing creature;
    CCreatureActionBase action; action.PCreature.m_pItem = &creature;
    action.m_timeStep = 0.5f; action.RunningTimeSeconds = 1.0f; action.StartFrame = 10; action.m_startDelay = 5; action.FramesRunning = 20;
    action.EditUpdate(0.0f);
    bool ok = action.LastRunningTimeSeconds == 1.0f && action.RunningTimeSeconds == 1.5f && action.m_movementCalls == 1 && action.m_finishCalls == 0 && action.FramesRunning == 45;
    action.TotalTime = 40; action.EditUpdate(0.0f);
    ok = ok && action.m_finishCalls == 1 && action.FramesRunning == 45 && action.m_movementCalls == 2 && action.RunningTimeSeconds == 2.0f;
    action.TotalTime = 70; action.FramesRunning = 3; action.EditUpdate(0.0f);
    ok = ok && action.m_finishCalls == 1 && action.m_movementCalls == 2 && action.FramesRunning == 70 && action.RunningTimeSeconds == 2.5f;
    action.Finished = true; action.EditUpdate(0.0f);
    ok = ok && creature.m_cleared == 1 && action.LastRunningTimeSeconds == 2.5f && action.RunningTimeSeconds == 2.5f;
    if (!ok) { std::printf("00692db0_TEST FAIL\n"); return 1; }
    std::printf("00692db0_TEST PASS\n");
    return 0;
}