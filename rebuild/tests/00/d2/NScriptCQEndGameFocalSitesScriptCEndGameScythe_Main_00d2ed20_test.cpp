// Behaviour test for NScript::CQ_EndGameFocalSitesScript::CEndGameScythe::Main (0x00D2ED20): the intro cut-scene, the quest start
// screen only once (QuestStartScreenShown), then until JackLeaves: the Briar map line when talked to, walking to the hero while he
// is over 8 units away until within 4 (giving up when Jack leaves meanwhile), allies with the hero when hit. A 1-D state model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Scythe {
    float Me, Hero; int Frame; bool QuestStartScreenShown, JackLeaves, Allies;
    bool Over(float d) { float x = Hero - Me; if (x < 0) x = -x; return x > d; }
    void NewScriptFrame()
    {
        ++Frame;
        if (Frame == 3) Hero = 20.0f;
        if (Frame == 9) JackLeaves = true;
    }
    void Main()
    {
        Log("CS_FOCALSITE_1_INTRO");
        if (!QuestStartScreenShown) { Log("start_screen"); QuestStartScreenShown = true; }
        while (!JackLeaves) {
            NewScriptFrame();
            if (Frame == 2) Log("TEXT_QST_001_BRIAR_MAP");
            if (Over(8.0f)) {
                while (Over(4.0f)) {
                    NewScriptFrame();
                    if (JackLeaves) return;
                    Me += 4.0f;
                }
                Log("caught_up");
            }
            if (Frame == 7) { Allies = true; Log("allies"); }
        }
    }
};
int main()
{
    Scythe s = { 0.0f, 0.0f, 0, false, false, false };
    s.Main();
    bool ok = !std::strcmp(g_log, "CS_FOCALSITE_1_INTRO;start_screen;TEXT_QST_001_BRIAR_MAP;caught_up;allies;") && s.Allies && s.Me == 16.0f && s.Frame == 9;
    g_log[0] = 0;
    Scythe t = { 0.0f, 0.0f, 3, true, false, false };
    t.Main();
    ok = ok && !std::strcmp(g_log, "CS_FOCALSITE_1_INTRO;allies;") && t.Allies && t.Frame == 9;
    std::printf(ok ? "00d2ed20_TEST PASS\n" : "00d2ed20_TEST FAIL\n");
    return ok ? 0 : 1;
}