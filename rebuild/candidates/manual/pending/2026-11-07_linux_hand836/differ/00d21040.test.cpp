// Behaviour test for NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main (0x00D21040): the captured bandit leader. Until he
// is freed or the end cut-scene is done he curses at the hero when talked to (CURSES_AT_YOU) or hit (CURSES_AT_YOU_ON_HIT), the
// guard answering STOP_THAT, and every 10 frames walks to a new RestlessMarker; freed, he runs to MB_OutsideBanditCamp as a monster
// (blowing KNG_HORN when the horn marker is far and off-screen) unless the end cut-scene starts, in which case he waits it out and
// goes home; then the mission fails (reason 3, unless already succeeded) or, after the end cut-scene, he is removed. A frame model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Parent { bool BanditLeaderFree, EndCutsceneStarts, EndCutsceneDone, MissionSucceeded, MissionFailed; int ReasonForFailure; };
struct Leader {
    Parent* P; int Frame, TerminateAt, TalkAt, HitAt, FreeAt, StartsAt, DoneAt, Moves; float CampDist; bool HornFar, Removed;
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (Frame == FreeAt) P->BanditLeaderFree = true; if (Frame == StartsAt) P->EndCutsceneStarts = true; if (Frame == DoneAt) P->EndCutsceneDone = true; }
    void Main()
    {
        NSF(); if (T()) return;
        Log("OPINION_SOURCE_INANIMATE_EVIL_HIGH");
        int restless = 5;
        while (!P->BanditLeaderFree && !P->EndCutsceneDone) {
            NSF(); if (restless) --restless;
            if (T()) return;
            if (Frame == TalkAt) { Log("CURSES_AT_YOU"); Log("STOP_THAT"); }
            if (Frame == HitAt) { Log("CURSES_AT_YOU_ON_HIT"); Log("STOP_THAT"); }
            if (!restless) { ++Moves; restless = 10; }
        }
        if (T()) return;
        if (!P->EndCutsceneDone) {
            Log("HUD_ORB_QUEST_OPTIONAL"); Log("FACTION_MONSTERS");
            if (HornFar) Log("KNG_HORN");
            while (CampDist > 2.0f) {
                NSF(); if (T()) return;
                if (!P->EndCutsceneStarts) CampDist -= 1.0f;
                else {
                    Log("FACTION_NEUTRALS");
                    while (!P->EndCutsceneDone) { NSF(); if (T()) return; }
                    Log("home");
                    break;
                }
            }
            if (T()) return;
        }
        if (!P->EndCutsceneStarts) { P->ReasonForFailure = 3; if (!P->MissionSucceeded) P->MissionFailed = true; }
        else { Removed = true; Log("removed"); }
    }
};
int main()
{
    Parent p = { false, false, false, false, false, 0 };
    Leader a = { &p, 0, 100, 4, 8, 30, -1, -1, 0, 6.0f, true, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "OPINION_SOURCE_INANIMATE_EVIL_HIGH;CURSES_AT_YOU;STOP_THAT;CURSES_AT_YOU_ON_HIT;STOP_THAT;HUD_ORB_QUEST_OPTIONAL;FACTION_MONSTERS;KNG_HORN;")
        && a.Moves == 3 && p.ReasonForFailure == 3 && p.MissionFailed && !a.Removed;
    g_log[0] = 0;
    Parent q = { false, false, false, false, false, 0 };
    Leader b = { &q, 0, 100, -1, -1, 5, 7, 12, 0, 50.0f, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "OPINION_SOURCE_INANIMATE_EVIL_HIGH;HUD_ORB_QUEST_OPTIONAL;FACTION_MONSTERS;FACTION_NEUTRALS;home;removed;")
        && b.Removed && !q.MissionFailed && q.ReasonForFailure == 0;
    std::printf(ok ? "00d21040_TEST PASS\n" : "00d21040_TEST FAIL\n");
    return ok ? 0 : 1;
}
