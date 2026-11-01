// Behaviour test for NScript::CQ_GuildTrainingDepartureScript::Main (0x00D50800): bind TheRealGuildmaster, post the
// entities, wait for Q_GuildTrainingWoodsDeparture to become active and then to end, then deactivate the active quest.
#include <stdio.h>
#include <string.h>
class CGame { public:
    int activeFrames; int endedAfter; int frames; int postAdds; int deactivated; const char* quest;
    CGame(int a, int e) : activeFrames(a), endedAfter(e), frames(0), postAdds(0), deactivated(0), quest("Q_GuildTrainingWoodsDeparture") {}
    bool IsQuestActive(const char* name) { if (strcmp(name, quest) != 0) return false; return frames >= activeFrames && frames < endedAfter; }
    void NewScriptFrame() { ++frames; }
    void PostAddScriptedEntities() { ++postAdds; }
    const char* GetActiveQuestName() { return quest; }
    void DeactivateQuestLater(const char* name, unsigned long when) { if (strcmp(name, quest) == 0 && when == 0) ++deactivated; }
};
class CScriptBase { public:
    CGame* Game; int bindings; int postAdds; int terminateAt;
    CScriptBase(CGame* g, int t) : Game(g), bindings(0), postAdds(0), terminateAt(t) {}
    void AddEntityScriptBinding(const char* name, int flags) { if (strcmp(name, "TheRealGuildmaster") == 0 && flags == 1) ++bindings; }
    void PostAddScriptedEntities() { ++postAdds; }
    bool IsActiveThreadTerminating() { return Game->frames >= terminateAt; }
};
namespace NScript {
class CQ_GuildTrainingDepartureScript : public CScriptBase {
public:
    CQ_GuildTrainingDepartureScript(CGame* g, int t) : CScriptBase(g, t) {}
    void Main();
};
void CQ_GuildTrainingDepartureScript::Main()
{
    AddEntityScriptBinding("TheRealGuildmaster", 1);
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsQuestActive("Q_GuildTrainingWoodsDeparture")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (Game->IsQuestActive("Q_GuildTrainingWoodsDeparture") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}
int main()
{
    CGame g(3, 7);
    NScript::CQ_GuildTrainingDepartureScript s(&g, 1000);
    s.Main();
    if (s.bindings != 1 || s.postAdds != 1 || g.postAdds != 1 || g.frames != 7 || g.deactivated != 1) { printf("00d50800_TEST FAIL run frames=%d\n", g.frames); return 1; }
    CGame g2(3, 7);
    NScript::CQ_GuildTrainingDepartureScript s2(&g2, 5);
    s2.Main();
    if (g2.frames != 5 || g2.deactivated != 0) { printf("00d50800_TEST FAIL terminate frames=%d\n", g2.frames); return 1; }
    printf("00d50800_TEST PASS\n");
    return 0;
}