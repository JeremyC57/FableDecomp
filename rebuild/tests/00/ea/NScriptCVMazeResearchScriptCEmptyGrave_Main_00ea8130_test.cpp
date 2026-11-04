// 00ea8130 behaviour test: CEmptyGrave::Main (match.cpp body), host stand-ins. C++03, g++ -w.
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const std::string& s) { g_log += s; g_log += ";"; }

class CCharString { public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
    CCharString(const std::string& s) : S(s) { }
};
static CCharString operator+(const CCharString& a, const CCharString& b) { return CCharString(a.S + b.S); }
class CScriptThing { public:
    std::string Name;
    std::vector<int>* UsedByHero;    // per-frame "used by hero" answers
    int* Frame;
    CScriptThing() : UsedByHero(0), Frame(0) { }
    explicit CScriptThing(const std::string& n) : Name(n), UsedByHero(0), Frame(0) { }
    bool MsgIsUsedBy(const CCharString& name) const
    {
        if (name.S != "SCRIPT_NAME_HERO" || !UsedByHero) return false;
        return *Frame < (int)UsedByHero->size() && (*UsedByHero)[*Frame] != 0;
    }
};
class CScriptGameResourceObjectScriptedThingBase { public: std::string Who; };
class CScriptGameResourceObjectMovieBase { public: };
struct CCutsceneFlagMap { };
struct CCutsceneCastMap {
    std::map<std::string, CScriptGameResourceObjectScriptedThingBase> M;
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key) { return M[key.S]; }
};
struct CCutsceneArgMap;
static void RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6)
{
    (void)flags; (void)args; (void)a5; (void)a6;
    logs("cutscene " + name.S + " hero=" + cast.M["HERO"].Who);
}
class CSpawnedFuncBase { public:
    std::string Name;
    unsigned long Id;
    CSpawnedFuncBase(const CCharString& name, unsigned long id) : Name(name.S), Id(id) { }
    virtual ~CSpawnedFuncBase() { }
    virtual void SuspendableProcess() = 0;
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();
    T* PClass;
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) { }
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
static int g_frame = 0;
static bool g_paused = false;
class CGameScriptInterface { public:
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frame; }
    bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority)
    { object.Who = thing.Name; logs("script " + thing.Name); return priority == 4; }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const CCharString& name) const { return CScriptThing(name.S); }
    void DeactivateQuestLater(const CCharString& quest, unsigned long delay) const { (void)delay; logs("deactivate " + quest.S); }
    void MiniMapAddMarker(const CScriptThing& thing, const CCharString& marker) const { logs("marker+ " + thing.Name + " " + marker.S); }
    void MiniMapRemoveMarker(const CScriptThing& thing) const { logs("marker- " + thing.Name); }
    void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase&) { logs("movie [" + name.S + "]"); }
    void FixMovieSequenceCamera(bool fix) const { logs(fix ? "camfix" : "camfree"); }
    void PauseAllNonScriptedEntities(bool pause) { g_paused = pause; logs(pause ? "pause" : "unpause"); }
    void EntitySetInLimbo(const CScriptThing& thing, bool limbo, bool) const { logs(std::string("limbo ") + thing.Name + (limbo ? " on" : " off")); }
    CCharString GetActiveQuestName() const { return CCharString("Q_MAZE"); }
};
static std::vector<CSpawnedFuncBase*> g_spawned;
static std::vector<std::string> g_regions;
struct CV_MazeResearchScript {
    bool SwordTaken;
    bool BookRead;
    CScriptThing Sword;
    CCutsceneFlagMap cs_flags;
    void UnLimboSword() { logs("unlimbo"); }
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName)
    { g_spawned.push_back(func); g_regions.push_back(regionName.S); logs("spawn " + func->Name); }
    void SpawnFunction(CSpawnedFuncBase* func, const CCharString& region = CCharString("")) { AddSpawnedFunction(func, region); }
};
struct CQ_SunnyvaleMasterData {
    long PostSavePosition;
    long JackBossBattleResult;
    bool JackBossBattleHeroGoodAtEnd;
};
static int g_termCalls = 0, g_termAt = -1, g_stopFrame = 1000;
class CEntityScriptBase { public:
    CGameScriptInterface* Game;
    CScriptThing Me;
    bool IsActiveThreadTerminating() const
    {
        ++g_termCalls;
        return g_termCalls == g_termAt || g_frame >= g_stopFrame;
    }
};
struct CEmptyGrave : CEntityScriptBase {
    CV_MazeResearchScript* ParentClass;
    CQ_SunnyvaleMasterData* PMasterData;
    void Main();
};

void CEmptyGrave::Main()
{
    if (ParentClass->BookRead) {
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_VIGNETTE");
    }
    ParentClass->Sword = Game->GetThingWithScriptName("GoodSword");
    Game->EntitySetInLimbo(ParentClass->Sword, true, true);
    if (IsActiveThreadTerminating())
        return;
    do {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->BookRead && !ParentClass->SwordTaken && PMasterData->PostSavePosition > 1700 && PMasterData->JackBossBattleResult == 2) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    CScriptGameResourceObjectScriptedThingBase hero;
                    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
                    CCutsceneCastMap cast;
                    cast["HERO"] = hero;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CGameScriptInterface* game = Game;
                    game->PauseAllNonScriptedEntities(true);
                    Game->FixMovieSequenceCamera(true);
                    ParentClass->SpawnFunction(new CSpawnedFunc<CV_MazeResearchScript>(CCharString("ParentClass.") + CCharString("UnLimboSword"), 0, ParentClass, &CV_MazeResearchScript::UnLimboSword));
                    if (PMasterData->JackBossBattleHeroGoodAtEnd) {
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                        RunCutsceneMacro("CS_GET_SWORD_OF_AEONS_SAINT", cast, &ParentClass->cs_flags, 0, false, true);
                    } else {
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
                            return;
                        }
                        RunCutsceneMacro("CS_GET_SWORD_OF_AEONS_REDEEM", cast, &ParentClass->cs_flags, 0, false, true);
                    }
                    Game->FixMovieSequenceCamera(false);
                    Game->MiniMapRemoveMarker(Me);
                    game->PauseAllNonScriptedEntities(false);
                }
                ParentClass->SwordTaken = true;
                Game->MiniMapRemoveMarker(Me);
                Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
            }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}

static void reset(int stopFrame, int termAt)
{
    g_log = ""; g_frame = 0; g_termCalls = 0; g_termAt = termAt; g_stopFrame = stopFrame; g_paused = false;
    for (size_t i = 0; i < g_spawned.size(); ++i) delete g_spawned[i];
    g_spawned.clear(); g_regions.clear();
}

int main()
{
    CGameScriptInterface game; game.Hero = CScriptThing("HERO_THING");
    CV_MazeResearchScript parent; parent.SwordTaken = false; parent.BookRead = true;
    CQ_SunnyvaleMasterData master = { 1800, 2, true };
    std::vector<int> used; used.push_back(0); used.push_back(1); used.push_back(0); used.push_back(1);
    CEmptyGrave grave;
    grave.Game = &game; grave.Me = CScriptThing("GRAVE"); grave.Me.UsedByHero = &used; grave.Me.Frame = &g_frame;
    grave.ParentClass = &parent; grave.PMasterData = &master;

    // full path, good hero: marker, limbo, then on the first use the saint cut-scene; a later use does nothing
    reset(4, -1);
    grave.Main();
    CHECK(parent.Sword.Name == "GoodSword");
    CHECK(g_log == "marker+ GRAVE HUD_ORB_QUEST_VIGNETTE;limbo GoodSword on;"
                   "script HERO_THING;movie [];pause;camfix;spawn ParentClass.UnLimboSword;"
                   "cutscene CS_GET_SWORD_OF_AEONS_SAINT hero=HERO_THING;camfree;marker- GRAVE;unpause;"
                   "marker- GRAVE;deactivate Q_MAZE;");
    CHECK(parent.SwordTaken && !g_paused);
    CHECK(g_spawned.size() == 1 && g_regions[0] == "" && g_spawned[0]->Id == 0);
    g_spawned[0]->SuspendableProcess();
    CHECK(g_log.find("unlimbo;") != std::string::npos);
    CHECK(g_frame == 4);

    // evil hero: the redeem cut-scene; no book: no marker and no cut-scene
    parent.SwordTaken = false; master.JackBossBattleHeroGoodAtEnd = false;
    reset(4, -1); grave.Main();
    CHECK(g_log.find("CS_GET_SWORD_OF_AEONS_REDEEM") != std::string::npos);
    parent.SwordTaken = false; parent.BookRead = false;
    reset(4, -1); grave.Main();
    CHECK(g_log == "limbo GoodSword on;" && !parent.SwordTaken);
    parent.BookRead = true;

    // the game position and the boss result gate the scene
    master.PostSavePosition = 1700; reset(4, -1); grave.Main();
    CHECK(!parent.SwordTaken && g_spawned.empty());
    master.PostSavePosition = 1800; master.JackBossBattleResult = 1; reset(4, -1); grave.Main();
    CHECK(!parent.SwordTaken);
    master.JackBossBattleResult = 2;

    // terminating right before the cut-scene: entities unpaused, nothing taken
    // calls: 1 book marker, 2 after limbo, 3 loop test (frame 1), 4 used, 5 gate, 6 before the cut-scene
    reset(4, 6); grave.Main();
    CHECK(g_log.find("cutscene") == std::string::npos && !g_paused && !parent.SwordTaken);
    CHECK(g_spawned.size() == 1);

    // terminating at the start: only the marker
    reset(4, 1); grave.Main();
    CHECK(g_log == "");
    reset(4, 2); grave.Main();
    CHECK(g_log == "marker+ GRAVE HUD_ORB_QUEST_VIGNETTE;limbo GoodSword on;");

    reset(0, -1);
    if (!g_fail) printf("00ea8130_TEST PASS\n");
    return g_fail;
}