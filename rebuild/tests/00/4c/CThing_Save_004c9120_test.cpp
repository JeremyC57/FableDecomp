// Behaviour test for CThing::Save (0x004C9120): the level-script text a thing writes.
#include <stdio.h>
#include <string>
#include <vector>
static std::string Bool(bool b) { return b ? "TRUE" : "FALSE"; }
struct TC {
    std::string text; bool heroState, toBeRemoved;
    TC(const char* t, bool h, bool r) : text(t), heroState(h), toBeRemoved(r) { }
    void SerialiseLevelState(bool saving, std::string& data, bool notInEditor) { if (saving) { data += text; data += notInEditor ? "+" : "-"; } }
};
struct Thing {
    std::string scriptName, scriptData; bool gameStatePersisted, levelPersistent, coopSpirit, isCreature, addingTCsPermitted;
    std::vector<TC> tcs;
    bool IsGameStatePersisted() const { if (coopSpirit) return false; return gameStatePersisted; }
    bool IsLevelPersistent() const { return levelPersistent; }
    void Save(std::string& data, bool editorActive, bool serialisingHeroState);
};
void Thing::Save(std::string& data, bool editorActive, bool serialisingHeroState)
{
    data += "[info]";                           // SerialiseThingInfo(true, data, info)
    data += "[createTCs]";                      // SaveSerialiseCreatableTCs(data)
    data += std::string("ScriptName ") + (scriptName == "" ? std::string("NULL") : scriptName) + ";\r\n";
    data += std::string("ScriptData \"") + (scriptData == "" ? std::string("NULL") : scriptData) + "\";\r\n";
    data += "ThingGamePersistent " + Bool(IsGameStatePersisted()) + ";\r\n";
    bool level_persistent = IsLevelPersistent();
    data += "ThingLevelPersistent " + Bool(level_persistent) + ";\r\n";
    bool hero_state_only = false;
    if (isCreature && serialisingHeroState)
        hero_state_only = true;
    addingTCsPermitted = false;
    for (size_t i = 0; i < tcs.size(); ++i) {
        TC* tc = &tcs[i];
        if (hero_state_only && !tc->heroState) continue;
        if (tc->toBeRemoved) continue;
        tc->SerialiseLevelState(true, data, !editorActive);
    }
    addingTCsPermitted = true;
}
int main(void)
{
    int fails = 0;
    Thing t; t.scriptName = ""; t.scriptData = "x y"; t.gameStatePersisted = true; t.levelPersistent = false; t.coopSpirit = false;
    t.isCreature = true; t.addingTCsPermitted = true;
    t.tcs.push_back(TC("A", true, false)); t.tcs.push_back(TC("B", false, false)); t.tcs.push_back(TC("C", true, true));
    std::string out;
    t.Save(out, false, false);
    if (out != "[info][createTCs]ScriptName NULL;\r\nScriptData \"x y\";\r\nThingGamePersistent TRUE;\r\nThingLevelPersistent FALSE;\r\nA+B+") { printf("FAIL 1: %s\n", out.c_str()); ++fails; }
    if (!t.addingTCsPermitted) { printf("FAIL 2\n"); ++fails; }
    out = ""; t.scriptName = "Bob"; t.scriptData = ""; t.coopSpirit = true; t.levelPersistent = true;
    t.Save(out, true, true);                    // hero state while in the editor: only the hero-state components, flag false
    if (out != "[info][createTCs]ScriptName Bob;\r\nScriptData \"NULL\";\r\nThingGamePersistent FALSE;\r\nThingLevelPersistent TRUE;\r\nA-") { printf("FAIL 3: %s\n", out.c_str()); ++fails; }
    out = ""; t.isCreature = false;
    t.Save(out, false, true);
    if (out.substr(out.size() - 4) != "A+B+") { printf("FAIL 4: %s\n", out.c_str()); ++fails; }
    if (fails) { printf("004c9120_TEST FAIL\n"); return 1; }
    printf("004c9120_TEST PASS\n");
    return 0;
}