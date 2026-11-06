// Behaviour test for NScript::CV_MurderTwistScript::CMurderTwistBrotherYounger::Main (0x00EADBB0): while idle, brush the hero off
// when talked to and, when hit by the hero, say the two "it's mine" lines and fight; while fighting, at exactly 1 health count
// the gem as found, say the two surrender lines, point the quest at the leader, become nearly dead, heal 10 and take himself
// off the parent's magic-rune rumble list (first match only); while nearly dead, at 1 health again he dies; then he is removed.
// A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { MTBS_NONE, MTBS_FIGHTING, MTBS_NEARLY_DEAD, MTBS_DEAD };
enum { MTE_NONE = 0, MTE_MEET_LEADER = 8 };
struct World { int Frame, TalkAt, HitAt, OneHealthAt, SecondOneAt, TerminateAt; float Health; };
struct Younger {
    World* W; int BrainState, Event, ItemsFound; int Rumble[4]; int RumbleCount; int MeId; bool Removed;
    bool T() { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    void NewScriptFrame()
    {
        ++W->Frame; Log("frame");
        if (W->Frame == W->OneHealthAt || W->Frame == W->SecondOneAt) W->Health = 1.0f;
    }
    void Speak(const char* s) { if (W->Health > 0.0f) Log(s); }
    void Erase(int i) { for (int j = i; j + 1 < RumbleCount; ++j) Rumble[j] = Rumble[j + 1]; --RumbleCount; }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        while (BrainState == MTBS_NONE) {
            NewScriptFrame();
            if (T()) return;
            if (W->Frame == W->TalkAt) { if (T()) return; Speak("busy"); }
            if (W->Frame == W->HitAt) { if (T()) return; Speak("sent"); Speak("mine"); BrainState = MTBS_FIGHTING; }
        }
        if (T()) return;
        while (BrainState == MTBS_FIGHTING) {
            NewScriptFrame();
            if (T()) return;
            if (W->Health == 1.0f) {
                if (T()) return;
                ++ItemsFound;
                Speak("enough"); Speak("leave");
                Event = MTE_MEET_LEADER;
                BrainState = MTBS_NEARLY_DEAD;
                W->Health += 10.0f; Log("heal");
                for (int i = 0; i < RumbleCount; ++i) if (Rumble[i] == MeId) { Erase(i); break; }
            }
        }
        if (T()) return;
        while (BrainState == MTBS_NEARLY_DEAD) {
            NewScriptFrame();
            if (T()) return;
            if (W->Health == 1.0f) { if (T()) return; BrainState = MTBS_DEAD; }
        }
        if (T()) return;
        Removed = true; Log("remove");
    }
};
int main()
{
    World w = { 0, 2, 3, 5, 7, 0, 50.0f };
    Younger y = { &w, MTBS_NONE, MTE_NONE, 0, { 4, 9, 4, 6 }, 4, 4, false };
    g_log[0] = 0;
    y.Main();
    bool ok = !std::strcmp(g_log, "frame;frame;busy;frame;sent;mine;frame;frame;enough;leave;heal;frame;frame;remove;")
        && y.Event == MTE_MEET_LEADER && y.ItemsFound == 1 && y.RumbleCount == 3 && y.Rumble[0] == 9 && y.Rumble[1] == 4 && y.Removed;
    World w2 = { 0, 0, 2, 0, 0, 4, 50.0f };
    Younger y2 = { &w2, MTBS_NONE, MTE_NONE, 0, { 4 }, 1, 4, false };
    g_log[0] = 0;
    y2.Main();
    ok = ok && !std::strcmp(g_log, "frame;frame;sent;mine;frame;frame;") && !y2.Removed && y2.RumbleCount == 1;
    if (!ok) std::printf("%s\n", g_log);
    std::printf(ok ? "00eadbb0_TEST PASS\n" : "00eadbb0_TEST FAIL\n");
    return ok ? 0 : 1;
}