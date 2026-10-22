// Behaviour test for 0x008A9100 CGameScriptInterface::CreateCreature: no def -> the null script thing and nothing else; a def -> create with the def's player and the script name, return the creature's script thing, start the appear action when asked, then wait CreateCreatureDelayFrames frames.
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static void Log(const std::string& s) { g_log += s; g_log += ";"; }
struct Def { int player; };
struct Creature { int id; std::string scriptName; int player; };
static Def g_def;
static bool g_haveDef;
static Creature g_creature;
struct Init { std::string ScriptName; std::string BrainName; bool PlayerCharacter, Drawable, PlaceAtEnd, AutoPlace;
    Init() : ScriptName(""), BrainName(""), PlayerCharacter(false), Drawable(true), PlaceAtEnd(true), AutoPlace(false) {} };
static Creature* Create(int player, const Init& init)
{
    char b[64]; std::sprintf(b, "create:%s:p%d:%d%d%d%d", init.ScriptName.c_str(), player, init.PlayerCharacter, init.Drawable, init.PlaceAtEnd, init.AutoPlace);
    Log(b);
    g_creature.id = 7; g_creature.scriptName = init.ScriptName; g_creature.player = player;
    return &g_creature;
}
struct GSI {
    int CreateCreatureDelayFrames;
    void NewScriptFrame() { Log("frame"); }
    int CreateCreature(const std::string& scriptName, bool appear)
    {
        int result = 0;
        if (g_haveDef) {
            Init init;
            init.ScriptName = scriptName;
            Creature* c = Create(g_def.player, init);
            result = c->id;
            if (appear) Log("appear");
            for (int i = CreateCreatureDelayFrames; i > 0; --i) NewScriptFrame();
        }
        return result;
    }
};
static int Check(int got, int want, const char* expect)
{
    if (got != want || g_log != expect) { std::printf("got %d %s\nexp %d %s\n", got, g_log.c_str(), want, expect); return 1; }
    return 0;
}
int main()
{
    int bad = 0; GSI gsi; g_def.player = 4;
    gsi.CreateCreatureDelayFrames = 3; g_haveDef = false; g_log = "";
    bad += Check(gsi.CreateCreature("Bandit", true), 0, "");
    g_haveDef = true; g_log = "";
    bad += Check(gsi.CreateCreature("Bandit", true), 7, "create:Bandit:p4:0110;appear;frame;frame;frame;");
    gsi.CreateCreatureDelayFrames = 0; g_log = "";
    bad += Check(gsi.CreateCreature("Guard", false), 7, "create:Guard:p4:0110;");
    gsi.CreateCreatureDelayFrames = -2; g_log = "";
    bad += Check(gsi.CreateCreature("", false), 7, "create::p4:0110;");
    if (bad) { std::printf("008a9100_TEST FAIL\n"); return 1; }
    std::printf("008a9100_TEST PASS\n"); return 0;
}