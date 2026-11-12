// NScript::CV_SickChildScript::CManInLove::Main() -- the man in love (V_SickChild)
// Grade: DIFFER(6394v6394) -- size equal, 91 masked lines (chunk H40b; was 107 and 30 bytes short): the exits are SOLVED -- the main loop is `for (;; NSF) { if (T) return ...; }` (the entry check 0x00EC8359 then shares the last pad's epilogue 0x00EC9BEF) and the first in-loop thread exit calls ~seh_me under #pragma inline_depth(0) (0x00EC85E2 `jne 0x00EC9BEA`; expanded it merged into the loop exit). Left, one cause: the &Me CSE temporary (c2prio candidate `temp` lines 42..332, 31 refs, priority -311, split into pieces) gets edi in the three reward arms (0x00EC8D3C / 0x00EC9060 / 0x00EC91B3 `lea edi,[esi+8]` ... `push edi`) where retail recomputes `lea eax,[esi+8]` at both uses; with edi held, arm 2's else branch and tail no longer cross-jump into arm 3's (retail 0x00EC9087 `je 0x00EC91EF`), and the eax / edx scratch pairs follow; plus one `push eax` scheduled two instructions early at 0x00EC8372
// Next idea: the 2-reference piece of the &Me temporary must be dropped in each arm (a 2-reference piece stays in K and takes a free callee-saved register): find which candidate holds edi / ebx / ebp across the arms in retail (the Pause Game pointer is in ebp at the arms' join 0x00EC96B3) with `c2prio --trace` on the split pass, then spell that candidate's range (the answer / answer2 locals and the two PauseMyEntitiesPlease scopes are the ones that take edi here)
// Leaf: ?Main@CManInLove@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC8300, 6394 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CManInLove@CV_SickChildScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012F1A94 that the landed CManInLove::Alloc 0x00EC8160
//   stores (slot 0 ??_G 0x00EC82E0, 2 Init 0x00EC8210, 4 OnPersist 0x00ECD8E0).
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/C3DVector.h"
#include "engine/EHeroAbility.h"
#include "engine/CV_SickChildScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE3EA
void NScript::CV_SickChildScript::CManInLove::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (Game->IsObjectInThingsPossession(pGlobalScriptDef->ObjectFromMansLover,Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",Game->GetHero())
        || !GivenLetter || ParentClass.MansLoverState==ML_KILLED) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->SetThingHasInformation(Me,true,true,false);
    }
    Game->SetIsPushableByHero(Me,false);
    Game->SetCreatureBrain(Me,"BRAIN_PASSIVE_OVERRIDE");
    Game->SetWanderCentrePoint(Me,Me.GetPos());
    Game->SetWanderMinDistance(Me,0.0f);
    Game->SetWanderMaxDistance(Me,10.0f);
    Game->SetScriptingStateGroup(Me,(EScriptingStateGroups)4);
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) {
#pragma inline_depth(0)                                  // inline control: retail calls ~seh_me at this exit (0x00EC85E2 `jne 0x00EC9BEA`)
                return;
#pragma inline_depth()
            }
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!GivenLetter && ParentClass.MansLoverState!=ML_KILLED) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO_50",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->GiveHeroObject(pGlobalScriptDef->ObjectToGiveToMansLover,-1,false);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO_60",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_INTRO_70",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                GivenLetter=true;
                Game->ClearThingHasInformation(Me);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if ((Game->IsObjectInThingsPossession(pGlobalScriptDef->ObjectFromMansLover,Game->GetHero())
                    || Game->IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER",Game->GetHero())) && !GotReward) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->TakeObjectFromHero(pGlobalScriptDef->ObjectFromMansLover);
                        Game->TakeObjectFromHero("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER");
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                        Game->RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                        if (ParentClass.MansLoverState==ML_NOT_IN_LOVE_WITH_YOU) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->GiveHeroMorality(pGlobalScriptDef->LyingToManAboutWomanLikingHer);
                        }
                        GotReward=true;
                        HelpedGuyOut=true;
                        Game->ClearThingHasInformation(Me);
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        Game->GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_WOMANS_OPINION_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                        long answer2=Game->MsgIsQuestionAnsweredYesOrNo();
                        while (answer2<0) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            answer2=Game->MsgIsQuestionAnsweredYesOrNo();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (answer2==1) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_HAPPY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                            Game->RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                            GotReward=true;
                            HelpedGuyOut=true;
                            if (ParentClass.MansLoverState==ML_NOT_IN_LOVE_WITH_YOU) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                            }
                            else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->GiveHeroMorality(pGlobalScriptDef->LyingToManAboutWomanLikingHer);
                            }
                            Game->ClearThingHasInformation(Me);
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                            Game->RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                            GotReward=true;
                            if (ParentClass.MansLoverState==ML_NOT_IN_LOVE_WITH_YOU) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->GiveHeroMorality(pGlobalScriptDef->LyingToManAboutWomanNotLikingHer);
                            }
                            else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                Game->GiveHeroMorality(pGlobalScriptDef->TellingTheTruth);
                            }
                            Game->ClearThingHasInformation(Me);
                        }
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!GotReward) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ParentClass.MansLoverState==ML_KILLED) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->GiveHeroObject(pGlobalScriptDef->IngredientObject,-1,false);
                            Game->RemoveItemFromContainer(Me,pGlobalScriptDef->IngredientObject);
                            GotReward=true;
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        if (HelpedGuyOut) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        else if (ParentClass.MansLoverState==ML_KILLED) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                        }
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_MAN_IN_LOVE_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (GotReward && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing ExitPoint=Game->GetNearestWithDefName(Me,"REGION_EXIT_POINT");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsOver(Me,ExitPoint,2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToThing(ExitPoint,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.5f,true,true);
            }
        }
    }
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/mil_a3.cpp)
