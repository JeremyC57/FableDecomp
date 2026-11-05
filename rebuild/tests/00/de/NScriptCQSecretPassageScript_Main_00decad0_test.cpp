// Behaviour test for NScript::CQ_SecretPassageScript::Main (0x00DECAD0): the quest main binds the ten Prison Escape entity
// scripts (SP_UndeadTrigger with flags 1, the rest 0) in retail's order, registers them, writes logbook story entry 210, sets the
// quest card objective TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_01, spawns WatchForTermination then DoMission, and caches the two
// cutscene music sets. The model mirrors the source's call order through a log.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum EMusicSetType { MUSIC_SET_CUTSCENE_PRISON_MOTHER = 53, MUSIC_SET_CUTSCENE_JACK_CAPTURES = 56 };
class CGame { public:
    void PostAddScriptedEntities() { Log("gpost"); }
    const char* GetActiveQuestName() { return "Q_SecretPassage"; }
    void SetQuestCardObjective(const char* q, const char* o, const char* r, const char* t) { Log("obj"); Log(q); Log(o); Log(r); Log(t); }
    void CacheMusicSet(EMusicSetType t) { Log(t == MUSIC_SET_CUTSCENE_PRISON_MOTHER ? "music53" : t == MUSIC_SET_CUTSCENE_JACK_CAPTURES ? "music56" : "music?"); }
};
struct CQ_SecretPassageScript {
    CGame* Game;
    bool initialised;
    void InitialiseVariables() { initialised = true; Log("init"); }
    void Bind(const char* n, unsigned long flags) { Log("bind"); Log(n); Log(flags == 1 ? "1" : "0"); }
    void PostAddScriptedEntities() { Log("post"); }
    void AddLogbookStoryEntry(long id) { Log(id == 210 ? "log210" : "log?"); }
    void Spawn(const char* n) { Log("spawn"); Log(n); }
    void Main()
    {
        InitialiseVariables();
        Bind("SP_AmbushGuard", 0);
        Bind("SP_InvestigatingGuard", 0);
        Bind("SP_SecretPassageGuard", 0);
        Bind("SP_DeskGuard", 0);
        Bind("SP_UndeadTrigger", 1);
        Bind("SP_NymphTrigger", 0);
        Bind("SP_ScreamerTrigger", 0);
        Bind("MotherTorture", 0);
        Bind("TortureDoorCourtyard", 0);
        Bind("DoorToMothersTortureChamber", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        AddLogbookStoryEntry(210);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_01", "", "");
        Spawn("WatchForTermination");
        Spawn("DoMission");
        Game->CacheMusicSet(MUSIC_SET_CUTSCENE_PRISON_MOTHER);
        Game->CacheMusicSet(MUSIC_SET_CUTSCENE_JACK_CAPTURES);
    }
};
int main()
{
    CGame game;
    CQ_SecretPassageScript s;
    s.Game = &game;
    s.initialised = false;
    g_log[0] = 0;
    s.Main();
    const char* expected =
        "init;"
        "bind;SP_AmbushGuard;0;bind;SP_InvestigatingGuard;0;bind;SP_SecretPassageGuard;0;bind;SP_DeskGuard;0;"
        "bind;SP_UndeadTrigger;1;bind;SP_NymphTrigger;0;bind;SP_ScreamerTrigger;0;bind;MotherTorture;0;"
        "bind;TortureDoorCourtyard;0;bind;DoorToMothersTortureChamber;0;"
        "post;gpost;log210;obj;Q_SecretPassage;TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_01;;;"
        "spawn;WatchForTermination;spawn;DoMission;music53;music56;";
    if (!s.initialised || std::strcmp(g_log, expected) != 0) {
        std::printf("00decad0_TEST FAIL\n%s\n", g_log);
        return 1;
    }
    std::printf("00decad0_TEST PASS\n");
    return 0;
}