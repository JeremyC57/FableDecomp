#include <cstdio>
#include <cstring>
// Behaviour model of CBeggar::Main: scripted waits on "talked to by the hero", sit / stand calls, termination exits.
class CCharString { public: const char* text; CCharString(const char* t, int = -1) : text(t) {} };
static int g_frame, g_terminateAt, g_talkFrames[8], g_talkCount, g_sitLog[8], g_sitCount, g_badName;
class CScriptThing { public:
    bool MsgIsTalkedToBy(const CCharString& name) const
    {
        if (std::strcmp(name.text, "SCRIPT_NAME_HERO") != 0) ++g_badName;
        for (int i = 0; i < g_talkCount; ++i) if (g_talkFrames[i] == g_frame) return true;
        return false;
    }
};
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frame; }
    void EntitySetAsSittingOnFloor(const CScriptThing&, bool sitting) const { g_sitLog[g_sitCount++] = sitting ? g_frame + 100 : g_frame; }
};
struct CBeggar {
    CGameScriptInterface* Game; CScriptThing Me;
    bool IsActiveThreadTerminating() const { return g_frame >= g_terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!IsActiveThreadTerminating()) {
            while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetAsSittingOnFloor(Me, true);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetAsSittingOnFloor(Me, false);
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    CGameScriptInterface game; CBeggar b; b.Game = &game;
    g_frame = 0; g_terminateAt = 20; g_talkCount = 3; g_talkFrames[0] = 3; g_talkFrames[1] = 6; g_talkFrames[2] = 10;
    b.Main();
    // talked to at frame 3 -> sit (frame 3), next frame 4..6 -> talked at 6 -> stand, then sit again at 10, then terminate at 20
    if (g_sitCount != 3 || g_sitLog[0] != 103 || g_sitLog[1] != 6 || g_sitLog[2] != 110 || g_frame != 20 || g_badName) {
        std::printf("00ef3200_TEST FAIL\n"); return 1;
    }
    g_frame = 0; g_terminateAt = 1; g_sitCount = 0;   // terminating straight after the first frame
    b.Main();
    if (g_sitCount != 0 || g_frame != 1) { std::printf("00ef3200_TEST FAIL\n"); return 1; }
    std::printf("00ef3200_TEST PASS\n");
    return 0;
}