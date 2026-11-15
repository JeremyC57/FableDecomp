// Behaviour test for NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main (0x00E50FD0): a model of the row's decisions.
// Each frame: an attack is scolded (ATTACK, flag cleared, speech timer reset); else a run-out speech timer says a random line.
// Then the quest phase: 0 = haircut (intro once, then reminder + question: yes = hairdo card 1 unless already wearing style 1,
// then card 2; no = denied), 1 = beard (right hairdo -> beard card, else wrong hair), 2 = moustache (no beard / right hair and
// beard -> tash card unless already wearing one / wrong beard / wrong hair / both wrong), 3 = finale (not all changed = no tash;
// beard + tash + odd hairdo = full reward; more than three wrong combos = half reward; else wrong, count +1). A phase advances on the
// frame after its requirements are complete; a completed quest gives experience and walks Beardy Baldy to the exit.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct BB {
    int Frames, TerminateAt, TalkMask, AttackMask, Answers, AnswerIdx, Phase, Wrong;
    long SpeechTimer;
    bool Attacked, Intro, Req1, Req2, Req3, AllChanged, Complete, Left;
    bool HairStyle1, HairOk, Beard, BeardOk, Tash, TashStyle1, OddHair;
    bool T() const { return Frames >= TerminateAt; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    long Ask() { long a = (Answers >> AnswerIdx) & 1; ++AnswerIdx; return a; }
    void Main() {
        for (; !false; ++Frames, SpeechTimer = SpeechTimer > 0 ? SpeechTimer - 1 : 0) {
            if (T()) return;
            if (Bit(AttackMask)) Attacked = true;
            if (Attacked) { Log("ATTACK"); Attacked = false; SpeechTimer = 3; }
            else if (SpeechTimer == 0) { Log("RANDOM"); SpeechTimer = 3; }
            bool talk = Bit(TalkMask);
            if (Phase == 0) {
                if (talk) {
                    if (Req1) Log("P1_REMINDER");
                    else if (!Intro) { Log("P1_INTRO1"); Intro = true; }
                    else {
                        Log("P1_INTRO_REMINDER");
                        if (Ask() == 1) { Log("P1_INTRO2"); Log(!HairStyle1 ? "HAIRCARD1" : "HAIRCARD2"); Log("P1_INTRO3"); Req1 = true; }
                        else Log("P1_DENIED");
                    }
                }
                if (Req1) Phase = 1;
            } else if (Phase == 1) {
                if (talk) {
                    if (Req2) Log("P2_REMINDER");
                    else if (HairOk) { Log("P2_INTRO1"); Log("BEARDCARD"); Log("P2_INTRO2"); Req2 = true; }
                    else Log("P2_WRONG_HAIR");
                }
                if (Req2) Phase = 2;
            } else if (Phase == 2) {
                if (talk) {
                    if (Req3) Log("P3_REMINDER");
                    else if (!Beard) Log("P3_NO_BEARD");
                    else if (HairOk && BeardOk) {
                        Log("P3_INTRO1");
                        if (Tash) Log("P3_INTRO2");
                        else { Log("P3_INTRO3"); Log(!TashStyle1 ? "TASHCARD1" : "TASHCARD2"); }
                        Log("P3_INTRO4"); Req3 = true;
                    } else if (HairOk && !BeardOk) Log("P3_WRONG_BEARD");
                    else if (!HairOk && BeardOk) Log("P3_WRONG_HAIR");
                    else Log("P3_WRONG_HAIR_AND_BEARD");
                }
                if (Req3) Phase = 3;
            } else if (Phase == 3) {
                if (talk) {
                    if (!AllChanged) Log("END_NO_TASH");
                    else if (Beard && Tash && OddHair) { Log("FINALE1"); Complete = true; }
                    else if (Wrong > 3 && AllChanged) { Log("FINALE2"); Complete = true; }
                    else { ++Wrong; Log("END_WRONG"); }
                }
                if (Complete) { Log("EXPERIENCE"); Log("EXIT"); Left = true; return; }
            }
        }
    }
};
int main()
{
    // run 1: attack at frame 1, haircut question denied then accepted (already wearing style 1 -> card 2), beard phase with the
    // right hairdo, moustache phase with hair + beard right and a tash already worn, finale wrong 4 times then half reward.
    BB b = { 0, 40, (1 << 2) | (1 << 3) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 14),
             1 << 1, (1 << 1), 0, 0, 0, 3,
             false, false, false, false, false, true, false, false,
             true, true, true, true, true, false, false };
    b.Main();
    const char* want = "ATTACK;P1_INTRO1;P1_INTRO_REMINDER;P1_DENIED;RANDOM;P1_INTRO_REMINDER;P1_INTRO2;HAIRCARD2;P1_INTRO3;"
                       "P2_INTRO1;BEARDCARD;P2_INTRO2;RANDOM;P3_INTRO1;P3_INTRO2;P3_INTRO4;RANDOM;END_WRONG;END_WRONG;END_WRONG;RANDOM;END_WRONG;"
                       "FINALE2;EXPERIENCE;EXIT;";
    bool ok = std::strcmp(g_log, want) == 0 && b.Left && b.Wrong == 4;
    if (!ok) std::printf("got %s\n", g_log);
    if (ok) std::printf("00e50fd0_TEST PASS\n");
    return 0;
}
