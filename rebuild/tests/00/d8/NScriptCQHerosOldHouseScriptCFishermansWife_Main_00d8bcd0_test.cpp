// Behaviour test for NScript::CQ_HerosOldHouseScript::CFishermansWife::Main (0x00D8BCD0): until she leaves happy, every 15 frames she
// cries to a near hero; a talk gives the thanks (ghost helped), the 500-gold question (booty dug up, enough gold: yes = success
// cut-scene, -500 gold, leave happy; no = repeat greeting / intro) or the plain intro / repeat greeting; a hero hit (not a heal
// spell) makes her run to the leave marker and fade out.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Wife {
    int Frames, TerminateAt, Timer, Gold, TalkMask, HitMask, HealMask, Answer;
    bool Helping, Helped, BootyDug, TalkedTo, LeavingHappy, Near, Attacked;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (Timer > 0) --Timer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Intro() { if (TalkedTo) Log("REPEAT_GREET"); else { Log("CS_INTRO"); TalkedTo = true; } }
    void Main()
    {
        NSF(); if (T()) return;
        Timer = 5;
        while (!LeavingHappy) {
            NSF(); if (T()) return;
            if (Timer <= 0 && Near) { Log("CRY"); Timer = 15; }
            if (Bit(TalkMask)) {
                if (Helped) Log("THANKS");
                else if (Helping && BootyDug && Gold >= 500) {
                    Log("GOLD_QUESTION");
                    if (Answer == 1) { Helped = true; Log("CS_SUCCESS"); Gold -= 500; LeavingHappy = true; }
                    else Intro();
                } else Intro();
            }
            if (Bit(HitMask) && !Bit(HealMask)) { Log("ATTACKED"); Attacked = true; Log("fade"); return; }
        }
        Log("walk_to_leave_marker");
    }
};
int main()
{
    Wife a = { 0, 40, 0, 600, (1 << 3) | (1 << 8), 0, 0, 0, false, false, false, false, false, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_INTRO;CRY;REPEAT_GREET;CRY;CRY;") && a.TalkedTo;
    g_log[0] = 0;
    Wife b = { 0, 40, 0, 600, (1 << 2), 0, 0, 1, true, false, true, true, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "GOLD_QUESTION;CS_SUCCESS;walk_to_leave_marker;") && b.Gold == 100 && b.Helped;
    g_log[0] = 0;
    Wife c = { 0, 40, 0, 0, 0, (1 << 4) | (1 << 2), (1 << 2), 0, false, false, false, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "ATTACKED;fade;") && c.Attacked && c.Frames == 4;
    std::printf(ok ? "00d8bcd0_TEST PASS\n" : "00d8bcd0_TEST FAIL\n");
    return ok ? 0 : 1;
}