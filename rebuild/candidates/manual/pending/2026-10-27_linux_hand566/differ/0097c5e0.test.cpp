// Behaviour test for CAIStateGroup_ScorpionKing::Update (0x0097C5E0, parked DIFFER: re-split at 0x0097C720): phases 0/2/4
// recover when mode 0x38 is active, fight and advance when ready; phases 1/3 run phase 2 and schedule phase 2/4; Terminate
// when no longer runnable.
#include <stdio.h>
#include <string.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CTCBoss { public:
    long Phase; bool Ready; long Next;
    long GetBossPhase() const { return Phase; }
    bool IsReadyForPhaseChange() const { return Ready; }
    void IncrementBossPhase() { Log("increment"); }
    void SetNextBossPhase(long p, bool) { Next = p; Log("setNext"); }
};
class CTCCreatureModeManager { public: long Active; bool IsModeActive(long m) const { return Active == m; } };
class CThingAICreature { public:
    CTCBoss* Boss; CTCCreatureModeManager* Modes;
    void GetTC(CTCBoss** out) { *out = Boss; }
    bool GetTC(CTCCreatureModeManager** out) { if (!Modes) return false; *out = Modes; return true; }
};
class CAIStateGroup_ScorpionKing { public:
    CThingAICreature* Creature; bool Runnable;
    CThingAICreature& GetCreature() const { return *Creature; }
    void PerformRecover() { Log("recover"); }
    void UpdateCombat() { Log("combat"); }
    void UpdatePhase2() { Log("phase2"); }
    bool IsStillRunnable() const { return Runnable; }
    void Terminate() { Log("terminate"); }
    void Update()
    {
        CTCBoss* boss;
        GetCreature().GetTC(&boss);
        switch (boss->GetBossPhase()) {
        case 0: case 2: case 4: {
            CTCCreatureModeManager* modes;
            if (GetCreature().GetTC(&modes) && modes->IsModeActive(0x38))
                PerformRecover();
            UpdateCombat();
            if (boss->IsReadyForPhaseChange())
                boss->IncrementBossPhase();
            break;
        }
        case 1: UpdatePhase2(); boss->SetNextBossPhase(2, false); break;
        case 3: UpdatePhase2(); boss->SetNextBossPhase(4, false); break;
        }
        if (!IsStillRunnable())
            Terminate();
    }
};
static int Run(long phase, bool ready, long mode, bool runnable, const char* want, long wantNext)
{
    CTCBoss boss; boss.Phase = phase; boss.Ready = ready; boss.Next = -1;
    CTCCreatureModeManager modes; modes.Active = mode;
    CThingAICreature c; c.Boss = &boss; c.Modes = &modes;
    CAIStateGroup_ScorpionKing g; g.Creature = &c; g.Runnable = runnable;
    g_log[0] = 0; g.Update();
    return strcmp(g_log, want) == 0 && boss.Next == wantNext;
}
int main()
{
    int ok = Run(0, false, 0, true, "combat;", -1)
          && Run(2, true, 0x38, true, "recover;combat;increment;", -1)
          && Run(4, false, 0x38, false, "recover;combat;terminate;", -1)
          && Run(1, true, 0x38, true, "phase2;setNext;", 2)
          && Run(3, false, 0, false, "phase2;setNext;terminate;", 4)
          && Run(5, true, 0x38, true, "", -1);
    if (!ok) { printf("0097c5e0_TEST FAIL\n"); return 1; }
    printf("0097c5e0_TEST PASS\n");
    return 0;
}
