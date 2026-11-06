// Behaviour test for NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main (0x00CF68C0): before the intro, a talk greets the hero
// and, with Q_Arena active, asks the question (yes: refuse while a non-summoned follower is with the hero, else let him into the
// Hall of Heroes; no: the return line); without the quest it reports completed / need-quest / need-more; any talk sets IntroDone.
// A hit outside a hit cut-scene complains and heals. After the intro the return lines are used, and once Q_Arena is complete the
// guard clears his information marker once (DoneArena). A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World { int Frame, TerminateAt, Talk[4], HitAt; bool Active, Completed, WhiteBalverine; int Answer; int Followers[3]; int NFollowers; bool InHit; };
struct Guard {
    World* W; bool IntroDone, DoneArena;
    bool T() { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    bool Talked() { for (int i = 0; i < 4; ++i) if (W->Talk[i] == W->Frame) return true; return false; }
    void Enter(bool ret)
    {
        bool has = false;
        for (int i = 0; i < W->NFollowers; ++i) if (!W->Followers[i]) has = true;   // 0 = not summoned
        if (has) Log("nofollowers"); else Log("comein");
        (void)ret;
    }
    void Talk(bool ret)
    {
        if (!ret) Log("greeting");
        if (W->Active) { Log(ret ? "returngreeting" : "entry"); if (W->Answer == 1) Enter(ret); else Log("return"); }
        else if (W->Completed) Log(ret ? "rwelldone" : "welldone");
        else if (W->WhiteBalverine) Log(ret ? "rneedquest" : "needquest");
        else Log(ret ? "rneedmore" : "needmore");
    }
    void Main()
    {
        while (!IntroDone) {
            ++W->Frame; if (T()) return;
            if (Talked()) { Talk(false); IntroDone = true; }
            if (W->Frame == W->HitAt && !W->InHit) Log("attacked");
        }
        if (T()) return;
        for (; !T(); ++W->Frame) {
            if (Talked()) Talk(true);
            if (W->Frame == W->HitAt && !W->InHit) Log("attacked");
            if (!DoneArena && W->Completed) { Log("clearmarker"); DoneArena = true; }
        }
    }
};
int main()
{
    World w = { 0, 12, { 2, 5, 0, 0 }, 3, true, false, false, 1, { 0 }, 1, false };
    Guard g = { &w, false, false };
    g_log[0] = 0; g.Main();
    bool ok = !std::strcmp(g_log, "greeting;entry;nofollowers;returngreeting;nofollowers;attacked;returngreeting;nofollowers;") && g.IntroDone && !g.DoneArena;
    World w2 = { 0, 8, { 2, 0, 0, 0 }, 0, false, true, false, 0, { 1 }, 1, false };
    Guard g2 = { &w2, false, false };
    g_log[0] = 0; g2.Main();
    ok = ok && !std::strcmp(g_log, "greeting;welldone;rwelldone;clearmarker;") && g2.DoneArena;
    if (!ok) std::printf("%s\n", g_log);
    std::printf(ok ? "00cf68c0_TEST PASS\n" : "00cf68c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
