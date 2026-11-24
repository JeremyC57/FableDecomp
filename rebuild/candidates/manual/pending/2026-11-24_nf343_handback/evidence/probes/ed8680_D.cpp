// NScript::CV_TalentlessBardScript::CTB_Bard::Main() -- the Bowerstone bard: with the alive predicate, spawn the parent's attack and wander-centre watchers on him, then every frame: when the hero hits him (not with heal life), face the hero and say the on-hit line; when the hero talks to him, face the hero, play the intro or a random return line, ask whether to sing; yes and enough gold takes the fee and sings the verses with mandolin chords, yes without the fee says a random no-cash line, no says the rejected line
// Grade: RELOCATION_MATCH (B24A10, stock flags)
// Inline-budget: #pragma inline_depth(0) round the row-local CIsThingAlive ctor (+ FABLE_RETAIL_CPREDICATEBASE_NOVTABLE) and round the
//   CSpawnedFunc1<CV_TalentlessBardScript,CScriptThing> ctor specialization -- retail CALLS the nested CScriptThing copy 0x004ABE90 inside
//   the expanded CIsThingAlive ctor (0x00ED86A4) and both CSpawnedFunc1 Par1 copies (0x00ED877F / 0x00ED886F) while the by-value Me
//   argument copies expand (0x00ED870B ...): the chunk H40 predicate lever (landed 0x00D82640 / 0x00D8BCD0) and the CSwordCrowd::Main
//   0x00ED5F50 thread-ctor specialization; without either the row differs (3668 / 3684 bytes).
// Shape (debug build): Ego_d.pdb locals sehMe, randomSpeech, _Fail_Val (TRetVoid), speech_movie / PauseMyEntitiesPlease, YNQAnswer,
//   return_tag, int verse; FableWin 0x03AB2C80 opens the main loop and the verse loop with `xor reg,reg ; je ; jmp` (the TLC
//   `#define for if (0) {} else for` idiom, GOTCHAS 442), the main loop being `for (;; Game->NewScriptFrame())` with the terminate
//   check first in the body, and every terminate exit calls TRetVoid::Return. That loop shape gives retail's inline split at stock
//   threshold (the earlier while / do-while(0) forms needed /d2inlT51).
// Leaf: ?Main@CTB_Bard@CV_TalentlessBardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED8680, 3652 bytes; entity vtable 0x012F3498 slot 1.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CQ_SunnyvaleMasterData.h"
#define FABLE_RETAIL_CTB_BARD_DECLS
#include "engine/NScript_CV_TalentlessBardScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
// (NF343 probe D: row-local decl-only ~CCountedPointer<MovieBase> specialization removed)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CIsThingAlive.h"
#include "engine/EHeroAbility.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_def_global.h"
#include "engine/EScriptAIPriority.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/TRetVoid.h"
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CV_TalentlessBardScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_TalentlessBardScript* pclass, void (NScript::CV_TalentlessBardScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CV_TalentlessBardScript::CTB_Bard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CCharString randomSpeech;
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_TalentlessBardScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForAttack"), 0, ParentClass, &CV_TalentlessBardScript::WatchForAttack, Me), CCharString(""));
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_TalentlessBardScript, CScriptThing>(CCharString("ParentClass.") + CCharString("WatchForResetWanderCentre"), 0, ParentClass, &CV_TalentlessBardScript::WatchForResetWanderCentre, Me), CCharString(""));
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
            && Game->GetHealth(Me) > 0.0f) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                sehMe.Speak(Game->GetHero(), "TEXT_QST_163_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (sehMe.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (sehMe.IsValid())
                sehMe.Release();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            if (sehMe.IsValid())
                sehMe.Release();
            while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass->BardIntroComplete) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                    sehMe.Speak(Game->GetHero(), "TEXT_QST_163_INTRO", GROUP_SELECT_NONE, false, true, false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                ParentClass->BardIntroComplete = true;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CCharString return_tag;
                return_tag.Sprintf("TEXT_QST_163_RETURN%d", rand() % 4 + 1);
                if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                    sehMe.Speak(Game->GetHero(), return_tag, GROUP_SELECT_NONE, false, true, false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            Game->GiveHeroYesNoQuestion("TEXT_QST_163_QUESTION_SONG", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long YNQAnswer;
            while ((YNQAnswer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (YNQAnswer == 1) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHeroGold() > pGlobalScriptDef->BardSingingFee) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    Game->GiveHeroGold(-pGlobalScriptDef->BardSingingFee);
                    if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                        sehMe.Speak(Game->GetHero(), "TEXT_QST_163_START", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (sehMe.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (false) {} else for (int verse = 0; verse < pGlobalScriptDef->BardNumVersesPerVisit; verse++) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        Game->PlaySoundOnThing(Me, "SND_MANDOLINCHORD_01");
                        Game->Pause(2.0f);
                        randomSpeech = GetRandomSpeech();
                        if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                            sehMe.Speak(Game->GetHero(), randomSpeech, GROUP_SELECT_NONE, false, true, false);
                            while (sehMe.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        Game->Pause(1.0f);
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CCharString return_tag;
                    return_tag.Sprintf("TEXT_QST_163_NO_CASH%d", rand() % 2 + 1);
                    if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                        sehMe.Speak(Game->GetHero(), return_tag, GROUP_SELECT_NONE, false, true, false);
                        while (sehMe.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) {
                    sehMe.Speak(Game->GetHero(), "TEXT_QST_163_REJECTED", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (sehMe.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (sehMe.IsValid())
                sehMe.Release();
        }
    }
}
