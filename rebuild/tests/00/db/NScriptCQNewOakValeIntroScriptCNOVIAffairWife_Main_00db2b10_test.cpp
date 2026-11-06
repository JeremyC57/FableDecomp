// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWife::Main (0x00DB2B10): the wronged wife. Before she goes for
// her husband: a hit makes her an ally of the hero, costs a bad deed and plays ON_HIT; a talk plays LAYABOUT and, once the hero has
// discovered the infidelity, asks the yes/no QUESTION (yes: THANKYOU, good deed, GoingForHusband; no: PLEA); the intermittent timer
// makes her ask WHERES_HUSBAND (forced the first time). Going for him: she runs to the husband, then argues, numbering her
// WHATS_THIS_<n> conversation lines 10, 20, ... and falling back to _10 when the text entry does not exist. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Wife {
    int Frames, TerminateAt, HitAt, TalkAt[4], NTalk, Answer, MaxEntry, Timer, ConvLeft;
    bool Discovered, Going, ForceFirst, Ally; int BadDeeds, GoodDeeds;
    void NewScriptFrame() { ++Frames; if (Timer > 0) --Timer; if (ConvLeft > 0) --ConvLeft; }
    bool T() const { return Frames >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) return true; return false; }
    void Main()
    {
        for (;; NewScriptFrame()) {
            if (T()) return;
            if (Going) {
                Log("run_to_husband");
                int index = 0;
                for (;; NewScriptFrame()) {
                    if (T()) return;
                    if (ConvLeft == 0) {
                        index = index + 10;
                        char line[64]; std::sprintf(line, "WHATS_THIS_%d", index);
                        if (index > MaxEntry) { index = 10; std::strcpy(line, "WHATS_THIS_10"); }
                        Log(line); ConvLeft = 2;
                    }
                }
            }
            if (Frames == HitAt) { Ally = true; ++BadDeeds; Log("ON_HIT"); }
            else if (Talked()) {
                Log("LAYABOUT");
                if (Discovered) {
                    Log("QUESTION");
                    if (Answer == 1) { Log("THANKYOU"); ++GoodDeeds; Going = true; }
                    else Log("PLEA");
                }
            }
            if (!Going && Timer == 0 && ForceFirst) { Timer = 3; ForceFirst = false; Log("WHERES_HUSBAND"); }
        }
    }
};
int main()
{
    Wife a = { 0, 12, 1, { 2, 4 }, 2, 1, 20, 0, 0, false, false, true, false, 0, 0 };
    a.Main();
    // two talks without the discovery: no question
    bool ok = !std::strcmp(g_log, "WHERES_HUSBAND;ON_HIT;LAYABOUT;LAYABOUT;") && a.BadDeeds == 1 && a.Ally && !a.Going;
    g_log[0] = 0;
    Wife b = { 0, 12, -1, { 1 }, 1, 1, 20, 0, 0, true, false, false, false, 0, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "LAYABOUT;QUESTION;THANKYOU;run_to_husband;WHATS_THIS_10;WHATS_THIS_20;WHATS_THIS_10;WHATS_THIS_20;WHATS_THIS_10;")
        && b.Going && b.GoodDeeds == 1;
    g_log[0] = 0;
    Wife c = { 0, 5, -1, { 1 }, 1, 0, 20, 0, 0, true, false, false, false, 0, 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "LAYABOUT;QUESTION;PLEA;") && !c.Going;
    std::printf(ok ? "00db2b10_TEST PASS\n" : "00db2b10_TEST FAIL\n");
    return ok ? 0 : 1;
}