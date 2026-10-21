// Behaviour test for CTortureDoorPassage::Main (0x00DD8990): locked at start, the hint shown once per hero use (waiting
// for the click), unlocked once the mother is rescued; an early termination leaves it locked.
#include <cstdio>
#include <cstring>

struct CQ_PrisonEscapeRescueMotherScript { bool MotherRescued; };
static int g_frames, g_hints, g_clicksNeeded, g_terminateAfter;
static bool g_locked;
struct CGameScriptInterface {
    CQ_PrisonEscapeRescueMotherScript* Parent;
    void NewScriptFrame() const { ++g_frames; if (g_frames == 4) Parent->MotherRescued = true; }
    bool MsgIsGameInfoClickedPast() const { return g_clicksNeeded-- <= 0; }
    void DisplayGameInfo(const char* t) const { if (std::strcmp(t, "TEXT_QST_055_HINT_NEED_TO_HAVE_MOTHER") == 0) ++g_hints; }
    void EntitySetAsLocked(const void*, bool l) const { g_locked = l; }
};
struct CScriptThing { bool MsgIsUsedBy(const char* who) const { return g_frames == 1 && std::strcmp(who, "SCRIPT_NAME_HERO") == 0; } };
struct CTortureDoorPassage {
    CGameScriptInterface* Game; CScriptThing Me; CQ_PrisonEscapeRescueMotherScript* ParentClass;
    bool IsActiveThreadTerminating() { return g_terminateAfter >= 0 && g_frames >= g_terminateAfter; }
    void Main()
    {
        Game->EntitySetAsLocked(&Me, true);
        while (!ParentClass->MotherRescued) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                Game->DisplayGameInfo("TEXT_QST_055_HINT_NEED_TO_HAVE_MOTHER");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetAsLocked(&Me, false);
    }
};

int main()
{
    CQ_PrisonEscapeRescueMotherScript parent = { false };
    CGameScriptInterface gsi = { &parent };
    CTortureDoorPassage d = { &gsi, CScriptThing(), &parent };
    g_clicksNeeded = 1; g_terminateAfter = -1;
    d.Main();
    bool ok = !g_locked && g_hints == 1 && g_frames == 4;
    parent.MotherRescued = false; g_frames = 0; g_hints = 0; g_terminateAfter = 1; g_clicksNeeded = 0;
    d.Main();
    ok = ok && g_locked && g_frames == 1 && g_hints == 0;
    if (!ok) { std::printf("00dd8990_TEST FAIL\n"); return 1; }
    std::printf("00dd8990_TEST PASS\n");
    return 0;
}
