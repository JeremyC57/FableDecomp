// Behaviour test for NScript::CV_MurderTwistScript::CMurderTwistBrotherOlderFriend::Main (0x00EACF00): while idle, greet the hero
// when talked to ("Evenin' Hero.") and, when hit by the hero (or by a hero special ability other than heal life), say the two
// ambush lines and start fighting; while fighting, at exactly 1 health say the dying line, point the quest at the hidden gem
// and die; then remove the thing. Lines are spoken only while the scripted thing has health; every terminate check returns.
// A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { MTBS_NONE, MTBS_FIGHTING, MTBS_NEARLY_DEAD, MTBS_DEAD };
enum { MTE_NONE = 0, MTE_FIND_HIDDEN_GEM = 7 };
struct World {
    int Frame, TalkAt, HitAt, HealAbilityHit, OneHealthAt, TerminateAt;
    float Health;
};
struct Friend {
    World* W; int BrainState; int Event; bool Removed;
    bool T() { return W->Frame >= W->TerminateAt && W->TerminateAt > 0; }
    void NewScriptFrame() { ++W->Frame; Log("frame"); if (W->OneHealthAt && W->Frame >= W->OneHealthAt) W->Health = 1.0f; }
    void Speak(const char* s) { Log(s); }
    bool Talked() { return W->Frame == W->TalkAt; }
    bool Hit() { return W->Frame == W->HitAt && !W->HealAbilityHit; }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        while (BrainState == MTBS_NONE) {
            NewScriptFrame();
            if (T()) return;
            if (Talked()) { if (T()) return; if (W->Health > 0.0f) Speak("Evenin' Hero."); }
            if (Hit()) {
                if (T()) return;
                if (W->Health > 0.0f) Speak("So... you defeated Mike and tracked me down.");
                if (W->Health > 0.0f) Speak("No matter. I came prepared. (spawn some HARDER baddies)");
                BrainState = MTBS_FIGHTING;
            }
        }
        if (T()) return;
        while (BrainState == MTBS_FIGHTING) {
            NewScriptFrame();
            if (T()) return;
            if (W->Health == 1.0f) {
                if (T()) return;
                if (W->Health > 0.0f) Speak("acck");
                Event = MTE_FIND_HIDDEN_GEM;
                BrainState = MTBS_DEAD;
            }
        }
        if (T()) return;
        Removed = true; Log("remove");
    }
};
static bool Run(int talk, int hit, int heal, int one, int term, int event, bool removed, const char* expect)
{
    World w = { 0, talk, hit, heal, one, term, 50.0f };
    Friend f = { &w, MTBS_NONE, MTE_NONE, false };
    g_log[0] = 0;
    f.Main();
    if (std::strcmp(g_log, expect) || f.Event != event || f.Removed != removed) { std::printf("%s\n", g_log); return false; }
    return true;
}
int main()
{
    bool ok =
        Run(2, 3, 0, 5, 0, MTE_FIND_HIDDEN_GEM, true,
            "frame;frame;Evenin' Hero.;frame;So... you defeated Mike and tracked me down.;No matter. I came prepared. (spawn some HARDER baddies);frame;frame;acck;remove;")
        && Run(0, 2, 1, 0, 4, MTE_NONE, false, "frame;frame;frame;frame;")          // a heal-life hit is ignored
        && Run(0, 2, 0, 0, 4, MTE_NONE, false,
               "frame;frame;So... you defeated Mike and tracked me down.;No matter. I came prepared. (spawn some HARDER baddies);frame;frame;");
    std::printf(ok ? "00eacf00_TEST PASS\n" : "00eacf00_TEST FAIL\n");
    return ok ? 0 : 1;
}