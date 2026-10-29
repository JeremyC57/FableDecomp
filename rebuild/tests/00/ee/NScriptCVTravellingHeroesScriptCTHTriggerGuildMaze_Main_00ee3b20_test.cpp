// Behaviour test for 0x00EE3B20 NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main(): the source's body
// over a scripted game interface (hero distance and the guild warning driven by the frame number), checked as an event log.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
static char gLog[4096];
static int gFrame = 0;
static int gTerminateAt = 1000;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { (void)length; }
    ~CCharString() { PStringData = 0; }
};
static int gThingsAlive = 0;
class CScriptThing { public:
    int Id;
    CScriptThing(int id) : Id(id) { ++gThingsAlive; }
    CScriptThing(const CScriptThing& other) : Id(other.Id) { ++gThingsAlive; }
    ~CScriptThing() { --gThingsAlive; }
};
namespace NScript { class CPredicateBase { public: virtual ~CPredicateBase() { } }; }
struct CIsThingAlive : public NScript::CPredicateBase {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
typedef long EGameflowPosition;
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }
static int gHero = 0;
static int HeroDistanceAt(int frame);
static bool WarningAt(int frame);
struct GameCalls {
    CScriptThing Hero;
    GameCalls() : Hero(0) { }
    void NewScriptFrame() const;
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const CCharString& name) const { Log(name.PStringData); return CScriptThing(7); }
    void RemoveThing(const CScriptThing& thing, bool a, bool b) const { char t[64]; std::sprintf(t, "RemoveThing %d %d %d", thing.Id, a, b); Log(t); }
    void SetThingHasInformation(const CScriptThing& thing, bool a, bool b, bool c) const { char t[64]; std::sprintf(t, "SetInfo %d %d %d %d @%d", thing.Id, a, b, c, gFrame); Log(t); }
    void ClearThingHasInformation(const CScriptThing& thing) const { char t[64]; std::sprintf(t, "ClearInfo %d @%d", thing.Id, gFrame); Log(t); }
    void SetThingPersistent(const CScriptThing& thing, bool persistent) const { char t[64]; std::sprintf(t, "Persistent %d %d", thing.Id, persistent); Log(t); }
    void RemoveRumourCategory(const CCharString& category) const { char t[96]; std::sprintf(t, "RemoveRumour %s", category.PStringData); Log(t); }
};
static bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance) { (void)a; (void)b; return HeroDistanceAt(gFrame) < distance; }
static bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance) { (void)a; (void)b; return HeroDistanceAt(gFrame) > distance; }
class CEntityScriptBase { public:
    GameCalls*   Game;
    CScriptThing Me;
    CEntityScriptBase() : Game(0), Me(1) { }
    virtual ~CEntityScriptBase() { }
    bool IsActiveThreadTerminating() const { return gFrame >= gTerminateAt; }
    void SetPredicate(const NScript::CPredicateBase&) { Log("SetPredicate"); }
};
namespace NScript {
class CQ_SunnyvaleMasterData { public:
    EGameflowPosition PostSavePosition;
    bool GuildWarningOccuring;
};
class CV_TravellingHeroesScript { public:
    bool MazeCutsceneDone;
    CV_TravellingHeroesScript() : MazeCutsceneDone(false) { }
    void GossipSetup(CCharString category, CCharString rumour, CCharString village, CCharString faction)
    {
        char t[200]; std::sprintf(t, "Gossip %s|%s|%s|%s", category.PStringData, rumour.PStringData, village.PStringData, faction.PStringData); Log(t);
    }
    void PlayCutscene(CCharString name) { char t[80]; std::sprintf(t, "Cutscene %s @%d", name.PStringData, gFrame); Log(t); }
    class CTH_Trigger_Guild_Maze;
};
class CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;
    CQ_SunnyvaleMasterData*    PMasterData;
    bool                       ExitThisLoop;
    void Main();
};
}

static NScript::CQ_SunnyvaleMasterData gMaster;
static int gScenario = 0;
static int HeroDistanceAt(int frame)
{
    if (gScenario == 0) return frame >= 3 ? 1 : 10;
    // scenario 1: the hero comes close during a guild warning (frames 4..6), walks away (frame 9), comes back at 12
    if (frame >= 12) return 1;
    if (frame >= 9) return 50;
    if (frame >= 4) return 1;
    return 10;
}
static bool WarningAt(int frame) { return gScenario == 1 && frame >= 4 && frame < 7; }

class CDriverGame : public GameCalls { };

static const char* Run(int scenario, long postSave, int terminateAt, bool* mazeDone)
{
    gLog[0] = 0; gFrame = 0; gScenario = scenario; gTerminateAt = terminateAt;
    gMaster.PostSavePosition = postSave; gMaster.GuildWarningOccuring = false;
    {
        GameCalls game;
        NScript::CV_TravellingHeroesScript parent;
        NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze maze;
        maze.Game = &game; maze.ParentClass = &parent; maze.PMasterData = &gMaster; maze.ExitThisLoop = false;
        maze.Main();
        *mazeDone = parent.MazeCutsceneDone;
    }
    return gLog;
}
void GameCalls::NewScriptFrame() const { ++gFrame; gMaster.GuildWarningOccuring = WarningAt(gFrame); }

void NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetThingPersistent(Me, true);
    if (PMasterData->PostSavePosition > 100) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveRumourCategory("TravHero Maze-Scythe");
        Game->RemoveThing(Me, false, true);
    }
    else {
        if (IsActiveThreadTerminating()) return;
        ParentClass->GossipSetup("TravHero Maze-Scythe", "TEXT_AI_GOSSIP_MAZE_SCYTHE_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE", "FACTION_GUILD");
    }
    CScriptThing preMelee = Game->GetThingWithScriptName("PreMeleeMaze");
    Game->SetThingHasInformation(preMelee, false, true, false);
    while (!ExitThisLoop) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f) && !PMasterData->GuildWarningOccuring) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (PMasterData->PostSavePosition > 100) {
            if (IsActiveThreadTerminating()) return;
            ExitThisLoop = true;
        }
        else if (PMasterData->GuildWarningOccuring) {
            if (IsActiveThreadTerminating()) return;
            while (PMasterData->GuildWarningOccuring) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->ClearThingHasInformation(preMelee);
            while (!IsDistanceBetweenThingsOver(Me, Game->GetHero(), 40.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->SetThingHasInformation(preMelee, false, true, false);
        }
        else {
            if (IsActiveThreadTerminating()) return;
            ParentClass->PlayCutscene("CS_TRAVHERO_MAZEARGUMENT");
            ExitThisLoop = true;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveRumourCategory("TravHero Maze-Scythe");
    ParentClass->MazeCutsceneDone = true;
    Game->ClearThingHasInformation(preMelee);
    Game->RemoveThing(Me, false, true);
}

int main()
{
    int fails = 0;
    bool done = false;
    const char* a = Run(0, 200, 1000, &done);
    const char* ea = "SetPredicate;Persistent 1 1;RemoveRumour TravHero Maze-Scythe;RemoveThing 1 0 1;PreMeleeMaze;SetInfo 7 0 1 0 @1;"
                     "RemoveRumour TravHero Maze-Scythe;ClearInfo 7 @3;RemoveThing 1 0 1;";
    if (std::strcmp(a, ea) || !done || gThingsAlive != 0) { std::printf("A: %s\n", a); ++fails; }
    const char* b = Run(1, 50, 1000, &done);
    const char* eb = "SetPredicate;Persistent 1 1;Gossip TravHero Maze-Scythe|TEXT_AI_GOSSIP_MAZE_SCYTHE_GUILD|VILLAGE_GUILD_COMPLEX_INSIDE|FACTION_GUILD;"
                     "PreMeleeMaze;SetInfo 7 0 1 0 @1;ClearInfo 7 @7;SetInfo 7 0 1 0 @9;Cutscene CS_TRAVHERO_MAZEARGUMENT @12;"
                     "RemoveRumour TravHero Maze-Scythe;ClearInfo 7 @12;RemoveThing 1 0 1;";
    if (std::strcmp(b, eb) || !done || gThingsAlive != 0) { std::printf("B: %s\n", b); ++fails; }
    const char* c = Run(1, 50, 5, &done);
    const char* ec = "SetPredicate;Persistent 1 1;Gossip TravHero Maze-Scythe|TEXT_AI_GOSSIP_MAZE_SCYTHE_GUILD|VILLAGE_GUILD_COMPLEX_INSIDE|FACTION_GUILD;"
                     "PreMeleeMaze;SetInfo 7 0 1 0 @1;";
    if (std::strcmp(c, ec) || done || gThingsAlive != 0) { std::printf("C: %s\n", c); ++fails; }
    const char* d = Run(0, 50, 1, &done);
    if (std::strcmp(d, "SetPredicate;") || done || gThingsAlive != 0) { std::printf("D: %s\n", d); ++fails; }
    if (fails == 0) std::printf("00ee3b20_TEST PASS\n");
    return fails;
}