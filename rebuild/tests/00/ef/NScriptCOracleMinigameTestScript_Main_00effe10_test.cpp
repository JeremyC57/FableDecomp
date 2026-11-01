// Behaviour test for NScript::COracleMinigameTestScript::Main (0x00EFFE10): three entity bindings, post the entities,
// wait for MissionSucceeded, then complete and deactivate the active quest.
#include <stdio.h>
#include <string.h>
class CGame { public:
    int frames; int postAdds; int completed; int deactivated; bool* success; int succeedAt;
    CGame(bool* s, int at) : frames(0), postAdds(0), completed(0), deactivated(0), success(s), succeedAt(at) {}
    void NewScriptFrame() { ++frames; if (frames >= succeedAt) *success = true; }
    void PostAddScriptedEntities() { ++postAdds; }
    const char* GetActiveQuestName() { return "Q_OracleMinigameTest"; }
    void SetQuestAsCompleted(const char* n, bool a, bool b, bool c) { if (strcmp(n, "Q_OracleMinigameTest") == 0 && a && b && !c) ++completed; }
    void DeactivateQuestLater(const char* n, unsigned long when) { if (strcmp(n, "Q_OracleMinigameTest") == 0 && when == 0) ++deactivated; }
};
class CScriptBase { public:
    CGame* Game; int bindings; int postAdds; int terminateAt; char names[3][32];
    CScriptBase(CGame* g, int t) : Game(g), bindings(0), postAdds(0), terminateAt(t) {}
    void AddEntityScriptBinding(const char* name, int flags) { if (flags == 0 && bindings < 3) strcpy(names[bindings++], name); }
    void PostAddScriptedEntities() { ++postAdds; }
    bool IsActiveThreadTerminating() { return Game->frames >= terminateAt; }
};
namespace NScript {
class COracleMinigameTestScript : public CScriptBase {
public:
    bool MissionSucceeded;
    COracleMinigameTestScript(CGame* g, int t) : CScriptBase(g, t), MissionSucceeded(false) {}
    void Main();
};
void COracleMinigameTestScript::Main()
{
    AddEntityScriptBinding("OracleGamesmaster", 0);
    AddEntityScriptBinding("GamesmasterOracleMarker", 0);
    AddEntityScriptBinding("OracleSpectator", 0);
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}
int main()
{
    bool ok = false;
    CGame g(&ok, 4);
    NScript::COracleMinigameTestScript s(&g, 1000);
    s.MissionSucceeded = false; g.success = &s.MissionSucceeded;
    s.Main();
    if (s.bindings != 3 || strcmp(s.names[0], "OracleGamesmaster") || strcmp(s.names[1], "GamesmasterOracleMarker") || strcmp(s.names[2], "OracleSpectator")) { printf("00effe10_TEST FAIL bindings\n"); return 1; }
    if (s.postAdds != 1 || g.postAdds != 1 || g.frames != 4 || g.completed != 1 || g.deactivated != 1) { printf("00effe10_TEST FAIL run frames=%d\n", g.frames); return 1; }
    bool ok2 = false; CGame g2(&ok2, 4);
    NScript::COracleMinigameTestScript s2(&g2, 2); g2.success = &s2.MissionSucceeded;
    s2.Main();
    if (g2.frames != 2 || g2.completed != 0 || g2.deactivated != 0) { printf("00effe10_TEST FAIL terminate\n"); return 1; }
    printf("00effe10_TEST PASS\n");
    return 0;
}