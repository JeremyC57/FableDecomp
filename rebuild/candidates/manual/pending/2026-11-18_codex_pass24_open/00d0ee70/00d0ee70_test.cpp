#include <stdio.h>
#include <map>
#include <string>
#include <vector>

typedef std::string CCharString;
struct Pos { int Value; };
struct CScriptThing {
    int Tag;
    bool Open;
    unsigned long Kills;
    CScriptThing(int tag=0,bool open=false,unsigned long kills=0):Tag(tag),Open(open),Kills(kills) {}
    Pos GetPos() const { Pos pos={Tag}; return pos; }
    bool IsOpenDoor() const { return Open; }
    bool MsgGetThingsKilled(std::vector<unsigned long>& out) const
    {
        out.assign(Kills,1UL);
        return Kills!=0;
    }
};
struct CScriptGameResourceObjectMovieBase {};
struct CScriptGameResourceObjectScriptedThingBase {};
enum EScriptAIPriority { SCRIPT_PRIORITY=4 };
struct CRGBColour {
    CRGBColour(int,int,int,int) {}
};
struct ScriptGame {
    unsigned Frame,GateOpens,LevelUnload,StopsAt;
    unsigned Movies,HeroScripting,Fades,Pauses,Removals,Spawns;
    bool Bad;
    unsigned long HeroKills,FollowerKills,SummonKills;
    std::vector<int> Targets;
    ScriptGame():Frame(0),GateOpens(1),LevelUnload(3),StopsAt(9),Movies(0),HeroScripting(0),Fades(0),
        Pauses(0),Removals(0),Spawns(0),Bad(false),HeroKills(2),FollowerKills(1),SummonKills(2) {}
    CScriptThing GetThingWithScriptName(const CCharString& name) const
    {
        if(name=="Gate1") return CScriptThing(10,Frame>=GateOpens);
        if(name=="Gate2Guard1") return CScriptThing(20);
        if(name=="DefensiveGuardLeader") return CScriptThing(30);
        return CScriptThing(-1);
    }
    void NewScriptFrame() { ++Frame; }
    bool IsLevelLoaded(const CCharString& name) const { return name=="BanditCampMain" && Frame<LevelUnload; }
    CScriptThing GetHero() const { return CScriptThing(1,false,HeroKills); }
    void GetFollowingEntityList(const CScriptThing& hero,std::vector<CScriptThing>& out)
    { Bad=Bad||hero.Tag!=1;out.assign(1,CScriptThing(2,false,FollowerKills)); }
    void GetHeroSummonedCreaturesList(std::vector<CScriptThing>& out) const
    { out.assign(1,CScriptThing(3,false,SummonKills)); }
    void StartMovieSequence(const CCharString& name,CScriptGameResourceObjectMovieBase&) { Bad=Bad||!name.empty(); ++Movies; }
    void StartScriptingEntity(const CScriptThing& hero,CScriptGameResourceObjectScriptedThingBase&,EScriptAIPriority priority)
    { Bad=Bad||hero.Tag!=1||priority!=SCRIPT_PRIORITY; ++HeroScripting; }
    void FadeScreenOut(float t,float hold,const CRGBColour&) { Bad=Bad||t!=0.5f||hold!=0.5f; ++Fades; }
    void Pause(float seconds) { Bad=Bad||seconds!=1.0f; ++Pauses; }
    void RemoveThing(const CScriptThing& thing,bool first,bool second) { Bad=Bad||thing.Tag!=20||first||!second; ++Removals; }
    void GetAllThingsWithScriptName(const CCharString& name,std::vector<CScriptThing>& out)
    { Bad=Bad||name!="DefensiveGuardBandit";out.push_back(CScriptThing(4));out.push_back(CScriptThing(5)); }
    void GiveThingBestEnemyTarget(const CScriptThing& thing,const CScriptThing& hero)
    { Bad=Bad||hero.Tag!=1;Targets.push_back(thing.Tag); }
    CScriptThing CreateCreature(const CCharString& name,Pos pos,const CCharString& label,bool activate)
    {
        Bad=Bad||name!="CREATURE_BANDIT_LEADER_LEVEL2"||pos.Value!=30||!label.empty()||activate;
        ++Spawns;
        return CScriptThing(6);
    }
};
struct CWideScreenMagicPauseEntities {
    explicit CWideScreenMagicPauseEntities(ScriptGame&) {}
};
struct GlobalScriptDef { float BAC_EnemiesToKillInFirstArea; };
struct GlobalScriptDef fixtureDef={5.0f};
GlobalScriptDef* pGlobalScriptDef=&fixtureDef;
namespace NScript {
struct CQ_BanditCampBossBattleScript {
    ScriptGame* Game;
    bool Gate2Open;
    unsigned Cutscenes;
    explicit CQ_BanditCampBossBattleScript(ScriptGame& game):Game(&game),Gate2Open(false),Cutscenes(0) {}
    bool IsActiveThreadTerminating() const { return Game->Frame>=Game->StopsAt; }
    void RunCutsceneMacro(const CCharString& name,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>& cast,
        int first,int second,bool third,bool fourth)
    {
        Game->Bad=Game->Bad||name!="CS_BANDITCAMP_ALARM_OUTER"||!cast.empty()||first||second||third||!fourth;
        ++Cutscenes;
    }
    void CheckForFirstAreaMassacre();
};
}

void NScript::CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre()
{
    while (!Game->GetThingWithScriptName("Gate1").IsOpenDoor()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    long killed = 0;
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (Game->IsLevelLoaded("BanditCampMain")) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsLevelLoaded("BanditCampMain")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        std::vector<unsigned long> kills;
        if (Game->GetHero().MsgGetThingsKilled(kills)) {
            if (IsActiveThreadTerminating())
                return;
            killed += kills.size();
            kills.clear();
        }
        std::vector<CScriptThing> things;
        Game->GetFollowingEntityList(Game->GetHero(), things);
        for (unsigned long i = 0; i < things.size(); i++) {
            if (IsActiveThreadTerminating())
                return;
            if (things[i].MsgGetThingsKilled(kills)) {
                if (IsActiveThreadTerminating())
                    return;
                killed += kills.size();
                kills.clear();
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GetHeroSummonedCreaturesList(things);
        for (unsigned long i = 0; i < things.size(); i++) {
            if (IsActiveThreadTerminating())
                return;
            if (things[i].MsgGetThingsKilled(kills)) {
                if (IsActiveThreadTerminating())
                    return;
                killed += kills.size();
                kills.clear();
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if ((float)killed >= pGlobalScriptDef->BAC_EnemiesToKillInFirstArea) {
            if (IsActiveThreadTerminating())
                return;
            {
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                CScriptGameResourceObjectScriptedThingBase seh_Hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_Hero, (EScriptAIPriority)4);
                Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
                Game->Pause(1.0f);
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
                RunCutsceneMacro("CS_BANDITCAMP_ALARM_OUTER", cast, 0, 0, false, true);
                Game->RemoveThing(Game->GetThingWithScriptName("Gate2Guard1"), false, true);
            }
            Gate2Open = true;
            std::vector<CScriptThing> bandits;
            Game->GetAllThingsWithScriptName("DefensiveGuardBandit", bandits);
            for (unsigned long i = 0; i < bandits.size(); i++) {
                if (IsActiveThreadTerminating())
                    return;
                Game->GiveThingBestEnemyTarget(bandits[i], Game->GetHero());
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptThing leader = Game->CreateCreature("CREATURE_BANDIT_LEADER_LEVEL2", Game->GetThingWithScriptName("DefensiveGuardLeader").GetPos(), "", false);
            Game->GiveThingBestEnemyTarget(leader, Game->GetHero());
            return;
        }
    }
}

int main()
{
    ScriptGame scriptGame;
    NScript::CQ_BanditCampBossBattleScript script(scriptGame);
    script.CheckForFirstAreaMassacre();
    if(!script.Gate2Open || scriptGame.Frame!=3 || scriptGame.Bad || script.Cutscenes!=1 ||
        scriptGame.Movies!=1 || scriptGame.HeroScripting!=1 || scriptGame.Fades!=1 || scriptGame.Pauses!=1 ||
        scriptGame.Removals!=1 || scriptGame.Spawns!=1 || scriptGame.Targets.size()!=3 ||
        scriptGame.Targets[0]!=4 || scriptGame.Targets[1]!=5 || scriptGame.Targets[2]!=6)
        return 1;
    ScriptGame blocked;
    blocked.GateOpens=10;blocked.StopsAt=2;
    NScript::CQ_BanditCampBossBattleScript cancelled(blocked);
    cancelled.CheckForFirstAreaMassacre();
    if(cancelled.Gate2Open || blocked.Frame!=2 || cancelled.Cutscenes || blocked.Spawns)
        return 2;
    ScriptGame few;
    few.HeroKills=0;few.FollowerKills=1;few.SummonKills=0;
    NScript::CQ_BanditCampBossBattleScript accumulation(few);
    accumulation.CheckForFirstAreaMassacre();
    if(!accumulation.Gate2Open || few.Frame!=7 || accumulation.Cutscenes!=1 || few.Bad)
        return 3;
    puts("00d0ee70_TEST PASS");
    return 0;
}
