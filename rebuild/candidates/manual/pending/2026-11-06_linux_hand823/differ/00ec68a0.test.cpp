// Behaviour test for NScript::CV_SickChildScript::CSickChildsSister::Main (0x00EC68A0): the sick child's sister. Until she has led
// the hero to her mother (and the mother intro is not done) she follows the hero, calls him over (CALLS_OVER_10) with an attention
// animation whenever her ten-second timer runs out and says ON_HIT when hit; talked to, she says TOLD_TO_FOLLOW_20 and runs to her
// mother, waving THIS_WAY when the hero falls more than 15 units behind (once per five-second wave timer). Afterwards each talk gets
// THANKS (quest finished), REMINDER (intro not done) or the numbered REMINDER_POSTINTRO_<n> (10, 20, ... back to 10 when the text
// entry does not exist). A state model with distances as numbers on a line.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Sister {
    int Frames, TerminateAt, TalkAt[4], NTalk, HitAt, MaxEntry; float Me, Hero, Mother, HeroSpeed;
    bool LeadHero, MotherIntroDone, FinishedQuest; int Waves;
    bool T() const { return Frames >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) return true; return false; }
    void Main()
    {
        if (!LeadHero && !MotherIntroDone) {
            int call_timer = 0;
            while (!Talked() && !MotherIntroDone) {
                ++Frames; if (call_timer > 0) --call_timer;
                if (T()) return;
                if (call_timer == 0) { Log("CALLS_OVER_10"); Log("attention_anim"); call_timer = 10; }
                if (Frames == HitAt) Log("ON_HIT");
            }
            if (!MotherIntroDone) {
                Log("TOLD_TO_FOLLOW_20");
                bool waved = false; int wave_timer = 0;
                while (Mother - Me > 2.0f) {
                    ++Frames; if (wave_timer > 0) --wave_timer; Hero += HeroSpeed;
                    if (T()) return;
                    if (Me - Hero > 15.0f) { if (!waved) { Log("THIS_WAY"); ++Waves; waved = true; wave_timer = 5; } else if (wave_timer == 0) waved = false; }
                    else { Me += 2.0f; waved = false; }
                }
                LeadHero = true;
            }
        }
        int reminder = 10;
        for (;; ++Frames) {
            if (T()) return;
            if (Talked()) {
                if (FinishedQuest) Log("THANKS");
                else if (!MotherIntroDone) Log("REMINDER");
                else {
                    if (reminder > MaxEntry) reminder = 10;
                    char line[48]; std::sprintf(line, "REMINDER_POSTINTRO_%d", reminder); Log(line); reminder += 10;
                }
            }
        }
    }
};
int main()
{
    Sister a = { 0, 60, { 12, 50, 55 }, 3, 5, 20, 0.0f, 0.0f, 40.0f, 1.0f };
    a.Main();
    bool ok = !std::strcmp(g_log, "CALLS_OVER_10;attention_anim;ON_HIT;CALLS_OVER_10;attention_anim;TOLD_TO_FOLLOW_20;THIS_WAY;THIS_WAY;REMINDER;REMINDER;")
        && a.LeadHero && a.Waves == 2;
    g_log[0] = 0;
    Sister b = { 0, 40, { 2, 3, 4, 5 }, 4, -1, 20, 0.0f, 0.0f, 10.0f, 0.0f, true, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "REMINDER_POSTINTRO_10;REMINDER_POSTINTRO_20;REMINDER_POSTINTRO_10;REMINDER_POSTINTRO_20;");
    g_log[0] = 0;
    Sister c = { 0, 40, { 2 }, 1, -1, 20, 0.0f, 0.0f, 10.0f, 0.0f, true, true, true };
    c.Main();
    ok = ok && !std::strcmp(g_log, "THANKS;") && c.Frames == 40;
    std::printf(ok ? "00ec68a0_TEST PASS\n" : "00ec68a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
