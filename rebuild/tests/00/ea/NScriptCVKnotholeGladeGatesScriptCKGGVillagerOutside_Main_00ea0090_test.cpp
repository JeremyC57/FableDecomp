// Behaviour test for NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Main (0x00EA0090): a villager already attacked
// only idles until terminated; otherwise it is scripted (priority 3), offers information, and until the balverines are killed:
// when the hero talks to it, it says the male / female intro line (only while alive), when the hero is within 30 of the gate and
// rand() % 200 == 0 it panics in a conversation, otherwise, while the gates are shut, it looks at and attacks the gate and, if the
// hero killed a villager, panics about the hero; afterwards the information is cleared and the scripted handle released.
// A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World { int Frame, TalkAt, NearAt, KilledBalvsAt, TerminateAt; bool GatesOpened, HeroKilledAVillager; };
struct Villager {
    World* W; bool Attacked, Male;
    bool T() { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    void NewScriptFrame() { ++W->Frame; Log("frame"); }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        if (Attacked) { if (T()) return; for (;;) { NewScriptFrame(); if (T()) return; } }
        Log("script3");
        Log("info");
        while (W->Frame < W->KilledBalvsAt) {
            NewScriptFrame();
            if (T()) return;
            if (W->Frame == W->TalkAt) {
                if (T()) return;
                Log(Male ? "TEXT_QST_050_VILLAGER_MALE_INTRO" : "TEXT_QST_050_VILLAGER_FEMALE_INTRO");
            } else if (W->Frame == W->NearAt) {
                if (T()) return;
                Log(Male ? "TEXT_QST_050_VILLAGER_MALE_PANIC" : "TEXT_QST_050_VILLAGER_FEMALE_PANIC");
            } else {
                if (T()) return;
                if (!W->GatesOpened) {
                    if (T()) return;
                    Log("attack_gate");
                    if (W->HeroKilledAVillager) {
                        if (T()) return;
                        Log(Male ? "TEXT_QST_050_VILLAGER_MALE_PANIC_ABOUT_HERO" : "TEXT_QST_050_VILLAGER_FEMALE_PANIC_ABOUT_HERO");
                    }
                }
            }
        }
        if (T()) return;
        Log("clear_info");
    }
};
int main()
{
    World w = { 0, 2, 3, 4, 0, false, true };
    Villager v = { &w, false, true };
    g_log[0] = 0; v.Main();
    bool ok = !std::strcmp(g_log, "frame;script3;info;frame;TEXT_QST_050_VILLAGER_MALE_INTRO;frame;TEXT_QST_050_VILLAGER_MALE_PANIC;"
                                  "frame;attack_gate;TEXT_QST_050_VILLAGER_MALE_PANIC_ABOUT_HERO;clear_info;");
    World w2 = { 0, 0, 0, 3, 0, true, false };
    Villager v2 = { &w2, false, false };
    g_log[0] = 0; v2.Main();
    ok = ok && !std::strcmp(g_log, "frame;script3;info;frame;frame;clear_info;");
    World w3 = { 0, 0, 0, 9, 3, false, false };
    Villager v3 = { &w3, true, false };
    g_log[0] = 0; v3.Main();
    ok = ok && !std::strcmp(g_log, "frame;frame;frame;");
    std::printf(ok ? "00ea0090_TEST PASS\n" : "00ea0090_TEST FAIL\n");
    return ok ? 0 : 1;
}