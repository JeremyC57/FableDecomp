// Behaviour test for NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main (0x00D2D930): Jack waits until the hero is
// within 15 units of him, within 5 of the site's hero marker, or hits him; then marks JackReached / JackLeaves, builds the focal
// site, plays the cut scene of the current focal-site region (1 Scythe, 2 -, 3 Thunder after the hero reaches the Hobbe-cave radius
// check, else Scarlet Robe, who is then removed), teleports the hero to that region's hero spawn point, sets EndCutScenePlayed and
// leaves the teleporter residue. A 1-D state model of the control flow and the per-region choices.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Jack {
    int Region; float Me, Hero, Marker; bool HitByHero; int Frame;
    bool JackReached, JackLeaves, EndCutScenePlayed;
    static float Dist(float a, float b) { float d = a - b; return d < 0 ? -d : d; }
    void NewScriptFrame() { ++Frame; Hero += 4.0f; }
    void Main()
    {
        const char* marker = Region == 1 ? "MK_FS1_HERO" : Region == 2 ? "MK_FS2_HERO" : Region == 3 ? "MK_FS3_HERO" : "MK_FS4_HERO";
        Log(marker);
        while (!(Dist(Me, Hero) < 15.0f) && !(Dist(Marker, Hero) < 5.0f) && !HitByHero)
            NewScriptFrame();
        JackReached = true;
        Log("OBJECT_FOCAL_SITE_BASE_01");
        JackLeaves = true;
        if (Region == 1) { Log("CS_FOCALSITE_1"); Log("EndGameSecondHSP"); }
        else if (Region == 2) { Log("CS_FOCALSITE_2"); Log("HobbeCave2HSP"); }
        else if (Region == 3) { Log("CS_FOCALSITE_3"); Log("BowerstoneJailHSP"); }
        else { Log("CS_FOCALSITE_4"); Log("remove_robe"); Log("GuildTrainingHSP"); }
        EndCutScenePlayed = true;
        Log("NEWTELEPORTER2");
    }
};
int main()
{
    Jack a = { 1, 0.0f, -40.0f, 100.0f, false, 0, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "MK_FS1_HERO;OBJECT_FOCAL_SITE_BASE_01;CS_FOCALSITE_1;EndGameSecondHSP;NEWTELEPORTER2;")
              && a.Frame == 7 && a.JackReached && a.JackLeaves && a.EndCutScenePlayed;
    g_log[0] = 0;
    Jack b = { 4, 0.0f, -100.0f, -100.0f, false, 0, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "MK_FS4_HERO;OBJECT_FOCAL_SITE_BASE_01;CS_FOCALSITE_4;remove_robe;GuildTrainingHSP;NEWTELEPORTER2;")
         && b.Frame == 0;
    g_log[0] = 0;
    Jack c = { 3, 0.0f, -100.0f, 50.0f, true, 0, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "MK_FS3_HERO;OBJECT_FOCAL_SITE_BASE_01;CS_FOCALSITE_3;BowerstoneJailHSP;NEWTELEPORTER2;") && c.Frame == 0;
    std::printf(ok ? "00d2d930_TEST PASS\n" : "00d2d930_TEST FAIL\n");
    return ok ? 0 : 1;
}
