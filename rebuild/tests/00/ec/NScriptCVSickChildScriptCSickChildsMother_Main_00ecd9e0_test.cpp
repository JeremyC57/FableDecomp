// Behaviour test for NScript::CV_SickChildScript::CSickChildsMother::Main (0x00ECD9E0): the sick child's mother. She is flagged as
// having information unless the hero holds the cure (EndPotion) and the intro is done; within 7 units before the intro she plays
// CS_SICK_INTRO and gives the quest card (objective 01); talked to after the intro she asks the cure question when the hero carries
// the potion (yes: CS_SICK_OUTRO, the potion is taken, FinishedQuest, the loop ends; no: ASK_CURE) or reminds him (CURE_REMIND_10);
// hit by the hero she says MOTHER_HIT and the two become allies; at the end she is released and idles. A frame-stepped model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Mother {
    int Frame, TerminateAt, TalkAt[3], NTalk, HitAt, PotionAt, AnswerYesAt; float Dist;
    bool MotherIntroDone, FinishedQuest, HasInfo, Potion, Scripted;
    bool T() const { return Frame >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    void NSF() { ++Frame; Dist -= 1.0f; if (Frame == PotionAt) Potion = true; }
    void Main()
    {
        NSF(); if (T()) return;
        Scripted = true;
        if (Potion || !MotherIntroDone) HasInfo = true;
        for (; !T(); NSF()) {
            if (!MotherIntroDone && Dist < 7.0f) {
                Log("CS_SICK_INTRO"); Log("OBJECT_QUEST_CARD_SICK_CHILD"); Log("OBJECTIVE_01");
                MotherIntroDone = true; HasInfo = false;
            }
            if (MotherIntroDone && Talked()) {
                if (Potion) {
                    Log("CURE_QUESTION");
                    if (Frame >= AnswerYesAt) {
                        Log("CS_SICK_OUTRO"); Potion = false; HasInfo = false; FinishedQuest = true;
                        break;
                    }
                    Log("ASK_CURE");
                }
                else Log("CURE_REMIND_10");
            }
            if (Frame == HitAt) { Log("MOTHER_HIT"); Log("allies"); }
        }
        if (T()) return;
        Scripted = false;
        for (;;) { NSF(); if (T()) return; }
    }
};
int main()
{
    Mother a = { 0, 50, { 12, 20, 30 }, 3, 15, 18, 25, 10.0f };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_SICK_INTRO;OBJECT_QUEST_CARD_SICK_CHILD;OBJECTIVE_01;CURE_REMIND_10;MOTHER_HIT;allies;CURE_QUESTION;ASK_CURE;CURE_QUESTION;CS_SICK_OUTRO;")
        && a.FinishedQuest && !a.Potion && !a.Scripted && !a.HasInfo && a.Frame == 50;
    g_log[0] = 0;
    Mother b = { 0, 6, { 0 }, 0, -1, -1, 0, 100.0f };
    b.Main();
    ok = ok && g_log[0] == 0 && b.HasInfo && b.Scripted && !b.MotherIntroDone;
    std::printf(ok ? "00ecd9e0_TEST PASS\n" : "00ecd9e0_TEST FAIL\n");
    return ok ? 0 : 1;
}