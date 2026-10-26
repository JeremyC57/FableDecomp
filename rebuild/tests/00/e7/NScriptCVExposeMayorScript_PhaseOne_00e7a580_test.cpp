// Behaviour test for CV_ExposeMayorScript::PhaseOne (0x00E7A580).
#include <cstdio>

struct CV_ExposeMayorScript;
static CV_ExposeMayorScript* g_s = 0;
static int g_frames = 0, g_setAt = 0, g_termAt = -1, g_termCalls = 0;
static void OnFrame();

class CGameScriptInterface { public: void NewScriptFrame() const { ++g_frames; OnFrame(); } };

struct CV_ExposeMayorScript {
    CGameScriptInterface* Game;
    long Path;
    long StageNumber;
    bool IsActiveThreadTerminating() { ++g_termCalls; return g_termAt >= 0 && g_frames >= g_termAt; }
    void PhaseOne();
};

static void OnFrame() { if (g_frames == g_setAt) g_s->StageNumber = 2; }

void CV_ExposeMayorScript::PhaseOne()
{
    while (StageNumber == 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}

int main()
{
    CGameScriptInterface gsi;
    CV_ExposeMayorScript s; s.Game = &gsi; s.Path = 0; s.StageNumber = 0; g_s = &s;
    g_setAt = 3; s.PhaseOne();
    if (g_frames != 3 || g_termCalls != 4) { std::printf("00e7a580_TEST FAIL\n"); return 1; }
    s.StageNumber = 0; g_frames = 0; g_termCalls = 0; g_setAt = 99; g_termAt = 2; s.PhaseOne();
    if (g_frames != 2 || g_termCalls != 2 || s.StageNumber != 0) { std::printf("00e7a580_TEST FAIL\n"); return 1; }
    std::printf("00e7a580_TEST PASS\n");
    return 0;
}