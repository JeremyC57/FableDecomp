// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main (0x00D0C410): the bandit forger who sells the camp
// pass. He is given the pass once (GivenPass). Until attacked: every ten seconds he waves at a near hero who has no pass while Gate2
// is shut (ASIDE_FIRST / ASIDE_SECOND alternating); talked to, he says EARLY_NOT_NEED (Gate2 open), EARLY_GOT_PASS (hero has a pass)
// or CHAT + CHAT_QUESTION (yes: FAILED when the hero is short of gold, else SUCCESS, the pass and gold change hands, he stops
// selling, small experience; no: TURNED_DOWN). While still selling and not attacked a later talk gets LATE_NOT_NEED or
// SECOND_QUESTION (SECOND_FAIL / SUCCESS / TURNED_DOWN). Hit (or made an enemy) he turns on the hero. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Forger {
    int Frames, TerminateAt, Timer, TalkAt[4], NTalk, Answers[4], HitAt; bool Gate2Open, HeroHasPass; int HeroGold, Price;
    bool GivenPass, StillSelling, SaidFirstAside, NotAttacked, Enemy; int MyGold, Experience;
    bool T() const { return Frames >= TerminateAt; }
    int TalkIndex() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) return i; return -1; }
    bool Buy(int answer, const char* fail)
    {
        if (answer != 1) { Log("TURNED_DOWN"); return false; }
        if (HeroGold < Price) { Log(fail); return false; }
        Log("SUCCESS"); HeroHasPass = true; HeroGold -= Price; MyGold += Price; StillSelling = false; ++Experience; return true;
    }
    void Main()
    {
        StillSelling = true; SaidFirstAside = false; NotAttacked = true;
        if (!GivenPass) { Log("give_pass"); GivenPass = true; }
        while (NotAttacked) {
            ++Frames; if (Timer > 0) --Timer;
            if (T()) return;
            if (Frames == HitAt) { NotAttacked = false; continue; }
            if (Timer <= 0 && !Gate2Open && !HeroHasPass) { Log(SaidFirstAside ? "ASIDE_SECOND" : "ASIDE_FIRST"); SaidFirstAside = !SaidFirstAside; Timer = 10; }
            int t = TalkIndex();
            if (t >= 0) {
                if (Gate2Open) Log("EARLY_NOT_NEED");
                else if (HeroHasPass) Log("EARLY_GOT_PASS");
                else { Log("CHAT"); Log("CHAT_QUESTION"); Buy(Answers[t], "FAILED"); }
                break;
            }
        }
        while (StillSelling && NotAttacked) {
            ++Frames;
            if (T()) return;
            if (Frames == HitAt) { NotAttacked = false; break; }
            int t = TalkIndex();
            if (t >= 0) {
                if (Gate2Open || HeroHasPass) Log("LATE_NOT_NEED");
                else { Log("SECOND_QUESTION"); Buy(Answers[t], "SECOND_FAIL"); }
            }
        }
        if (!NotAttacked) { Enemy = true; Log("attack_hero"); }
        while (!T()) ++Frames;
    }
};
int main()
{
    Forger a = { 0, 30, 0, { 3, 6 }, 2, { 0, 1 }, -1, false, false, 50, 40 };
    a.Main();
    bool ok = !std::strcmp(g_log, "give_pass;ASIDE_FIRST;CHAT;CHAT_QUESTION;TURNED_DOWN;SECOND_QUESTION;SUCCESS;")
        && a.HeroHasPass && a.HeroGold == 10 && a.MyGold == 40 && !a.StillSelling && a.Experience == 1;
    g_log[0] = 0;
    Forger b = { 0, 30, 0, { 2, 5 }, 2, { 1, 1 }, 8, false, false, 10, 40, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "ASIDE_FIRST;CHAT;CHAT_QUESTION;FAILED;SECOND_QUESTION;SECOND_FAIL;attack_hero;") && b.Enemy && b.StillSelling;
    g_log[0] = 0;
    Forger c = { 0, 30, 0, { 2 }, 1, { 1 }, -1, true, false, 99, 40, true };
    c.Main();
    ok = ok && !std::strcmp(g_log, "EARLY_NOT_NEED;") && c.Frames == 30 && !c.Enemy;
    std::printf(ok ? "00d0c410_TEST PASS\n" : "00d0c410_TEST FAIL\n");
    return ok ? 0 : 1;
}
