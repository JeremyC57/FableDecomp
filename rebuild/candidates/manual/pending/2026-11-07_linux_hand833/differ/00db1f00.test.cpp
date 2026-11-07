// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWoman::Main (0x00DB1F00): the husband's lover. Each frame until
// NOVI_AffairWife comes within 5 units: walk home when more than 0.1 off it (to within 2 units); a hero hit says AFFAIRWOMAN_ON_HIT and
// costs bad deed 2; a talk says AFFAIRWOMAN_BUSY with the man turned to the hero, then back to her (TalkingToWoman set around both);
// ReceiveKiss / ReceiveHug play RECEIVE_KISS / RECEIVE_HUG when she is idle and clear the flag. Then she runs to
// AffairWomanRunOffPoint, waits until off screen and is removed. A state model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Woman {
    int Frames, TerminateAt, HitAt, TalkAt, WifeArrivesAt, OffHome, OnScreenFor;
    bool Kiss, Hug, Busy, Talking; int BadDeeds;
    void NewScriptFrame() { ++Frames; }
    bool T() const { return Frames >= TerminateAt; }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        if (T()) return;
        for (;;) {
            if (OffHome > 0) {
                while (OffHome > 0) { NewScriptFrame(); if (T()) return; --OffHome; }
                Log("home");
            }
            if (Frames == HitAt) { Talking = true; Log("TEXT_QST_048_AFFAIRWOMAN_ON_HIT"); ++BadDeeds; Talking = false; }
            if (Frames == TalkAt) { Talking = true; Log("man_faces_hero"); Log("TEXT_QST_048_AFFAIRWOMAN_BUSY"); Log("man_faces_woman"); Talking = false; }
            if (Kiss && !Busy) { Log("RECEIVE_KISS"); Kiss = false; }
            if (Hug && !Busy) { Log("RECEIVE_HUG"); Hug = false; }
            if (Frames >= WifeArrivesAt) break;
            NewScriptFrame();
            if (T()) return;
        }
        if (T()) return;
        Log("run_off");
        while (OnScreenFor > 0) { NewScriptFrame(); if (T()) return; --OnScreenFor; }
        if (T()) return;
        Log("RemoveThing");
    }
};
int main()
{
    Woman a = { 0, 40, 2, 3, 6, 0, 2, true, false, false, false, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "RECEIVE_KISS;TEXT_QST_048_AFFAIRWOMAN_ON_HIT;man_faces_hero;TEXT_QST_048_AFFAIRWOMAN_BUSY;man_faces_woman;run_off;RemoveThing;")
        && a.BadDeeds == 1 && !a.Kiss && !a.Talking;
    g_log[0] = 0;
    Woman b = { 0, 40, -1, -1, 4, 2, 0, false, true, true, false, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "home;run_off;RemoveThing;") && b.Hug;
    g_log[0] = 0;
    Woman c = { 0, 5, -1, -1, 99, 0, 0, false, false, false, false, 0 };
    c.Main();
    ok = ok && g_log[0] == 0;
    std::printf(ok ? "00db1f00_TEST PASS\n" : "00db1f00_TEST FAIL\n");
    return ok ? 0 : 1;
}
