// Behaviour test for NScript::CQ_RansomVictimScript::CVictimsFather::Main (0x00DE4CB0): until the intro is done the chief faces the
// hero, calls OVER_HERE every 15 seconds while the hero is within 15, and on a talk plays CS_RANSOM_INTRO, pays 7500 gold and starts
// the quest screen once; until the victim is rescued a talk offers help while the bodyguard lives and he was hit at most twice (yes
// lowers the reward), else KILLED_BODYGUARD_TALK or REMIND, and the bodyguard's death near the hero is noted once (lower reward);
// afterwards a talk with the victim alive plays the good outro, asks TELL / NOTELL and marks the mission succeeded; every hit block
// says ON_HIT and counts HitChief.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Father {
    int Frames, TerminateAt, TalkMask, HitMask, NearMask, AnswerYes, GuardDiesAt, RescuedAt;
    long Gold, OverHereTimer, HitChief, GoldReward;
    bool IntroDone, QuestStartScreened, ExtraAssistance, Reacted, VictimAlive, MissionSucceeded;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (OverHereTimer > 0) --OverHereTimer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    bool GuardAlive() const { return Frames < GuardDiesAt; }
    void OnHit() { if (Bit(HitMask)) { Log("ON_HIT"); ++HitChief; } }
    void Main() {
        if (T()) return;
        OverHereTimer = 0;
        while (!IntroDone) {
            if (OverHereTimer == 0 && Bit(NearMask)) { Log("OVER_HERE"); OverHereTimer = 15; }
            if (Bit(TalkMask)) {
                Log("CS_RANSOM_INTRO"); Gold += 7500;
                if (!QuestStartScreened) { Log("quest_screen"); QuestStartScreened = true; }
                IntroDone = true;
            }
            OnHit();
            NSF();
            if (T()) return;
        }
        while (Frames < RescuedAt) {
            if (Bit(TalkMask)) {
                if (!ExtraAssistance && GuardAlive() && HitChief <= 2) {
                    Log("OFFER");
                    if (AnswerYes) { ExtraAssistance = true; GoldReward -= 1000; Log("ACCEPTED_HELP"); }
                    else Log("NOT_ACCEPTED_HELP");
                } else if (!ExtraAssistance && !GuardAlive()) Log("KILLED_BODYGUARD_TALK");
                else Log("REMIND");
            }
            OnHit();
            if (!Reacted && !GuardAlive() && Bit(NearMask)) { Log("KILLED_BODYGUARD"); GoldReward -= 500; Reacted = true; }
            NSF();
            if (T()) return;
        }
        Log("teleport_home");
        for (; !T(); NSF()) {
            if (Bit(TalkMask)) {
                if (VictimAlive) {
                    Log("CS_RANSOM_OUTRO_GOOD");
                    Log(AnswerYes ? "TELL" : "NOTELL");
                    MissionSucceeded = true;
                } else Log("REMIND");
            }
            OnHit();
        }
    }
};
int main()
{
    Father a = { 0, 20, (1 << 3) | (1 << 5) | (1 << 9) | (1 << 14), (1 << 2) | (1 << 15), (1 << 1) | (1 << 8), 0, 7, 12,
                 0, 0, 0, 10000, false, false, false, false, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "OVER_HERE;ON_HIT;CS_RANSOM_INTRO;quest_screen;OFFER;NOT_ACCEPTED_HELP;KILLED_BODYGUARD;"
                                  "KILLED_BODYGUARD_TALK;teleport_home;CS_RANSOM_OUTRO_GOOD;NOTELL;ON_HIT;")
              && a.Gold == 7500 && a.HitChief == 2 && a.GoldReward == 9500 && a.MissionSucceeded && a.Frames == 20;
    g_log[0] = 0;
    Father b = { 0, 8, (1 << 0) | (1 << 2) | (1 << 4), 0, 0, 1, 99, 3, 0, 0, 3, 10000, false, true, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "CS_RANSOM_INTRO;REMIND;teleport_home;REMIND;") && b.Gold == 7500 && !b.MissionSucceeded;
    std::printf(ok ? "00de4cb0_TEST PASS\n" : "00de4cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
