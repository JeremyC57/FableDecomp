// NScript::CV_DemonDoorsScript::CDF_DoDamage::Main() -- test damage dealt to the door and unlock its region exit.
// Leaf: ?Main@CDF_DoDamage@CV_DemonDoorsScript@NScript@@EAEXXZ
// Grade: RELOCATION_MATCH(1970v1970) -- original header pause constructor expansion; keyword unproven.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include <stdlib.h>
#include "engine/TRetVoid.h"
enum EScriptAIPriority {
    SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1,
    SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3,
    SCRIPT_AI_PRIORITY_HIGHEST=4
};
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_DemonDoorsScript::CDF_DoDamage::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    if(seh_me.IsValid()) seh_me.Release();
    while(!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    }
    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    Game.SetThingPersistent(Me,true);
    Game.EntitySetMaxHealth(Me,1000.0f,true);
    Game.ModifyThingHealth(Me,1000.0f,false);
    Game.EntitySetAsKillable(Me,false,false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    CScriptThing region_exit=Game.GetNearestWithDefName(Me,"REGION_EXIT_POINT");
    Game.SetRegionExitAsActive(region_exit,false);
    float start_health=Game.GetHealth(Me);
    for(;true;Game.NewScriptFrame()) {
        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
        if(Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") &&
             !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            float new_health=Game.GetHealth(Me);
            if(start_health-new_health>pGlobalScriptDef->DemonDoor_DoDamage_DamageLimit) {
                if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_OPENING",GROUP_SELECT_NONE,false,true,false);
                    while(seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                ParentClass.FadeOutFaceAndOpenDoor(Me,"DD_DoDamage");
                Game.SetRegionExitAsActive(region_exit,true);
                return;
            } else {
                if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if(rand()%2) {
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_TOO_WEAK_A",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                } else {
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_TOO_WEAK_B",GROUP_SELECT_NONE,false,true,false);
                        while(seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                        }
                        if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                }
                Game.ModifyThingHealth(Me,1000.0f,false);
                start_health=Game.GetHealth(Me);
            }
        }
        if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if(Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_088_DO_DAMAGE_INTRO",GROUP_SELECT_NONE,false,true,false);
                while(seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                if(IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
        }
    }
}
