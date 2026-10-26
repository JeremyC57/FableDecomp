// Behaviour test for CCreatureAction_PlayIntoLoopOutOfAnimation::FinishAction (0x00843C10): stage 0 queues the loop
// stage with the full loop count; stage 1 queues another loop (count - 1) while loops remain or the finish frame is not
// reached, else the out-of stage; nothing when the creature is busy or in stage 2.
#include <cstdio>
#include <cstring>

static long g_frame = 0;
static int g_baseFinish = 0, g_queued = 0, g_lastKind = 0;
static long g_lastLoops = 0; static float g_lastTime = 0; static const char* g_lastName = 0;
long GetWorldFrame() { return g_frame; }
class CCharString { public: const char* s; };
class CThingCreatureBase { public: int Busy; bool IsBusy() const { return Busy != 0; } };
struct CPtr { CThingCreatureBase* p; CThingCreatureBase* GetPItem() const { return p; } };
class CCreatureActionBase { public:
    CPtr PCreature; long InterruptionGroupID, InterruptionPriority;
    void FinishAction() { ++g_baseFinish; }
    CThingCreatureBase* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    long Stage, NumberOfTimesToLoop; float AmountOfTimeToLoop; long FrameToFinish; bool UseAnimationMovement;
    CCharString LoopAnimName, OutOfAnimName;
    void FinishAction();
    void QueueLoop(const CCharString& loop, long loops, float time) { ++g_queued; g_lastKind = 1; g_lastName = loop.s; g_lastLoops = loops; g_lastTime = time; }
    void QueueOut(const CCharString& out) { ++g_queued; g_lastKind = 2; g_lastName = out.s; }
};
void CCreatureAction_PlayIntoLoopOutOfAnimation::FinishAction()
{
    CCreatureActionBase::FinishAction();
    if (GetCreature()->IsBusy())
        return;
    switch (Stage) {
    case 0:
        QueueLoop(LoopAnimName, NumberOfTimesToLoop, (float)FrameToFinish);
        break;
    case 1:
        if (NumberOfTimesToLoop <= 0 && GetWorldFrame() >= FrameToFinish)
            QueueOut(OutOfAnimName);
        else {
            long loops = NumberOfTimesToLoop - 1;
            QueueLoop(LoopAnimName, loops, (float)FrameToFinish);
        }
        break;
    }
}

int main()
{
    int ok = 1;
    CThingCreatureBase c; c.Busy = 0;
    CCreatureAction_PlayIntoLoopOutOfAnimation a; a.PCreature.p = &c;
    a.LoopAnimName.s = "LOOP"; a.OutOfAnimName.s = "OUT"; a.FrameToFinish = 100; a.NumberOfTimesToLoop = 3; a.UseAnimationMovement = true;
    a.Stage = 0; a.FinishAction();
    if (g_queued != 1 || g_lastKind != 1 || g_lastLoops != 3 || g_lastTime != 100.0f || std::strcmp(g_lastName, "LOOP") != 0) ok = 0;
    a.Stage = 1; a.FinishAction();
    if (g_queued != 2 || g_lastKind != 1 || g_lastLoops != 2) ok = 0;
    a.NumberOfTimesToLoop = 0; g_frame = 50; a.FinishAction();      // loops spent but frame not reached
    if (g_queued != 3 || g_lastKind != 1 || g_lastLoops != -1) ok = 0;
    g_frame = 100; a.FinishAction();                                 // done: out-of stage
    if (g_queued != 4 || g_lastKind != 2 || std::strcmp(g_lastName, "OUT") != 0) ok = 0;
    a.Stage = 2; a.FinishAction();
    c.Busy = 1; a.Stage = 0; a.FinishAction();
    if (g_queued != 4 || g_baseFinish != 6) ok = 0;
    std::printf("00843c10_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}