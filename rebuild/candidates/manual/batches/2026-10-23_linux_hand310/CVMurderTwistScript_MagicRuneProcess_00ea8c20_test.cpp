// Behaviour test for CV_MurderTwistScript::MagicRuneProcess (0x00EA8C20).
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing { float dist; int id; };
struct CTimer { int t; int GetTime() { return t; } void SetTime(int v) { t = v; Log("set"); } };
struct CGameScriptInterface {
    int frame; int unloadAt; int* state;
    void NewScriptFrame() { Log("frame"); ++frame; if (frame == 2) *state = 2; }
    bool MsgOnRegionUnloaded() { return frame == unloadAt; }
    int GetHero() { return 0; }
    void CreateRumble(int id, float a, float b, const char* s)
    { char buf[16]; std::sprintf(buf, "r%d", id); Log(a == 0.3f && b == 0.2f && !s[0] ? buf : "bad"); }
};
static float Dist(int, const CScriptThing& t) { return t.dist; }
struct CV_MurderTwistScript {
    CGameScriptInterface* Game; int MurderTwistState; std::vector<CScriptThing> MagicRuneRumbleList;
    int termAfter; CTimer timer;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void MagicRuneProcess()
    {
        while (!(MurderTwistState > 1)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            while (!Game->MsgOnRegionUnloaded()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (timer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) return;
                    timer.SetTime(1);
                    for (unsigned int i = 0; i < MagicRuneRumbleList.size(); ++i) {
                        if (IsActiveThreadTerminating()) return;
                        if (Dist(Game->GetHero(), MagicRuneRumbleList[i]) < 15.0f)
                            Game->CreateRumble(MagicRuneRumbleList[i].id, 0.3f, 0.2f, "");
                    }
                    if (IsActiveThreadTerminating()) return;
                    timer.t = 0;
                }
            }
            if (IsActiveThreadTerminating()) return;
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    g_log[0] = 0;
    CGameScriptInterface g; g.frame = 0; g.unloadAt = 4;
    CV_MurderTwistScript s; s.Game = &g; s.MurderTwistState = 0; g.state = &s.MurderTwistState;
    CScriptThing a = { 3.0f, 1 }, b = { 20.0f, 2 }, c = { 14.9f, 3 };
    s.MagicRuneRumbleList.push_back(a); s.MagicRuneRumbleList.push_back(b); s.MagicRuneRumbleList.push_back(c);
    s.timer.t = 0; s.termAfter = 22;
    s.MagicRuneProcess();
    const char* expect = "frame;frame;frame;set;r1;r3;frame;set;r1;r3;frame;frame;set;r1;";
    if (std::strcmp(g_log, expect) != 0) { std::printf("00ea8c20_TEST FAIL %s\n", g_log); return 1; }
    std::printf("00ea8c20_TEST PASS\n");
    return 0;
}
