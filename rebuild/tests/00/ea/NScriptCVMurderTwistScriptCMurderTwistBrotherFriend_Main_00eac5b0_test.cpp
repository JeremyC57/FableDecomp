// Behaviour test for NScript::CV_MurderTwistScript::CMurderTwistBrotherFriend::Main (0x00EAC5B0): a copy of the brain-state
// flow over recording stand-ins (talk greeting, the hit-triggered fight lines, the 1-health death speech that sets
// MurderTwistEvent = MTE_MEET_LEADER and BrainState = MTBS_DEAD, the paused-entity scopes and RemoveThing at the end).
#include <cstdio>
#include <cstring>

struct State {
    int frame, limit, talkAt, hitAt, lowAt, spoken, removed, pauseDepth, maxPause, startFail;
    bool healHit;
    const char* lines[8];
};
static State st;

static bool Term() { return st.frame >= st.limit; }
struct Game {
    void NewScriptFrame() { ++st.frame; }
    bool StartScriptingEntity() { return st.frame >= st.startFail; }
    float GetHealthHandle() { return 100.0f; }
    float GetHealthMe() { return st.frame >= st.lowAt ? 1.0f : 50.0f; }
    void Pause(bool on) { st.pauseDepth += on ? 1 : -1; if (st.pauseDepth > st.maxPause) st.maxPause = st.pauseDepth; }
    void RemoveThing() { ++st.removed; }
};
struct Pauser { Game& g; Pauser(Game& game) : g(game) { g.Pause(true); } ~Pauser() { g.Pause(false); } };

struct Friend {
    enum { MTBS_NONE = 0, MTBS_FIGHTING = 1, MTBS_NEARLY_DEAD = 2, MTBS_DEAD = 3 };
    enum { MTE_MEET_LEADER = 1 };
    Game G; long BrainState; long ParentEvent;
    void Speak(const char* s) { if (st.spoken < 8) st.lines[st.spoken] = s; ++st.spoken; }
    bool Talked() { return st.frame == st.talkAt; }
    bool HitBy() { return st.frame == st.hitAt && !st.healHit; }
    bool HitByAbility() { return st.frame == st.hitAt && st.healHit; }
    bool HitByHeal() { return st.healHit; }
    void Main();
};

void Friend::Main()
{
    G.NewScriptFrame();
    if (Term()) return;
    while (!G.StartScriptingEntity()) { G.NewScriptFrame(); if (Term()) return; }
    if (Term()) return;
    while (BrainState == MTBS_NONE) {
        G.NewScriptFrame();
        if (Term()) return;
        if (Talked()) {
            if (Term()) return;
            Pauser p(G);
            if (G.GetHealthHandle() > 0.0f) { Speak("Evening Hero."); if (Term()) return; }
        }
        if (HitBy() || (HitByAbility() && !HitByHeal())) {
            if (Term()) return;
            Pauser p(G);
            if (G.GetHealthHandle() > 0.0f) { Speak("So... you defeated Rhod and tracked me down."); if (Term()) return; }
            if (G.GetHealthHandle() > 0.0f) { Speak("No matter. I came prepared. (spawn some baddies)"); if (Term()) return; }
            BrainState = MTBS_FIGHTING;
        }
    }
    if (Term()) return;
    while (BrainState == MTBS_FIGHTING) {
        G.NewScriptFrame();
        if (Term()) return;
        if (G.GetHealthMe() == 1.0f) {
            if (Term()) return;
            Pauser p(G);
            if (G.GetHealthHandle() > 0.0f) { Speak("And so I die... I failed you Rhod. I am sorry."); if (Term()) return; }
            ParentEvent = MTE_MEET_LEADER;
            BrainState = MTBS_DEAD;
        }
    }
    if (Term()) return;
    G.RemoveThing();
}

int main()
{
    std::memset(&st, 0, sizeof(st));
    st.limit = 100; st.talkAt = 3; st.hitAt = 5; st.lowAt = 9; st.startFail = 2;
    Friend f; f.BrainState = 0; f.ParentEvent = 5;
    f.Main();
    if (st.spoken != 4 || std::strcmp(st.lines[0], "Evening Hero.") != 0 || std::strncmp(st.lines[3], "And so I die", 12) != 0
        || f.BrainState != 3 || f.ParentEvent != 1 || st.removed != 1 || st.pauseDepth != 0 || st.maxPause != 1) {
        std::printf("00eac5b0_TEST FAIL\n"); return 1;
    }
    // a heal-spell hit does not start the fight; terminate before the end
    std::memset(&st, 0, sizeof(st));
    st.limit = 20; st.talkAt = -1; st.hitAt = 4; st.healHit = true; st.lowAt = 1000;
    Friend g; g.BrainState = 0; g.ParentEvent = 5;
    g.Main();
    if (st.spoken != 0 || g.BrainState != 0 || st.removed != 0 || g.ParentEvent != 5) { std::printf("00eac5b0_TEST FAIL\n"); return 1; }
    std::printf("00eac5b0_TEST PASS\n");
    return 0;
}