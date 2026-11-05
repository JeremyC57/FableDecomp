// Behaviour test for NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoDealWithHero (0x00EB4760): the movie and
// pause wrap the whole dialogue; ATTACKED line, first bribe question; yes -> BRIBE_ACCEPTED, 2000 gold, THANKS, state 3 +
// morality + KILL_SHERIFF objective; no -> BRIBE2 line and a second question; yes -> 2500 gold, state 3 tail; no -> DECLINED,
// state 2. A dead assassin skips the lines, a terminating thread returns at once (pause released, movie destroyed).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Game {
    int answers[2]; int asked; int waitFrames; bool alive; int termAfterFrames; int frames;
    long Answer() { if (waitFrames > 0) { --waitFrames; return -1; } return answers[asked - 1]; }
};
struct Assassin {
    Game* g; int AssassinState; bool paused; bool movie;
    bool Term() { return g->termAfterFrames >= 0 && g->frames >= g->termAfterFrames; }
    void Frame() { ++g->frames; }
    bool SpeakLine(const char* t)   // false = terminated
    {
        if (g->alive) {
            Log(t);
            Frame();                    // one IsPerformingScriptTask frame
            if (Term()) return false;
            if (Term()) return false;
        }
        return true;
    }
    bool Ask(const char* q, long& a)
    {
        Log(q); ++g->asked; a = g->Answer();
        while (a < 0) { Frame(); if (Term()) return false; a = g->Answer(); }
        return !Term();
    }
    void Done() { paused = false; movie = false; Log("unpause"); }
    void DoDealWithHero()
    {
        movie = true; paused = true; Log("movie");
        long a;
        if (!SpeakLine("ATTACKED") || !Ask("BRIBE_QUESTION", a) || Term()) { Done(); return; }
        if (a == 1) {
            if (!SpeakLine("BRIBE_ACCEPTED")) { Done(); return; }
            Log("gold2000");
            if (!SpeakLine("BRIBE_ACCEPTED_THANKS")) { Done(); return; }
            AssassinState = 3; Log("morality"); Log("KILL_SHERIFF");
        } else {
            if (!SpeakLine("BRIBE2") || !Ask("BRIBE2_QUESTION", a) || Term()) { Done(); return; }
            if (a == 1) {
                if (!SpeakLine("BRIBE2_ACCEPTED")) { Done(); return; }
                Log("gold2500");
                if (!SpeakLine("BRIBE2_ACCEPTED_THANKS")) { Done(); return; }
                AssassinState = 3; Log("morality"); Log("KILL_SHERIFF");
            } else {
                if (!SpeakLine("BRIBE2_DECLINED")) { Done(); return; }
                AssassinState = 2;
            }
        }
        Done();
    }
};
static bool Run(int a1, int a2, bool alive, int term, int state, const char* expect)
{
    Game g; g.answers[0] = a1; g.answers[1] = a2; g.asked = 0; g.waitFrames = 1; g.alive = alive; g.termAfterFrames = term; g.frames = 0;
    Assassin s; s.g = &g; s.AssassinState = 1; s.paused = false; s.movie = false;
    g_log[0] = 0;
    s.DoDealWithHero();
    if (s.paused || s.movie || s.AssassinState != state || std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return true;
}
int main()
{
    bool ok = Run(1, 0, true, -1, 3, "movie;ATTACKED;BRIBE_QUESTION;BRIBE_ACCEPTED;gold2000;BRIBE_ACCEPTED_THANKS;morality;KILL_SHERIFF;unpause;")
        && Run(0, 1, true, -1, 3, "movie;ATTACKED;BRIBE_QUESTION;BRIBE2;BRIBE2_QUESTION;BRIBE2_ACCEPTED;gold2500;BRIBE2_ACCEPTED_THANKS;morality;KILL_SHERIFF;unpause;")
        && Run(0, 0, true, -1, 2, "movie;ATTACKED;BRIBE_QUESTION;BRIBE2;BRIBE2_QUESTION;BRIBE2_DECLINED;unpause;")
        && Run(0, 0, false, -1, 2, "movie;BRIBE_QUESTION;BRIBE2_QUESTION;unpause;")
        && Run(1, 0, true, 1, 1, "movie;ATTACKED;unpause;");
    if (!ok) { std::printf("00eb4760_TEST FAIL\n"); return 1; }
    std::printf("00eb4760_TEST PASS\n");
    return 0;
}