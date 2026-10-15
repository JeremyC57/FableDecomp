// Behaviour test for CActionDoCreatureAction::Update (0x00712BB0).
#include <cstdio>

struct CCreatureActionBase { long id; };

struct CCreatureActionQueue {
    long current;
    long GetCurrentActionID() { return current; }
};

struct CThingCreatureBase {
    CCreatureActionQueue* PActionQueue; bool busy; CCreatureActionBase* given;
    bool IsPerformingAnyAction() { return busy; }
    bool SetCurrentAction(CCreatureActionBase* a) { given = a; busy = true; PActionQueue->current = a->id; return true; }
};

struct CTCScriptedControl { CThingCreatureBase* PThing; };

struct CActionDoCreatureAction {
    CTCScriptedControl* PTCScript; bool Activated; bool Terminated;
    long ActionID; bool ActionSet; CCreatureActionBase* Action_Data;
    void Update()
    {
        CThingCreatureBase* creature = PTCScript->PThing;
        if (!ActionSet) {
            if (creature->IsPerformingAnyAction()) {
                if (creature->PActionQueue->GetCurrentActionID() == ActionID)
                    return;
                Terminated = true;
            }
            creature->SetCurrentAction(Action_Data);
            ActionID = creature->PActionQueue->GetCurrentActionID();
            ActionSet = true;
        } else {
            if (!creature->IsPerformingAnyAction() || creature->PActionQueue->GetCurrentActionID() != ActionID)
                Terminated = true;
        }
    }
};

static int fail() { std::printf("00712bb0_TEST FAIL\n"); return 1; }

int main()
{
    CCreatureActionQueue q; q.current = 0;
    CThingCreatureBase c; c.PActionQueue = &q; c.busy = false; c.given = 0;
    CTCScriptedControl sc; sc.PThing = &c;
    CCreatureActionBase act; act.id = 42;
    CActionDoCreatureAction a; a.PTCScript = &sc; a.Activated = false; a.Terminated = false;
    a.ActionID = 0; a.ActionSet = false; a.Action_Data = &act;
    a.Update();
    if (!a.ActionSet || a.ActionID != 42 || c.given != &act || a.Terminated) return fail();
    a.Update();                                  // still running the same id
    if (a.Terminated) return fail();
    c.busy = false; a.Update();                  // creature idle -> finished
    if (!a.Terminated) return fail();
    // busy with another action before the hand-over: flagged, then still handed over
    CActionDoCreatureAction b = a; b.ActionSet = false; b.Terminated = false; b.ActionID = 7; c.busy = true; q.current = 9;
    b.Update();
    if (!b.Terminated || !b.ActionSet || b.ActionID != 42) return fail();
    std::printf("00712bb0_TEST PASS\n");
    return 0;
}
