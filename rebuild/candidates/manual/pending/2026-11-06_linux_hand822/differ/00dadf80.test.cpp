// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main (0x00DADF80): per frame, a hero hit (not a heal spell)
// makes allies, counts a bad deed and sets HeroDidHitMe (complaint only with health left); a talk gives the bad-deeds or spoken-to
// line with the villager's sex suffix; otherwise, with the talk timer clear, 1 in 100 and the hero near, the timer restarts at 3 and a
// line is picked from the speech list chosen by sex and the good / bad deed tally. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void Log2(const char* a, const char* b) { std::strcat(g_log, a); std::strcat(g_log, b); std::strcat(g_log, ";"); }
struct Villager {
    bool HeroDidHitMe, Male, Terminate;
    int Frames, Good, Bad, Timer, BadDeeds, HitAt, TalkAt, ChatAt;
    float Health;
    void NewScriptFrame() { ++Frames; if (Timer > 0) --Timer; if (Frames >= 6) Terminate = true; }
    bool IsActiveThreadTerminating() { return Terminate; }
    const char* List()
    {
        if (Bad == 0 && Good > 0) return Male ? "MaleGood" : "FemaleGood";
        if (Bad > 0 && Good == 0) return Male ? "MaleBad" : "FemaleBad";
        if (Bad > 0 && Good > 0) return Male ? "MaleBoth" : "FemaleBoth";
        return Male ? "MaleNeither" : "FemaleNeither";
    }
    void Main()
    {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!IsActiveThreadTerminating()) {
            if (Frames == HitAt) {
                Log("allies"); ++BadDeeds; ++Bad; HeroDidHitMe = true;
                if (Health > 0.0f) Log(Male ? "ATTACKED_MALE" : "ATTACKED_FEMALE");
            } else if (Frames == TalkAt) {
                const char* suffix = Male ? "_MALE" : "_FEMALE";
                Log2(HeroDidHitMe ? "DONE_BAD_DEEDS" : "SPOKEN_TO", suffix);
            } else if (Timer == 0 && Frames == ChatAt) {
                Timer = 3;
                Log(List());
            }
            NewScriptFrame();
        }
    }
};
int main()
{
    Villager a = { false, true, false, 0, 1, 0, 0, 0, 2, 3, 4, 10.0f };
    a.Main();
    bool ok = !std::strcmp(g_log, "allies;ATTACKED_MALE;DONE_BAD_DEEDS_MALE;MaleBoth;") && a.HeroDidHitMe && a.BadDeeds == 1 && a.Timer == 1;
    g_log[0] = 0;
    Villager b = { false, false, false, 0, 0, 0, 0, 0, 9, 2, 3, 10.0f };
    b.Main();
    ok = ok && !std::strcmp(g_log, "SPOKEN_TO_FEMALE;FemaleNeither;") && !b.HeroDidHitMe;
    g_log[0] = 0;
    Villager c = { false, true, false, 0, 0, 0, 0, 0, 1, 9, 9, 0.0f };
    c.Main();
    ok = ok && !std::strcmp(g_log, "allies;") && c.HeroDidHitMe;
    std::printf(ok ? "00dadf80_TEST PASS\n" : "00dadf80_TEST FAIL\n");
    return ok ? 0 : 1;
}
