// Behaviour test for 0x00EEC410 CGlobal_GiveHeroItemsFromRewardChestScript::Main: hand over the used chest's items, wait for player control, show tutorial 10 after the reward-on-death chest for an adult hero, then deactivate the quest.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_termAt, g_termCalls;
static bool Term() { ++g_termCalls; return g_termCalls == g_termAt; }
static const char* g_ans;
static int g_ansIdx;
static bool Ans(const char* what)
{
    Log(what);
    char c = g_ans[g_ansIdx];
    if (c) ++g_ansIdx;
    return c == 'y';
}
static void Reset(const char* answers, int termAt)
{
    g_log[0] = 0; g_ans = answers; g_ansIdx = 0; g_termAt = termAt; g_termCalls = 0;
}
static int Check(const char* expect)
{
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\nexp %s\n", g_log, expect); return 1; }
    return 0;
}
#include <string>
static void Frame() { Log("frame"); }
static const char* g_def;
static bool DefIs(const char* name)
{
    std::string def = g_def ? g_def : "";
    if (def == name) return true;
    return false;
}
struct CGlobal_GiveHeroItemsFromRewardChestScript {
    void Main()
    {
        Log("pred"); Frame();
        if (Term()) return;
        Log("persistent");
        bool rewardChest = false;
        Log("target");
        if (Ans("alive?")) {
            if (Term()) return;
            if (DefIs("OBJECT_CHEST_REWARD_ON_DEATH")) { if (Term()) return; rewardChest = true; }
            Log("giveItems"); Log("remove");
        }
        while (!Ans("control?")) { Frame(); if (Term()) return; }
        if (Term()) return;
        if (rewardChest) {
            if (Term()) return;
            if (!Ans("child?")) {
                if (Term()) return;
                if (Ans("tutorial10")) {
                    if (Term()) return;
                    while (!Ans("clicked?")) { Frame(); if (Term()) return; }
                    if (Term()) return;
                }
            }
        }
        Log("deactivate");
    }
};
int main()
{
    int bad = 0; CGlobal_GiveHeroItemsFromRewardChestScript s;
    g_def = "OBJECT_CHEST_REWARD_ON_DEATH"; Reset("ynynyny", 0); s.Main(); bad += Check("pred;frame;persistent;target;alive?;giveItems;remove;control?;frame;control?;child?;tutorial10;clicked?;frame;clicked?;deactivate;");
    g_def = "OBJECT_CHEST"; Reset("yy", 0); s.Main(); bad += Check("pred;frame;persistent;target;alive?;giveItems;remove;control?;deactivate;");
    g_def = 0; Reset("n" "y", 0); s.Main(); bad += Check("pred;frame;persistent;target;alive?;control?;deactivate;");
    g_def = "OBJECT_CHEST_REWARD_ON_DEATH"; Reset("yyy", 0); s.Main(); bad += Check("pred;frame;persistent;target;alive?;giveItems;remove;control?;child?;deactivate;");
    if (bad) { std::printf("00eec410_TEST FAIL\n"); return 1; }
    std::printf("00eec410_TEST PASS\n"); return 0;
}