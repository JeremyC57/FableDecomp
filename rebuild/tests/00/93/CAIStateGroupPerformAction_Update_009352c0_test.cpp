// Behaviour test for CAIStateGroup_PerformAction::Update (0x009352C0): model copy, scripted stubs.
#include <cstdio>
#include <cstring>

struct CThing { unsigned char m_flags91; };
static bool IsAlive(const CThing* thing)
{
    if (thing != 0 && !(thing->m_flags91 & 1))
        return true;
    return false;
}
static CThing* g_perceived;
static int g_looks[32], g_li, g_performs[32], g_pi, g_speaks[32], g_si, g_fired, g_terminated;
static const char* g_lastAnim;
static bool g_lastFace;
static const CThing* g_lastTarget;
static long g_lastPriority;
struct CTCPerceiveThing { CThing* GetPerceivedThing() { return g_perceived; } };
struct CCharString { const char* text; CCharString(const char* t) : text(t) { } CCharString(const CCharString& o) : text(o.text) { } };
struct CTCAIScratchpad {
    long ScriptingStateGroup;
    CCharString Action;
    bool FaceHeroForAction;
    CTCAIScratchpad() : ScriptingStateGroup(0), Action("ANIM"), FaceHeroForAction(false) { }
};
static CTCAIScratchpad* g_pad;
static int g_changeAfter = -1;          // the scratchpad state switches to g_changeTo after this many speak/perform calls
static long g_changeTo;
struct CCreatureActionBase { };
struct CCreatureAction_PlayCombatAnimation : CCreatureActionBase {
    const CThing* target; const char* anim; bool face;
    CCreatureAction_PlayCombatAnimation(CThing*, CThing* t, const CCharString& a, bool f) : target(t), anim(a.text), face(f) { }
};
static void Tick() { if (g_changeAfter > 0 && --g_changeAfter == 0) g_pad->ScriptingStateGroup = g_changeTo; }
struct CThingCreatureBase : CThing {
    CTCPerceiveThing perceive;
    CTCAIScratchpad pad;
};
static CThingCreatureBase* g_hero;
struct CAIBrain { CThingCreatureBase* PCreature; };
struct CAIStateGroup_PerformAction {
    CAIBrain* PBrain;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CThingCreatureBase* GetHero() { return g_hero; }
    void Terminate() { ++g_terminated; }
    void Fired() { ++g_fired; }
    long AIAction_LookAtThing(long repeat, const CThing* thing, long frames, float speed)
    {
        g_lastTarget = thing; if (repeat != 2 || frames != 11 || speed != 0.013888889f) return -1;
        long r = g_looks[g_li++]; if (r == 4 && g_li == 3) g_perceived = 0; return r;
    }
    long AIAction_PerformAction(CCreatureActionBase* action)
    {
        CCreatureAction_PlayCombatAnimation* a = (CCreatureAction_PlayCombatAnimation*)action;
        g_lastAnim = a->anim; g_lastFace = a->face; g_lastTarget = a->target; Tick(); return g_performs[g_pi++];
    }
    long AIAction_SpeakToTarget(long repeat, CThingCreatureBase*, const CThing* target, unsigned long speech, bool interrupt, long priority, bool wait)
    {
        g_lastTarget = target; g_lastPriority = priority; Tick();
        if (repeat != 2 || speech != 0 || !interrupt || wait) return -1;
        return g_speaks[g_si++];
    }
    void Update();
};

void CAIStateGroup_PerformAction::Update()
{
    CThingCreatureBase* hero = GetHero();
    CThingCreatureBase* creature = GetCreature();
    CTCPerceiveThing* perceive = &creature->perceive;
    CTCAIScratchpad* scratchpad = &creature->pad;
    if (IsAlive(perceive->GetPerceivedThing()) && scratchpad->FaceHeroForAction) {
        do {
            if (scratchpad->ScriptingStateGroup == 0) {
                Terminate();
                return;
            }
        } while (AIAction_LookAtThing(2, perceive->GetPerceivedThing(), 11, 0.013888889f) == 4
                && IsAlive(perceive->GetPerceivedThing()));
        Fired();
    }
    if (IsAlive(perceive->GetPerceivedThing()) && scratchpad->ScriptingStateGroup != 0) {
        for (;;) {
            long result;
            if (scratchpad->ScriptingStateGroup == 0) {
                Terminate();
                return;
            }
            switch (scratchpad->ScriptingStateGroup) {
            case 1: {
                bool face = scratchpad->FaceHeroForAction;
                CCharString action(scratchpad->Action);
                CCreatureAction_PlayCombatAnimation play(GetCreature(), hero, action, face);
                result = AIAction_PerformAction(&play);
                break;
            }
            case 2:
                result = AIAction_SpeakToTarget(2, GetCreature(), hero, 0, true, 0x93, false);
                break;
            default:
                continue;
            }
            if (result == 4)
                continue;
            break;
        }
    }
    Fired();
    Terminate();
}

static void Reset()
{
    g_li = g_pi = g_si = g_fired = g_terminated = 0; g_changeAfter = -1; g_lastAnim = 0; g_lastTarget = 0;
    for (int i = 0; i < 32; ++i) { g_looks[i] = 2; g_performs[i] = 2; g_speaks[i] = 2; }
}

int main()
{
    int ok = 1;
    CThing other = { 0 };
    CThingCreatureBase hero; hero.m_flags91 = 0;
    CThingCreatureBase me; me.m_flags91 = 0; g_pad = &me.pad;
    CAIBrain brain = { &me };
    CAIStateGroup_PerformAction s; s.PBrain = &brain;
    g_hero = &hero;
    // nothing perceived: fired + terminate
    Reset(); g_perceived = 0; s.Update(); if (g_fired != 1 || g_terminated != 1 || g_li || g_pi || g_si) ok = 0;
    // perceived, state 1, no facing: play the action until it stops returning 4
    g_perceived = &other; me.pad.ScriptingStateGroup = 1; me.pad.FaceHeroForAction = false;
    Reset(); g_performs[0] = 4; g_performs[1] = 4; g_performs[2] = 3; s.Update();
    if (g_pi != 3 || g_li || g_fired != 1 || g_terminated != 1 || std::strcmp(g_lastAnim, "ANIM") != 0 || g_lastFace || g_lastTarget != &hero) ok = 0;
    // facing: look while 4 (the perceived thing dies on the third look), fired twice
    me.pad.FaceHeroForAction = true; me.pad.ScriptingStateGroup = 2;
    Reset(); g_looks[0] = 4; g_looks[1] = 4; g_looks[2] = 4; s.Update();
    if (g_li != 3 || g_si != 0 || g_fired != 2 || g_terminated != 1) ok = 0;
    // facing with state 0 at once: terminate only
    g_perceived = &other; me.pad.ScriptingStateGroup = 0;
    Reset(); s.Update(); if (g_fired || g_terminated != 1 || g_li) ok = 0;
    // speak, then the state drops to 0 mid-loop: terminate without firing
    me.pad.FaceHeroForAction = false; me.pad.ScriptingStateGroup = 2;
    Reset(); g_speaks[0] = 4; g_changeAfter = 1; g_changeTo = 0; s.Update();
    if (g_si != 1 || g_fired || g_terminated != 1 || g_lastPriority != 0x93 || g_lastTarget != &hero) ok = 0;
    // an unknown state spins until it becomes 2
    me.pad.ScriptingStateGroup = 2;
    Reset(); g_speaks[0] = 4; g_changeAfter = 1; g_changeTo = 1; g_performs[0] = 2; s.Update();
    if (g_si != 1 || g_pi != 1 || g_fired != 1 || g_terminated != 1) ok = 0;
    if (!ok) { std::printf("009352c0_TEST FAIL\n"); return 1; }
    std::printf("009352c0_TEST PASS\n");
    return 0;
}