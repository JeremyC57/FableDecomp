// NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main() -- the Witchwood rock troll: spawn the parent's WatchForRockTrollKilled thread on Me; take scripted control; give it the two RockTrollReward items once; play CS_ROCKTROLL_EXHUME (HERO / TROLL) once under a paused movie and spawn WatchForRockTrollHit; play SPECIAL_BOAST then SPECIAL_IDLE and wait; release control, face and target the hero, then idle
// Grade: DIFFER(1524v1540) -- md 15, one residue: the WatchForRockTrollKilled CSpawnedFunc1 argument copy of Me calls CScriptThing(const CScriptThing&) 0x004ABE90 where retail expands it; every inline copy spelling (row-local inline, FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY, CBaseClass init, explicit temporary) also expands the WatchForRockTrollHit argument and the CSpawnedFunc1 Par1 copy (per-site CScriptThing copy split)
// Next idea: a CSpawnedFunc1 header model whose ctor takes Par1 by value with an inline body, so the first argument copy is made in a different inline context from the Par1 member copy
// Leaf: ?Main@CRTFE_RockTroll@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_RockTrollFirstEncounterScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/fable_script_def_global.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();                         // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();                        // 0x00CDBFB0

void NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main()
{
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_RockTrollFirstEncounterScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForRockTrollKilled"),0,&ParentClass,&CV_RockTrollFirstEncounterScript::WatchForRockTrollKilled,Me),CCharString(""));
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass.AddedItemsToRockTroll) {
        if (IsActiveThreadTerminating())
            return;
        Game->AddItemToContainer(Me,pGlobalScriptDef->RockTrollReward1);
        Game->AddItemToContainer(Me,pGlobalScriptDef->RockTrollReward2);
        ParentClass.AddedItemsToRockTroll=true;
    }
    if (!ParentClass.PlayedExhumeCutScene) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid())
            seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["TROLL"]=seh_me;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_ROCKTROLL_EXHUME",mymap,0,0,false,true);
        ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_RockTrollFirstEncounterScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForRockTrollHit"),0,&ParentClass,&CV_RockTrollFirstEncounterScript::WatchForRockTrollHit,Me),CCharString(""));
        ParentClass.PlayedExhumeCutScene=true;
    }
    seh_me.PlayAnimation("SPECIAL_BOAST",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    seh_me.PlayAnimation("SPECIAL_IDLE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    while (seh_me.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_me.IsValid())
        seh_me.Release();
    Game->EntityForceToLookAtThing(Me,Game->GetHero());
    Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
