// NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main() -- stages the final decision and the Sister ending scene.
// Leaf: ?Main@CEndGameSister@CQ_EndGameBossBattleScript@NScript@@EAEXXZ
// Grade: DIFFER(2446v2484) -- Movie construction expansion and temporary allocation remain unresolved.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_EXTERNAL_DTOR
#define FABLE_RETAIL_STLPORT_MAP
#include "engine/CQ_EndGameBossBattleScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

void NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    while(!ParentClass.IntroFinished) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    Game.EntitySetTargetable(Me,false);
    Game.EntitySetAsDamageable(Me,false);
    Game.SetIsPushableByHero(Me,false);
    Game.EntitySetAsKillable(Me,false,true);
    Game.SetIsThingEpicSpellable(Me,false);
    Game.SetThingAsConscious(Me,false,"FALL_UNCONSCIOUS");
    Game.SetIsThingForcePushable(Me,false);
    Game.EntitySetEvadeAllHits(Me,true);
    while(!ParentClass.FinalDecision) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
    }
    if(IsActiveThreadTerminating()) return;
    Game.EntitySetTargetable(Me,true);
    Game.EntitySetAsDamageable(Me,true);
    Game.EntitySetEvadeAllHits(Me,false);
    Game.ModifyThingHealth(Me,10000.0f,false);
    Game.SetIsThingEpicSpellable(Me,true);
    {
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if(seh_Hero.IsValid()) seh_Hero.Release();
        while(!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) return;
        }
        if(IsActiveThreadTerminating()) return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["Hero"]=seh_Hero;
        Game.FadeScreenOut(1.3f,0.5f,CRGBColour(0,0,0,255));
        Game.Pause(0.5f);
        Game.SetThingAsConscious(Me,true,"");
        seh_me.ClearAllActionsIncludingLoopingAnimations();
        seh_me.ClearAllActions();
        seh_me.ClearCommands();
        Game.EntityTeleportToThing(Game.GetThingWithScriptName("EndGameSister"),Game.GetThingWithScriptName("MK_JACK1_HOLDJACK"),false);
        Game.EntitySetAsDrawable(Game.GetThingWithScriptName("EndGameSister"),false);
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        RunCutsceneMacro("CS_JACK_DEATH",mymap,0,0,false,true);
        Game.EntityTeleportToThing(Game.GetThingWithScriptName("EndGameSister"),Game.GetThingWithScriptName("MK_END1_SISTER_STOP"),false);
        Game.EntitySetAsDrawable(Game.GetThingWithScriptName("EndGameSister"),true);
        Game.RemoveThing(Game.GetThingWithScriptName("CS_AEONS"),false,true);
        Game.GiveHeroWeapon("OBJECT_SWORD_OF_AEONS",false);
        Game.SetWeaponAsHerosActiveWeapon("OBJECT_SWORD_OF_AEONS");
    }
    while(!ParentClass.FinalDecisionMade) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
        if(Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO","OBJECT_SWORD_OF_AEONS")) {
            if(IsActiveThreadTerminating()) return;
            ParentClass.FinalDecisionMade=true;
            PMasterData->JackBossBattleResult=1;
            CScriptGameResourceObjectScriptedThingBase seh_Hero;
            if(seh_Hero.IsValid()) seh_Hero.Release();
            while(!Game.StartScriptingEntity(Game.GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["Hero"]=seh_Hero;
            mymap["Sister"]=seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            RunCutsceneMacro("CS_JACK_BOSS_ENDING_EVIL",mymap,0,0,false,true);
            ParentClass.FinalCutscenePlayed=true;
        } else if(ParentClass.SisterNeededForCutscene) {
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(ParentClass.SisterNeededForCutscene) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            if(seh_me.IsValid()) seh_me.Release();
            while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
        }
    }
    while(!IsActiveThreadTerminating()) Game.NewScriptFrame();
}
