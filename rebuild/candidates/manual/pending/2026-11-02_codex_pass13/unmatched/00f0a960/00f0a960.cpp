// Leaf: ?Main@CAT_BanditContact@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Original Ego_r0x1143e, private virtual; native1985-byte CAT_BanditContact script.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_TEXT_COMPARE
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLPORT_MAP
#define FABLE_RETAIL_CTIMER
#include <string.h>
#pragma intrinsic(strcmp)
#include "engine/CCountedPointer.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptingStateGroups { ESSG_NONE=0, ESSG_PERFORM_ACTION_PHYSICAL=1, ESSG_PERFORM_ACTION_VERBAL=2, ESSG_PERFORM_ACTION_AURAL=3, ESSG_WANDER_NEAR=4, ESSG_FOLLOW_PATH=5, ESSG_FOLLOW_RANDOM=6, ESSG_FOLLOW_NEAREST=7, ESSG_WALK_TO_RANDOM=8, ESSG_WALK_TO_NEAREST_DIFFERENT=9, ESSG_RUN_AT_HERO_AND_ATTACK_UNTIL_DEAD=10 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& t):Thing(t) {}
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

void NScript::CQ_AmbushTradersScript::CAT_BanditContact::Main()
{
    CCharString regionName;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CScriptThing Exit;
    Game.SetWanderCentrePoint(Me,Me.GetPos());
    Game.SetWanderMinDistance(Me,0.0f);
    Game.SetWanderMaxDistance(Me,pGlobalScriptDef->AmbushTradersContactWanderDistance);
    Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
    Game.EntityGetAppearanceSeed(Me,ParentClass.ContactSeed);
    Game.SetThingHasInformation(Me,true,false,false);
    if(IsActiveThreadTerminating()) return;
    do {
        if(ParentClass.ContactAttackedByHero) {
            if(IsActiveThreadTerminating()) return;
            if(sehMe.IsValid()) sehMe.Release();
            while(!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(sehMe.GetScriptThing())>0.0f) {
                sehMe.Speak(Game.GetHero(),"TEXT_QST_003_CONTACT_ATTACK",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while(sehMe.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return;
                }
                if(IsActiveThreadTerminating()) return;
            }
            if(sehMe.IsValid()) sehMe.Release();
            Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
            ParentClass.ContactAttackedByHero=false;
        } else if(ParentClass.BanditContactIntroComplete && Game.MsgOnRegionUnloaded(regionName)) {
            if(IsActiveThreadTerminating()) return;
            if(regionName=="BowerstonePosh") {
                if(IsActiveThreadTerminating()) return;
                Game.RemoveThing(Me,false,true);
            }
        } else if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) return;
            if(!ParentClass.BanditContactIntroComplete) {
                if(IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectScriptedThingBase sehHero;
                Game.StartScriptingEntity(Game.GetHero(),sehHero,SCRIPT_AI_PRIORITY_HIGHEST);
                if(sehMe.IsValid()) sehMe.Release();
                while(!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) return;
                }
                if(IsActiveThreadTerminating()) return;
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csMap;
                csMap["HERO"]=sehHero;
                csMap["WOMAN"]=sehMe;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                Game.FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_GIBBETHILL_AMBUSH_PREINTRO",csMap,0,0,false,true);
                Game.FixMovieSequenceCamera(false);
                ParentClass.BanditContactIntroComplete=true;
                CScriptThing HSP=Game.GetThingWithScriptName("BowerstoneJailHSP");
                Game.EntityTeleportToThing(Game.GetHero(),HSP,false);
                Game.RemoveThing(Me,false,true);
            }
        }
        Game.NewScriptFrame();
    } while(!IsActiveThreadTerminating());
}
