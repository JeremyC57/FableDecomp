// Behaviour test for 00e824e0 (CV_FisticuffsClubScript::KeepVariablesUpToDate): outside the club the hero is
// unrestricted; once the hero enters, teleport/morality/will/weapons are disabled, guards ignore crimes and the data
// flag mirrors the state; the thread ends when terminating.
#include <cstdio>

struct GSI {
    int frames; bool will, weapons, teleport, morality, guardsIgnore, killable; int returns;
    bool* inClub; int enterAt, leaveAt;
    void NewScriptFrame() { ++frames; if (frames == enterAt) *inClub = true; if (frames == leaveAt) *inClub = false; }
};
struct Script {
    GSI* gsi; bool dataFlag; bool HeroInFisticuffsClub; int termAfterFrames;
    bool IsActiveThreadTerminating() { return gsi->frames >= termAfterFrames; }
    void KeepVariablesUpToDate()
    {
        while (!IsActiveThreadTerminating()) {
            dataFlag = false;
            gsi->will = true; gsi->weapons = true; gsi->teleport = true; ++gsi->returns;
            gsi->killable = true; gsi->morality = true; gsi->guardsIgnore = false;
            while (!HeroInFisticuffsClub) { gsi->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            dataFlag = true;
            gsi->teleport = false; gsi->killable = false; gsi->morality = false; gsi->guardsIgnore = true;
            gsi->will = false; gsi->weapons = false;
            while (HeroInFisticuffsClub) { gsi->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            gsi->NewScriptFrame();
        }
    }
};
int main()
{
    Script s; GSI g = {};
    g.inClub = &s.HeroInFisticuffsClub; g.enterAt = 3; g.leaveAt = 1000;
    s.gsi = &g; s.HeroInFisticuffsClub = false; s.dataFlag = false; s.termAfterFrames = 10;
    s.KeepVariablesUpToDate();
    int ok = s.dataFlag && !g.will && !g.weapons && !g.teleport && !g.morality && g.guardsIgnore && !g.killable && g.returns == 1;
    Script t; GSI h = {};
    h.inClub = &t.HeroInFisticuffsClub; h.enterAt = 2; h.leaveAt = 4;
    t.gsi = &h; t.HeroInFisticuffsClub = false; t.dataFlag = true; t.termAfterFrames = 7;
    t.KeepVariablesUpToDate();
    if (t.dataFlag || !h.will || !h.teleport || h.guardsIgnore || h.returns != 2) ok = 0;
    if (!ok) { std::printf("00e824e0_TEST FAIL\n"); return 1; }
    std::printf("00e824e0_TEST PASS\n");
    return 0;
}