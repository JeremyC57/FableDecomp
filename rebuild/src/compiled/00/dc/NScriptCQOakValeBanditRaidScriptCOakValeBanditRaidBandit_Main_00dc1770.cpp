// NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit::Main() -- detect the hero or followers, close the gate and chase until recalled.
// Leaf: ?Main@COakValeBanditRaidBandit@CQ_OakValeBanditRaidScript@NScript@@EAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#include "engine/CQ_OakValeBanditRaidScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/TRetVoid.h"
// Row-local inline body (engine/CScriptThing.h declares ??8CScriptThing@@QBE_NABV0@@Z, out of line at 0x008AAE20; this TU expands it,
// the header's documented convention for CScriptThing's mixed inlining; Codex pass 13 FABLE_RETAIL_CSCRIPTTHING_EQUAL).
__forceinline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_DEFAULT=0, CUTSCENE_BEHAVIOUR_PAUSED=1, CUTSCENE_BEHAVIOUR_NOT_PAUSED=2 };
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& t):Thing(t) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
void NScript::CQ_OakValeBanditRaidScript::COakValeBanditRaidBandit::Main()
{
    TRetVoid _Fail_Val;
    long spotted;
    SetPredicate(CIsThingConscious(Me));
    Game->EntityAttachToScript(Me,"Q_OakValeBanditRaid");
    while (ParentClass.OpenGates) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_PAUSED);
    for (;;) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.BanditsAwareOfPlayer) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            spotted=ParentClass.CanThingSeeAnythingInVector(Me,ParentClass.HerosFollowers);
            if (ParentClass.BanditsStopChasing==0 && Game->GetHealth(Me)>0.0f &&
                (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Me.MsgIsHitBy("") || Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(),Me) || spotted>=0 ||
                 (ParentClass.IsBanditInRange && ParentClass.BanditInRange==Me))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.IsBanditInRange=false;
                ParentClass.StopWhistling=true;
                ParentClass.BanditsAwareOfPlayer=true;
                if (Game->IsSoundPlaying(ParentClass.SoundPlayed)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->StopSound(ParentClass.SoundPlayed);
                }
                Game->EntitySetAsKillable(Me,false,true);
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_me;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["ME"]=seh_me;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (spotted>=0 && !(Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) && !Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(),Me)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptThing ICU=ParentClass.HerosFollowers[spotted];
                    CScriptGameResourceObjectScriptedThingBase seh_merc;
                    if (seh_merc.IsValid()) seh_merc.Release();
                while (!Game->StartScriptingEntity(ICU,seh_merc,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    mymap["MERC"]=seh_merc;
                    RunCutsceneMacro("CS_BANDITRAID_GATECLOSE_MERC",mymap,0,0,false,true);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    RunCutsceneMacro("CS_BANDITRAID_GATECLOSE",mymap,0,0,false,true);
                }
                ParentClass.GateClosed=true;
                Game->EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_PAUSED);
                Game->EntitySetAsKillable(Me,true,true);
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            bool GoBackToWandering;
            do {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                spotted=ParentClass.CanThingSeeAnythingInVector(Me,ParentClass.HerosFollowers);
                if (Game->GetHealth(Me)>0.0f && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")) || Me.MsgIsHitBy("") || Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(),Me) || spotted>=0)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.BanditsAwareOfPlayerAgain=true;
                }
                GoBackToWandering=ParentClass.BanditsStopChasing!=0;
            } while (!GoBackToWandering);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            --ParentClass.BanditsStopChasing;
            Game->ClearThingBestEnemyTarget(Me);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        Game->NewScriptFrame();
    }
}