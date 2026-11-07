// NScript::CV_SickChildScript::CSickChildsMother::Main() -- the sick child's mother: live while alive; take scripted control (seh_Mother) and flag her as having information unless the hero already holds the cure; then every frame: within 7 m before the intro play CS_SICK_INTRO and give the quest card (objective 01); talked to after the intro, ask TEXT_QST_B10_MOTHER_CURE_QUESTION when the hero carries the EndPotion (yes: CS_SICK_OUTRO, take the potion, FinishedQuest, leave the loop; no: ASK_CURE) or remind him (CURE_REMIND_10); when hit by the hero (except a heal-life spell) say TEXT_QST_B10_MOTHER_HIT and make the two allies; finally release her and idle
// Leaf: ?Main@CSickChildsMother@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ECD9E0, 2679 bytes, thiscall, plain `ret` (eight epilogue copies), frame 0xa4.
// Identity (proven): Ego_r.pdb ?Main@CSickChildsMother@CV_SickChildScript@NScript@@EAEXXZ; slot 1 of the CSickChildsMother entity vtable.
// Shape: the Codex pass-13 body (rebuild/candidates/manual/pending/2026-11-02_codex_pass13/unmatched/00ecd9e0) on the current headers;
//   MOVIEBASE_OUTOFLINE_DTOR (all ten movie dtor sites call 0x006E7B80) with the implicit movie ctor (VC7.1 calls 0x006E7B60 at the
//   intro and hit sites and expands the talk site, as retail); the plain-inline CIsThingAlive ctor with no inline dtor (retail calls
//   ~CIsThingAlive 0x00E241B0); both seh_Mother releases through a `parent` reference (`mov eax,[esi+14h]; lea edi,[eax+48h]`, chunk 808).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/C3DVector.h"
#include "engine/CV_SickChildScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
void NScript::CV_SickChildScript::CSickChildsMother::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CV_SickChildScript& parent = ParentClass;
        if (parent.seh_Mother.IsValid()) parent.seh_Mother.Release();
    }
    while (!Game->StartScriptingEntity(Me,ParentClass.seh_Mother,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (Game->IsObjectInThingsPossession(pGlobalScriptDef->EndPotion,Game->GetHero()) || !ParentClass.MotherIntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->SetThingHasInformation(Me,false,true,false);
    }
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!ParentClass.MotherIntroDone && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),7.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_SICK_INTRO","NULL");
            Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SICK_CHILD",Game->GetActiveQuestName(),false);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_SICK_CHILD_OBJECTIVE_01","","");
            ParentClass.MotherIntroDone=true;
            Game->ClearThingHasInformation(Me);
        }
        if (ParentClass.MotherIntroDone && Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->IsObjectInThingsPossession(pGlobalScriptDef->EndPotion,Game->GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroYesNoQuestion("TEXT_QST_B10_MOTHER_CURE_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long YNQAnswer;
                while ((YNQAnswer=Game->MsgIsQuestionAnsweredYesOrNo())<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (YNQAnswer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.PlayCutscene("CS_SICK_OUTRO","NULL");
                    Game->TakeObjectFromHero(pGlobalScriptDef->EndPotion);
                    Game->ClearThingHasInformation(Me);
                    ParentClass.FinishedQuest=true;
                    break;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
                        ParentClass.seh_Mother.Speak(Game->GetHero(),"TEXT_QST_B10_MOTHER_ASK_CURE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                        while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
                    ParentClass.seh_Mother.Speak(Game->GetHero(),"TEXT_QST_B10_MOTHER_CURE_REMIND_10",GROUP_SELECT_NONE,false,true,false);
                    while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Mother.GetScriptThing())>0.0f) {
                ParentClass.seh_Mother.Speak(Game->GetHero(),"TEXT_QST_B10_MOTHER_HIT",GROUP_SELECT_NONE,false,true,false);
                while (ParentClass.seh_Mother.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CV_SickChildScript& parent = ParentClass;
        if (parent.seh_Mother.IsValid()) parent.seh_Mother.Release();
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}