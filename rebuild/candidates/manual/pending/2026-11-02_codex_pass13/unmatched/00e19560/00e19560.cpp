// NScript::CQ_WhiteBalverineWWScript::SpawnBalverines() -- summon increasing balverine waves as health crosses quarter-health barriers.
// Leaf: ?SpawnBalverines@CQ_WhiteBalverineWWScript@NScript@@QAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_STLPORT_MAP
#include "engine/CQ_WhiteBalverineWWScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0,SCRIPT_AI_PRIORITY_LOW=1,SCRIPT_AI_PRIORITY_MEDIUM=2,SCRIPT_AI_PRIORITY_HIGH=3,SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const { if(!PImp.Get()) return NullCoord; return PImp->GetPos(); }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

void NScript::CQ_WhiteBalverineWWScript::SpawnBalverines()
{
    bool UsedBackupSpawnPoint=false;
    bool SummonCutsceneShown=false;
    Game.ModifyThingHealth(WhiteBalverine,250.0f,false);
    long HealthLossToTriggerSummon=(long)(Game.GetHealth(WhiteBalverine)*0.25f);
    long NextHealthBarrier=(long)(Game.GetHealth(WhiteBalverine)-HealthLossToTriggerSummon);
    long RoundNumber=1;
    while(RoundNumber<=3) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return;
        if(Game.GetHealth(WhiteBalverine)<NextHealthBarrier) {
            if(IsActiveThreadTerminating()) return;
            CScriptThing Marker=Game.GetThingWithScriptName("MK_WBW_FIRSTSPAWN");
            if(IsDistanceBetweenThingsUnder(Marker,Game.GetHero(),10.0f)) {
                if(IsActiveThreadTerminating()) return;
                Marker=Game.GetThingWithScriptName("MK_WBW_FIRSTSPAWNB");
                UsedBackupSpawnPoint=true;
            }
            for(long i=0;i<RoundNumber;++i) {
                if(IsActiveThreadTerminating()) return;
                CScriptThing NewBalv=Game.CreateCreature("CREATURE_BALVERINE_01",Marker.GetPos(),"WBWW_SoldierBalverine",false);
                Game.GiveThingBestEnemyTarget(NewBalv,Game.GetHero());
                Game.Pause(0.2f);
            }
            if(IsActiveThreadTerminating()) return;
            Game.EntitySetAsDamageable(WhiteBalverine,false);
            {
                CScriptGameResourceObjectScriptedThingBase seh_WhiteBalverine;
                Game.StartScriptingEntity(WhiteBalverine,seh_WhiteBalverine,SCRIPT_AI_PRIORITY_HIGHEST);
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.Pause(0.2f);
                    Game.EntitySetAttackThingImmediately(WhiteBalverine,Game.GetHero(),true,true);
                    Game.Pause(0.2f);
                    Game.PlaySoundOnThing(WhiteBalverine,"SND_LONGWOLFHOWL_01");
                    seh_WhiteBalverine.PlayAnimation("HOWL",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    while(seh_WhiteBalverine.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) return;
                    }
                    if(IsActiveThreadTerminating()) return;
                    Game.CameraDefault();
                }
                if(seh_WhiteBalverine.IsValid()) seh_WhiteBalverine.Release();
            }
            Game.EntitySetAsDamageable(WhiteBalverine,true);
            if(!SummonCutsceneShown) {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase seh_balv;
                CScriptThing CutsceneBalverine=Game.GetRandomThingWithScriptName("WBWW_SoldierBalverine");
                Game.StartScriptingEntity(CutsceneBalverine,seh_balv,SCRIPT_AI_PRIORITY_HIGHEST);
                if(UsedBackupSpawnPoint) {
                    if(IsActiveThreadTerminating()) return;
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["BALV"]=seh_balv;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_WBW_FIRSTSPAWNB",mymap,0,0,false,true);
                    Game.Pause(0.1f);
                    Game.CameraDefault();
                } else {
                    if(IsActiveThreadTerminating()) return;
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["BALV"]=seh_balv;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_WBW_FIRSTSPAWN",mymap,0,0,false,true);
                    Game.Pause(0.1f);
                    Game.CameraDefault();
                }
                SummonCutsceneShown=true;
            }
            NextHealthBarrier-=HealthLossToTriggerSummon;
            ++RoundNumber;
        }
    }
    if(IsActiveThreadTerminating()) return;
}
