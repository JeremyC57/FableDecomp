// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main (0x00D0A830): while the fight is not over, record the
// king's health and taunt once below three quarters and once below half of the starting health (each only when the 8-second taunt
// timer has run out); then beg every ten seconds (lines 10, 20, 30, 40, 50, then 40 / 50 alternating after 30); when the hero hits him
// say the "over" line, flag TwinBladeAttacked with AngryBanditNeeded 4 and track his health on a bar until he dies. A state model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct King {
    long Health, KingHealth; int Frame; long Timer; bool ItsAllOver, TwinBladeAttacked; long AngryBanditNeeded; int Bars;
    void Main()
    {
        long StartHealth = Health;
        bool FirstTaunt = false, SecondTaunt = false;
        long TauntTimer = 0;
        while (!ItsAllOver) {
            ++Frame; if (TauntTimer > 0) --TauntTimer;
            Health = Frame < 10 ? 100 - Frame * 10 : 30;
            if (Frame == 12) ItsAllOver = true;
            KingHealth = Health;
            if (KingHealth < StartHealth * 3 / 4 && !FirstTaunt && TauntTimer <= 0) { Log("TAUNT_10"); FirstTaunt = true; TauntTimer = 8; }
            if (KingHealth < StartHealth / 2 && !SecondTaunt && TauntTimer <= 0) { Log("TAUNT_20"); SecondTaunt = true; TauntTimer = 8; }
        }
        static const char* const beg[5] = { "BEG_10", "BEG_20", "BEG_30", "BEG_40", "BEG_50" };
        static const long next[5] = { 1, 2, 3, 4, 3 };
        long BegCount = 0;
        for (int i = 0; i < 7; ++i) { Log(beg[BegCount]); BegCount = next[BegCount]; }
        Log("OVER"); TwinBladeAttacked = true; AngryBanditNeeded = 4;
        while (Health > 0) { Health -= 10; ++Bars; }
    }
};
int main()
{
    King k = { 100, 0, 0, 0, false, false, 0, 0 };
    k.Main();
    bool ok = !std::strcmp(g_log, "TAUNT_10;TAUNT_20;BEG_10;BEG_20;BEG_30;BEG_40;BEG_50;BEG_40;BEG_50;OVER;")
        && k.TwinBladeAttacked && k.AngryBanditNeeded == 4 && k.Bars == 3 && k.KingHealth == 30;
    std::printf(ok ? "00d0a830_TEST PASS\n" : "00d0a830_TEST FAIL\n");
    return ok ? 0 : 1;
}
