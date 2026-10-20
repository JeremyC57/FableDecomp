// Behaviour test for CV_FisticuffsClubScript::IsKnotholeGladeNotAPunchClub (0x00E84A20): the quest-state truth table,
// short-circuit order of the queries, and a terminating thread reporting true.
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
static int g_calls; static bool g_term;
struct CGameScriptInterface {
    bool Completed, Siege, Ransom, Thunder;
    bool IsQuestCompleted(const CCharString& n) const { ++g_calls; return std::strcmp(n.Text, "Q_WhiteBalverineKnotholeGlade") == 0 && Completed; }
    bool IsQuestActive(const CCharString& n) const
    {
        ++g_calls;
        if (std::strcmp(n.Text, "Q_BreakSiege") == 0) return Siege;
        if (std::strcmp(n.Text, "Q_RansomVictim") == 0) return Ransom;
        if (std::strcmp(n.Text, "Q_HeroSoulsThunder") == 0) return Thunder;
        return false;
    }
};
struct CV_FisticuffsClubScript {
    CGameScriptInterface* Game;
    bool IsActiveThreadTerminating() { return g_term; }
    bool IsKnotholeGladeNotAPunchClub()
    {
        bool notAPunchClub = !Game->IsQuestCompleted("Q_WhiteBalverineKnotholeGlade") || Game->IsQuestActive("Q_BreakSiege")
            || Game->IsQuestActive("Q_RansomVictim") || Game->IsQuestActive("Q_HeroSoulsThunder");
        if (notAPunchClub) {
            if (IsActiveThreadTerminating())
                return true;
            return true;
        }
        if (IsActiveThreadTerminating())
            return true;
        return false;
    }
};
static bool run(bool c, bool s, bool r, bool t, bool term, bool want, int wantCalls)
{
    CGameScriptInterface g = { c, s, r, t }; CV_FisticuffsClubScript x; x.Game = &g; g_calls = 0; g_term = term;
    return x.IsKnotholeGladeNotAPunchClub() == want && g_calls == wantCalls;
}
int main()
{
    bool ok = run(false, false, false, false, false, true, 1) && run(true, true, false, false, false, true, 2)
        && run(true, false, true, false, false, true, 3) && run(true, false, false, true, false, true, 4)
        && run(true, false, false, false, false, false, 4) && run(true, false, false, false, true, true, 4);
    if (!ok) { std::printf("00e84a20_TEST FAIL\n"); return 1; }
    std::printf("00e84a20_TEST PASS\n");
    return 0;
}