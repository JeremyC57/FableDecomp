// Behaviour test for CGuardHoldingBandit::Main (0x00D1E3E0): talk / hit / follow-expression reactions and the exit order.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Parent { bool BanditLeaderFree, EndCutsceneDone; };
struct Sim { int frame, talkAt, hitAt, hitAbility, exprAt, freeAt, doneAt; const char* expr; };
static Sim s;
struct Guard {
    Parent* P;
    const char* data;
    void NewScriptFrame()
    {
        ++s.frame;
        if (s.frame == s.freeAt) P->BanditLeaderFree = true;
        if (s.frame == s.doneAt) P->EndCutsceneDone = true;
    }
    bool IsActiveThreadTerminating() const { return s.frame > 60; }
    void Line(const char* suffix)
    {
        char buf[64];
        std::strcpy(buf, "TEXT_QST_018_"); std::strcat(buf, data); std::strcat(buf, suffix);
        Log(buf);
    }
    void Main();
};
void Guard::Main()
{
    Log("heal");
    while (!P->BanditLeaderFree && !P->EndCutsceneDone) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (s.frame == s.talkAt) Line("_STAY_BACK");
        bool hit = s.frame == s.hitAt && (s.hitAbility < 0 || s.hitAbility != 14);
        if (hit) { Line("_ON_HIT"); Log("ally"); }
        if (s.frame == s.exprAt) {
            if (std::strcmp(s.expr, "EXPRESSION_FOLLOW") == 0) Line("_WONT_FOLLOW");
        }
    }
    Log("killable");
    while (!P->EndCutsceneDone) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    Log("remove");
}
int main()
{
    Parent p = { false, false };
    Sim z = { 0, 2, 3, 14, 4, -1, 6, "EXPRESSION_FOLLOW" };
    s = z; g_log[0] = 0;
    Guard g; g.P = &p; g.data = "GUARD1";
    g.Main();
    bool ok = std::strcmp(g_log, "heal;TEXT_QST_018_GUARD1_STAY_BACK;TEXT_QST_018_GUARD1_WONT_FOLLOW;killable;remove;") == 0;
    Parent p2 = { false, false };
    Sim z2 = { 0, -1, 3, -1, 4, 5, 8, "EXPRESSION_WAIT" };
    s = z2; g_log[0] = 0; g.P = &p2;
    g.Main();
    ok = ok && std::strcmp(g_log, "heal;TEXT_QST_018_GUARD1_ON_HIT;ally;killable;remove;") == 0;
    if (!ok) {
        std::printf("00d1e3e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00d1e3e0_TEST PASS\n");
    return 0;
}
