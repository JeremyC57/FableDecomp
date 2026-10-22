#include <cstdio>
#include <cstring>
// behaviour test: CQ_MinionCampScript::Main -- switch off the Darkwood3 creature generators, bind the five scripted
// entity kinds, post the scripted entities (script and game) and spawn the DoMission thread
static int g_generatorsOff = 0, g_posted = 0, g_spawned = 0; static char g_bound[160];
struct CGameScriptInterface {
    void SetCreatureGeneratorsEnabled(const char* r, bool on) const { if (!std::strcmp(r, "Darkwood3") && !on) ++g_generatorsOff; }
    void PostAddScriptedEntities() const { ++g_posted; }
};
struct CQ_MinionCampScript {
    CGameScriptInterface* Game;
    void AddEntityScriptBinding(const char* n) { std::strcat(g_bound, n); std::strcat(g_bound, ","); }
    void PostAddScriptedEntities() { ++g_posted; }
    void SpawnFunction(const char* n) { if (!std::strcmp(n, "DoMission")) ++g_spawned; }
    void Main();
};
void CQ_MinionCampScript::Main()
{
    Game->SetCreatureGeneratorsEnabled("Darkwood3", false);
    AddEntityScriptBinding("BriarRose");
    AddEntityScriptBinding("MC_MinionGuard");
    AddEntityScriptBinding("MC_SpawnPoint");
    AddEntityScriptBinding("MC_Minion");
    AddEntityScriptBinding("FadeinSummoner");
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction("DoMission");
}
int main()
{
    CGameScriptInterface game; CQ_MinionCampScript s; s.Game = &game; g_bound[0] = 0;
    s.Main();
    bool ok = g_generatorsOff == 1 && g_posted == 2 && g_spawned == 1
           && std::strcmp(g_bound, "BriarRose,MC_MinionGuard,MC_SpawnPoint,MC_Minion,FadeinSummoner,") == 0;
    if (!ok) { std::printf("00d9e5e0_TEST FAIL\n"); return 1; }
    std::printf("00d9e5e0_TEST PASS\n");
    return 0;
}