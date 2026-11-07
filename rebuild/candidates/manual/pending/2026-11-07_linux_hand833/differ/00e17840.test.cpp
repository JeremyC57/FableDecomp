// Behaviour test for NScript::CQ_WhiteBalverineKnotholeGladeScript::CKG_Chief::Main (0x00E17840): the Knothole Glade chief. After
// setup (not combo / stat counted, not pushable, unkillable, no information marker) he loops while the thread lives: a talk picks the
// line for ParentClass.BalverineState (2 IN_FIRST_ATTACK, 3 AFTER_FIRST_ATTACK, 4 IN_SECOND_ATTACK, 6 IN_THIRD_ATTACK, 7
// AFTER_THIRD_ATTACK only while Q_WhiteBalverineWW is not active, anything else silent); a hero hit (not a heal-life spell) before
// state 7 says BEEN_ATTACKED and sets the friends-with-everything flag. A state model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Chief {
    int Frames, TerminateAt, TalkAt, HitAt, HealAt, State;
    bool WWActive, Friends;
    void NewScriptFrame() { ++Frames; }
    bool T() const { return Frames >= TerminateAt; }
    const char* TalkLine() const
    {
        if (State == 2) return "TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK";
        if (State == 3) return "TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK";
        if (State == 4) return "TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK";
        if (State == 6) return "TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK";
        if (State == 7 && !WWActive) return "TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK";
        return 0;
    }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        Log("setup");
        while (!T()) {
            if (Frames == TalkAt) {
                const char* line = TalkLine();
                if (line) { Log(line); NewScriptFrame(); if (T()) return; }
            }
            if (Frames == HitAt && Frames != HealAt && State != 7) {
                NewScriptFrame();
                if (T()) return;
                Log("TEXT_QST_074_CHIEF_BEEN_ATTACKED");
                NewScriptFrame();
                if (T()) return;
                Friends = true;
            }
            NewScriptFrame();
        }
    }
};
static bool Run(int state, bool ww, int talk, int hit, int heal, const char* expect, bool friends)
{
    g_log[0] = 0;
    Chief c = { 0, 20, talk, hit, heal, state, ww, false };
    c.Main();
    return !std::strcmp(g_log, expect) && c.Friends == friends;
}
int main()
{
    bool ok = Run(2, false, 3, -1, -1, "setup;TEXT_QST_074_CHIEF_BALVERINE_IN_FIRST_ATTACK;", false);
    ok = ok && Run(3, false, 3, -1, -1, "setup;TEXT_QST_074_CHIEF_BALVERINE_AFTER_FIRST_ATTACK;", false);
    ok = ok && Run(4, false, 3, -1, -1, "setup;TEXT_QST_074_CHIEF_BALVERINE_IN_SECOND_ATTACK;", false);
    ok = ok && Run(6, false, 3, -1, -1, "setup;TEXT_QST_074_CHIEF_BALVERINE_IN_THIRD_ATTACK;", false);
    ok = ok && Run(7, false, 3, -1, -1, "setup;TEXT_QST_074_CHIEF_BALVERINE_AFTER_THIRD_ATTACK;", false);
    ok = ok && Run(7, true, 3, -1, -1, "setup;", false);
    ok = ok && Run(5, false, 3, -1, -1, "setup;", false);
    ok = ok && Run(2, false, -1, 3, -1, "setup;TEXT_QST_074_CHIEF_BEEN_ATTACKED;", true);
    ok = ok && Run(2, false, -1, 3, 3, "setup;", false);
    ok = ok && Run(7, false, -1, 3, -1, "setup;", false);
    std::printf(ok ? "00e17840_TEST PASS\n" : "00e17840_TEST FAIL\n");
    return ok ? 0 : 1;
}
