// test: CCreatureAction_ScorpionTailPlunge::Initialise -- base Initialise, TotalTime = (long)(anim duration * constant FPS), PerformAction
#include <stdio.h>
#include <string.h>
static char trace[64];
static float g_duration = 1.5f;
static long g_fps = 30;
struct Creature { float GetAnimDuration(long idx) const { strcat(trace, "D"); return idx == 7 ? g_duration : 0.0f; } };
struct CombatBase {
    long TotalTime; Creature* PCreature;
    virtual ~CombatBase() { }
    virtual long GetAnimationIndex() { return 7; }
    virtual void PerformAction() { strcat(trace, "P"); }
    virtual void Initialise() { strcat(trace, "B"); }
};
struct Plunge : CombatBase {
    void Initialise() {
        CombatBase::Initialise();
        TotalTime = (long)(PCreature->GetAnimDuration(GetAnimationIndex()) * g_fps);
        PerformAction();
    }
};
int main() {
    Creature c; Plunge p; p.PCreature = &c; p.TotalTime = -1;
    CombatBase* b = &p; b->Initialise();
    if (strcmp(trace, "BDP") != 0 || p.TotalTime != 45) { printf("008c3e90_TEST FAIL %s %d\n", trace, (int)p.TotalTime); return 1; }
    trace[0] = 0; g_duration = 0.99f; g_fps = 1; b->Initialise();
    if (p.TotalTime != 0) { printf("008c3e90_TEST FAIL trunc\n"); return 1; }
    printf("008c3e90_TEST PASS\n");
    return 0;
}