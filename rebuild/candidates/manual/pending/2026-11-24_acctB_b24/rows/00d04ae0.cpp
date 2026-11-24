// NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main() -- the first assassin in the bandit camp: alive filter on Me, scripted control, then until the assassins are attacked or their cutscene is triggered, mark them under attack when the hero hits him (not with heal life); when the hero talks to him: once the third gate is open say he is not needed, else the first time introduce himself and ask whether to hire him (yes with enough gold: hire line, cutscene flag, take the fee, grant experience; poor / no lines), later times ask the repeat question; finally (attacked or hired) clear his information and release control, idle forever
// Grade: RELOCATION_MATCH (B24A11, flags /d2inlS15)
// Flags: /d2inlS15
// Inline-budget: /d2inlS15 -- retail EXPANDS the implicit ~CScriptGameResourceObjectMovieBase (call 0x006E7AB0 ; GROBase vtable store ;
//   call 0x0099A430) at every speech-block exit, the terminate exits included; stock VC7.1 always-inlines only c1 <= 13 and calls our
//   ~MovieBase (sz 25 / c1 19, $S/w22/b24a/SHARED_FORM.md) at the cold exits: stock DIFFER(3884v3623), /d2inlS10..14 the same, S15 (always
//   iff c1 <= 19) matches. The family flag of this retail object (sibling CAssassinMarker::Main 0x00D11930 staged with it); no MovieBase /
//   GROBase header text with retail's bytes costs less (B24A4); FableWin 0x03691C40 shows plain while loops (no for-scope openers), so the
//   0x00ED8680 loop respelling does not apply. A fitted device until object mode settles this object's flag set (EXTRA_RULES 2026-11-24).
// Inline-budget: #pragma inline_depth(0) round the row-local CIsThingAlive ctor (+ FABLE_RETAIL_CPREDICATEBASE_NOVTABLE): retail CALLS the
//   nested CScriptThing copy 0x004ABE90 inside the expanded predicate ctor (H40 lever; without: 3655 / 24 hunks, without NOVTABLE 7 hunks);
//   the empty ~CIsThingAlive also under inline_depth(0) (landed 0x00D973D0 form) so the emitted ??_GCIsThingAlive is the family copy.
// Shape (debug build): Ego_d.pdb locals seh_me, TalkedTo (bool), _Fail_Val (TRetVoid); three blocks {speech_movie, PauseMyEntitiesPlease}
//   (+ long answer in the two question blocks); FableWin 0x03691C40: every terminate exit calls TRetVoid::Return, TalkedTo is set before
//   ParentClass.TalkedToAssassin, and the tail is if (hired || attacked) { clear information; release; idle } else { idle }.
// Leaf: ?Main@CAssassin1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D04AE0, 3623 bytes; entity vtable 0x012C8B9C slot 1.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
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
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    bool TalkedTo = false;
    while (!ParentClass.AssassinsUnderAttack && !ParentClass.AssassinCutsceneTriggered) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            ParentClass.AssassinsUnderAttack = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (ParentClass.Gate3Open) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_NOT_NEEDED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (!TalkedTo) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHeroGold() < pGlobalScriptDef->BAC_AssassinGoldAmount) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            ParentClass.AssassinCutsceneTriggered = true;
                            Game->GiveHeroGold((long)-pGlobalScriptDef->BAC_AssassinGoldAmount);
                            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                    TalkedTo = true;
                    ParentClass.TalkedToAssassin = true;
                } else {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer;
                    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    if (answer == 1) {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHeroGold() < pGlobalScriptDef->BAC_AssassinGoldAmount) {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                        } else {
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES", GROUP_SELECT_NONE, false, true, false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating())
                                        return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            ParentClass.AssassinCutsceneTriggered = true;
                            Game->GiveHeroGold((long)-pGlobalScriptDef->BAC_AssassinGoldAmount);
                            Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_SMALL);
                        }
                    } else {
                        if (IsActiveThreadTerminating())
                            return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating())
                                    return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating())
                                return _Fail_Val.Return();
                        }
                    }
                }
            }
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (ParentClass.AssassinCutsceneTriggered || ParentClass.AssassinsUnderAttack) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->ClearThingHasInformation(Me);
        seh_me.Release();
        while (true) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    } else {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        while (true) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
    }
}
