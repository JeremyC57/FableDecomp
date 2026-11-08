// Behaviour test for NScript::CV_ExposeMayorScript::CEM_Rhodri::Main (0x00E799F0): Rhodri in Expose the Mayor. Until the quest stage
// reaches 5 he faces the hero every 4 frames when the hero is within 10 units; talked to at stage 1 he plays CS_MAYOREXPOSE_RHODRI
// (ExplainedPlight, stage 2, HauntedHouse objective), at stage 4 says TEXT_QST_B18_RHODRI_LETTER and otherwise CHATTER (+_MARRIED
// when the hero married Lady Grey); hit by the hero (a heal-life spell excepted) he says ATTACKED(+_MARRIED); until he has explained
// his plight he calls the hero over (ATTENTION) every 7 frames within 15 units. A frame-stepped model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Rhodri {
    int Frame, TerminateAt, Stage, TalkAt[3], NTalk, HitAt, HealAt, Stage4At, Stage5At, Faced; float HeroDist; bool Married, Explained;
    bool T() const { return Frame >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    void Main()
    {
        ++Frame; if (T()) return;
        int talk = 0, face = 0;
        while (Stage != 5) {
            ++Frame; if (talk) --talk; if (face) --face;
            if (Frame == Stage4At) Stage = 4;
            if (Frame == Stage5At) Stage = 5;
            if (T()) return;
            if (!face && HeroDist < 10.0f) { ++Faced; face = 4; }
            if (Talked()) {
                if (Stage == 1) { Explained = true; Log("CS_MAYOREXPOSE_RHODRI"); Stage = 2; Log("HauntedHouse"); }
                else if (Stage == 4) Log("TEXT_QST_B18_RHODRI_LETTER");
                else Log(Married ? "TEXT_QST_B18_RHODRI_CHATTER_MARRIED" : "TEXT_QST_B18_RHODRI_CHATTER");
            }
            bool special = Frame == HealAt, heal = Frame == HealAt;   // the only special-ability hit is a heal-life spell
            if (Frame == HitAt || (special && !heal)) Log(Married ? "TEXT_QST_B18_RHODRI_ATTACKED_MARRIED" : "TEXT_QST_B18_RHODRI_ATTACKED");
            if (!Explained && !talk && HeroDist < 15.0f) { talk = 7; Log("TEXT_QST_B18_RHODRI_ATTENTION"); }
        }
    }
};
int main()
{
    Rhodri a = { 0, 100, 1, { 10, 14, 25 }, 3, 16, 17, 20, 30, 0, 12.0f, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "TEXT_QST_B18_RHODRI_ATTENTION;TEXT_QST_B18_RHODRI_ATTENTION;CS_MAYOREXPOSE_RHODRI;HauntedHouse;TEXT_QST_B18_RHODRI_CHATTER_MARRIED;TEXT_QST_B18_RHODRI_ATTACKED_MARRIED;TEXT_QST_B18_RHODRI_LETTER;")
        && a.Explained && a.Stage == 5 && a.Faced == 0 && a.Frame == 30;
    g_log[0] = 0;
    Rhodri b = { 0, 10, 3, { 4 }, 1, -1, -1, -1, -1, 0, 5.0f, false, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "TEXT_QST_B18_RHODRI_CHATTER;") && b.Faced == 2;
    std::printf(ok ? "00e799f0_TEST PASS\n" : "00e799f0_TEST FAIL\n");
    return ok ? 0 : 1;
}