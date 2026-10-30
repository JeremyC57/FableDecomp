// Behaviour test for 0x00EFF130 NScript::CTonyTestScriptsScript::Main: bind the "Dummy" entity script (flags 0), post-add
// the scripted entities (script, then the game interface) and add the quests to the guild, in that order.
#include <stdio.h>
#include <string.h>

static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CGame { public:
    void PostAddScriptedEntities() { Log("GamePost"); }
};
struct CTonyTestScripts {
    CGame* Game;
    void AddEntityScriptBinding(const char* name, unsigned long flags) { Log(name); if (flags) Log("FlagsBad"); }
    void PostAddScriptedEntities() { Log("ScriptPost"); }
    void AddQuestsToGuild() { Log("Guild"); }
    void Main()
    {
        AddEntityScriptBinding("Dummy", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        AddQuestsToGuild();
    }
};

int main()
{
    CGame g; CTonyTestScripts s; s.Game = &g; g_log[0] = 0;
    s.Main();
    if (strcmp(g_log, "Dummy;ScriptPost;GamePost;Guild;") != 0) { printf("00eff130_TEST FAIL\n"); return 1; }
    printf("00eff130_TEST PASS\n");
    return 0;
}