// Behaviour test for NScript::CQ_RansomVictimScript::CVictimsFather::Main (0x00DE4CB0): the kidnapped boy's father (the chief).
// Until the intro is done he faces the hero every frame-tick, calls OVER_HERE every 15 frames within 15 units and, when talked to,
// plays CS_RANSOM_INTRO, pays 7500 gold and (once) sets objective 02 and the quest start screen. Until the victim is rescued a chat
// offers the bodyguard while he lives, no help was accepted and the chief was hit at most twice (yes: ExtraAssistanceGiven, the
// lowered rewards, ACCEPTED_HELP; no: NOT_ACCEPTED_HELP), else KILLED_BODYGUARD_TALK (guard dead, no help) or REMIND; the guard's death
// within 15 units is commented once (KILLED_BODYGUARD_10, lowered gold reward). After the rescue he teleports to MK_RVG_CHIEF and a
// chat with the victim alive plays CS_RANSOM_OUTRO_GOOD and the reveal question (TELL + morality / NOTELL), then MissionSucceeded,
// ChiefsSonSurvived and the good morality; with the victim dead it is REMIND. Every hero hit (heal-life excepted) says ON_HIT and
// counts HitChief. A frame-stepped model with the engine calls logged.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Father {
    bool IntroDone, ReactedToBodyguardsDeath;                                   // the entity's own flags
    bool QuestStartScreened, MissionSucceeded, RescuedVictim, ExtraAssistanceGiven, ThanksCutsceneStarting; long HitChief;   // parent script
    bool SaveVictim, HaveVictimKilled;                                          // master data
    int Frame, TerminateAt, TalkAt[6], NTalk, HitAt[3], NHit, GuardDiesAt, RescuedAt, Answer[2], NAnswer, Gold, Morality; float HeroDist; bool VictimAlive;
    bool T() const { return Frame >= TerminateAt; }
    void NSF(long* a, long* b) { ++Frame; if (a && *a > 0) --*a; if (b && *b > 0) --*b; if (Frame == RescuedAt) RescuedVictim = true; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    bool Hit() const { for (int i = 0; i < NHit; ++i) if (HitAt[i] == Frame) return true; return false; }
    bool GuardAlive() const { return GuardDiesAt < 0 || Frame < GuardDiesAt; }
    void OnHit() { Log("TEXT_QST_B06_FATHER_ON_HIT"); ++HitChief; }
    void Main()
    {
        NSF(0, 0); if (T()) return;
        Log("StartScripting"); Log("Marker");
        long over_here_timer = 0, facing_timer = 0;
        while (!IntroDone) {
            NSF(&over_here_timer, &facing_timer); if (T()) return;
            if (facing_timer == 0) facing_timer = 1;
            if (over_here_timer == 0 && HeroDist < 15.0f) { Log("TEXT_QST_B06_FATHER_OVER_HERE"); over_here_timer = 15; }
            if (Talked()) {
                Log("CS_RANSOM_INTRO"); Gold += 7500;
                if (!QuestStartScreened) { Log("OBJECTIVE_02"); Log("QuestStartScreen"); QuestStartScreened = true; }
                IntroDone = true;
            }
            if (Hit()) OnHit();
        }
        if (T()) return;
        Log("RemoveMarker");
        if (ExtraAssistanceGiven) Log("ClearInformation");
        while (!RescuedVictim) {
            NSF(0, 0); if (T()) return;
            if (Talked()) {
                if (!ExtraAssistanceGiven && GuardAlive() && HitChief <= 2) {
                    Log("TEXT_QST_B06_FATHER_OFFER");
                    if (Answer[NAnswer++] == 1) { ExtraAssistanceGiven = true; Log("ClearInformation"); Log("LoweredRewards"); Log("TEXT_QST_B06_FATHER_ACCEPTED_HELP_10"); }
                    else Log("TEXT_QST_B06_FATHER_NOT_ACCEPTED_HELP_10");
                } else if (!ExtraAssistanceGiven && !GuardAlive()) Log("TEXT_QST_B06_FATHER_KILLED_BODYGUARD_TALK");
                else Log("TEXT_QST_B06_FATHER_REMIND");
            }
            if (Hit()) OnHit();
            if (!ReactedToBodyguardsDeath && !GuardAlive() && HeroDist < 15.0f) { Log("TEXT_QST_B06_FATHER_KILLED_BODYGUARD_10"); Log("LoweredGold"); ReactedToBodyguardsDeath = true; }
        }
        if (T()) return;
        Log("TeleportToChiefMarker"); Log("Marker");
        for (; !T(); NSF(0, 0)) {
            if (Talked()) {
                if (VictimAlive) {
                    ThanksCutsceneStarting = true; Log("CS_RANSOM_OUTRO_GOOD"); SaveVictim = true;
                    if (Answer[NAnswer++] == 1) { Log("CS_RANSOM_OUTRO_GOOD_TELL"); Morality += 5; }
                    else Log("CS_RANSOM_OUTRO_GOOD_NOTELL");
                    MissionSucceeded = true; Log("ChiefsSonSurvived"); Morality += 100; SaveVictim = false; HaveVictimKilled = false;
                } else Log("TEXT_QST_B06_FATHER_REMIND");
            }
            if (Hit()) OnHit();
        }
    }
};
static Father Make()
{
    Father f; std::memset(&f, 0, sizeof f);
    f.TerminateAt = 60; f.GuardDiesAt = -1; f.RescuedAt = -1; f.HeroDist = 10.0f; f.VictimAlive = true;
    return f;
}
int main()
{
    bool ok = true;
    {   // intro, the offer refused, a hit, the guard dies, the rescue, the good outro with the truth told
        Father f = Make(); f.TalkAt[0] = 4; f.TalkAt[1] = 6; f.TalkAt[2] = 12; f.TalkAt[3] = 20; f.NTalk = 4; f.HitAt[0] = 8; f.NHit = 1;
        f.GuardDiesAt = 10; f.RescuedAt = 15; f.Answer[0] = 0; f.Answer[1] = 1; f.HaveVictimKilled = true;
        f.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;Marker;TEXT_QST_B06_FATHER_OVER_HERE;CS_RANSOM_INTRO;OBJECTIVE_02;QuestStartScreen;RemoveMarker;"
            "TEXT_QST_B06_FATHER_OFFER;TEXT_QST_B06_FATHER_NOT_ACCEPTED_HELP_10;TEXT_QST_B06_FATHER_ON_HIT;TEXT_QST_B06_FATHER_KILLED_BODYGUARD_10;LoweredGold;"
            "TEXT_QST_B06_FATHER_KILLED_BODYGUARD_TALK;TeleportToChiefMarker;Marker;CS_RANSOM_OUTRO_GOOD;CS_RANSOM_OUTRO_GOOD_TELL;ChiefsSonSurvived;")
            && f.Gold == 7500 && f.Morality == 105 && f.MissionSucceeded && f.HitChief == 1 && f.ReactedToBodyguardsDeath && !f.SaveVictim && !f.HaveVictimKilled && f.ThanksCutsceneStarting;
    }
    g_log[0] = 0;
    {   // help accepted, a later chat reminds; three hits; after the rescue the victim is dead: REMIND
        Father f = Make(); f.IntroDone = true; f.QuestStartScreened = true; f.TalkAt[0] = 3; f.TalkAt[1] = 5; f.TalkAt[2] = 14; f.NTalk = 3;
        f.HitAt[0] = 7; f.HitAt[1] = 8; f.HitAt[2] = 9; f.NHit = 3; f.RescuedAt = 11; f.Answer[0] = 1; f.VictimAlive = false; f.HeroDist = 30.0f;
        f.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;Marker;RemoveMarker;TEXT_QST_B06_FATHER_OFFER;ClearInformation;LoweredRewards;"
            "TEXT_QST_B06_FATHER_ACCEPTED_HELP_10;TEXT_QST_B06_FATHER_REMIND;TEXT_QST_B06_FATHER_ON_HIT;TEXT_QST_B06_FATHER_ON_HIT;"
            "TEXT_QST_B06_FATHER_ON_HIT;TeleportToChiefMarker;Marker;TEXT_QST_B06_FATHER_REMIND;")
            && f.ExtraAssistanceGiven && f.HitChief == 3 && !f.MissionSucceeded && f.Gold == 0 && f.Frame == 60;
    }
    std::printf(ok ? "00de4cb0_TEST PASS\n" : "00de4cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}