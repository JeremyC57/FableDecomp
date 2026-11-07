// Behaviour test for NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard2::Main (0x00CF0D20): the second Arena entrance guard. He
// joins FACTION_HERO, is unkillable and faces GuardStareMarker; talked to he says RETURN_WELL_DONE once Q_Arena is complete, else
// GREETING the first time and GREETING_AGAIN afterwards; hit by the hero (a heal-life spell excepted) while no hit cut-scene runs he
// says ATTACKED, the camera is reset and he heals 100 (InHitCutsceneAlready is set for the duration). A frame-stepped model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Guard {
    int Frame, TerminateAt, TalkAt[4], NTalk, HitAt[2], ArenaDoneAt, Healed; bool InHitCutsceneAlready, OtherCutscene;
    bool T() const { return Frame >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    bool Hit() const { return Frame == HitAt[0] || Frame == HitAt[1]; }
    void Main()
    {
        if (T()) return;
        Log("FACTION_HERO"); Log("GuardStareMarker");
        bool HaveChatted = false;
        for (; !T(); ++Frame) {
            if (Talked()) {
                if (ArenaDoneAt >= 0 && Frame >= ArenaDoneAt) Log("RETURN_WELL_DONE");
                else if (HaveChatted) Log("GREETING_AGAIN");
                else { Log("GREETING"); HaveChatted = true; }
            }
            else if (Hit()) {
                if (!InHitCutsceneAlready && !OtherCutscene) {
                    InHitCutsceneAlready = true;
                    Log("ATTACKED"); Log("CameraDefault"); Healed += 100;
                    InHitCutsceneAlready = false;
                }
            }
        }
    }
};
int main()
{
    Guard a = { 0, 40, { 3, 8, 20, 30 }, 4, { 12, 30 }, 25, 0, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "FACTION_HERO;GuardStareMarker;GREETING;GREETING_AGAIN;ATTACKED;CameraDefault;GREETING_AGAIN;RETURN_WELL_DONE;")
        && a.Healed == 100 && !a.InHitCutsceneAlready && a.Frame == 40;
    g_log[0] = 0;
    Guard b = { 0, 10, { 0 }, 0, { 4, 6 }, -1, 0, false, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "FACTION_HERO;GuardStareMarker;") && b.Healed == 0;
    std::printf(ok ? "00cf0d20_TEST PASS\n" : "00cf0d20_TEST FAIL\n");
    return ok ? 0 : 1;
}
