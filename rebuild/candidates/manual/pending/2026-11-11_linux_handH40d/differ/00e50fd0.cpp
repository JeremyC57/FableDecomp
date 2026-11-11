// NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main() -- Beardy Baldy in Bowerstone South: start the parent's WatchForAttack thread, then each frame scold the hero when attacked or say a random line when the speech timer runs out, and walk the hairdo / beard / moustache phases when talked to; when the quest is complete give the experience, run to the exit marker and fade out
// Grade: DIFFER(10022v9885) -- 224 masked lines (281 before): the predicate lever of this chunk is applied (inline_depth(0) round the row-local predicate ctor / dtor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE) with FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY, plus the explicit CSpawnedFunc1<CV_BeardyBaldyScript,CScriptThing> ctor specialization under inline_depth(0) (Par1 copy 0x004ABE90 called inside the expanded spawn ctor); left: the movie ctor / dtor split, 137 bytes of expansions retail calls
// Next idea (chunk H40d, no grade spent; 6 inline-trace probes): the split is a frequency question, not a device one. Retail expands the movie ctor only at the attack scene (0x00E51149) and the movie dtor at that scene's end (0x00E512EB) and at its first two thread exits (pads 0x00E53561 / 0x00E53591), and calls 0x006E7B60 / 0x006E7B80 everywhere else (pads 0x00E535C1, 0x00E535DB ...). This draft's trace (tools/inl.sh; movie ctor c1 20, dtor c1 19): attack ctor f 125 EXP, exits 62.5 / 31.2 / 7.8 EXP, 3.9 CALL, scene end 19.5 EXP, phase-0 ctor 7.0 EXP (score 58), the rest CALL -- two sites wrong (the third exit, score 51, and the phase-0 ctor); the dtor is called below f ~4.6, the ctor below ~5.0, so everything from the attack scene's BB_SPEAK on has to be 0.59x colder (a uniform 0.5 fits every site). The main-loop spelling is right as it is: while (!T) { ...; NSF; }, the guarded do-while, while (true) with the check at the top or at the bottom all move the sites the wrong way or give EXP CALL EXP CALL for the four exits. Look for the missing 0.5 in front of the attack scene's speech (an `&&` whose second operand folds, or a constant test); then regrade: the prologue zero-register / 0x0126008C-in-ebp hunks follow from the two extra expansions
// Leaf: ?Main@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_BeardyBaldyScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
// SPEECH: the generated "say a line and wait for it" block (T-check at its end, after the WAIT loop).
#pragma inline_depth(0)
template<> __forceinline CSpawnedFunc1<NScript::CV_BeardyBaldyScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_BeardyBaldyScript* pclass, void (NScript::CV_BeardyBaldyScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
#pragma inline_depth()
#define BB_SPEAK(text, group) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), text, group, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
        } \
        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
    }
#define BB_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define BB_START_SCRIPTING \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        BB_T; \
    } \
    BB_T

void NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CCharString unusedText;
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_BeardyBaldyScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForAttack"),0,&ParentClass,&CV_BeardyBaldyScript::WatchForAttack,Me),CCharString(""));
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (ParentClass.AttackedByHero) {
            BB_T;
            CScriptGameResourceObjectMovieBase attack_movie;
            Game->StartMovieSequence("", attack_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            BB_START_SCRIPTING;
            BB_SPEAK("TEXT_QST_014_ATTACK", GROUP_SELECT_RANDOM_NO_REPEAT)
            if (seh_me.IsValid()) seh_me.Release();
            ParentClass.AttackedByHero = false;
            ParentClass.ResetRandomSpeechTime();
        } else if (ParentClass.RandomSpeechTimer.GetTime() == 0) {
            BB_T;
            CCharString speech = GetRandomSpeech();
            Game->NewScriptFrame();
            BB_T;
            Game->NewScriptFrame();
            BB_T;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            Game->NewScriptFrame();
            BB_T;
            Game->NewScriptFrame();
            BB_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, speech, false, Me, Game->GetHero());
            ParentClass.ResetRandomSpeechTime();
        }
        if (ParentClass.QuestPhase == 0) {
            BB_T;
            if (ParentClass.InitialisePhase) {
                BB_T;
                ParentClass.SetWanderPointAndDistance(Me, Me.GetPos());
                ParentClass.InitialisePhase = false;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                BB_T;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                BB_START_SCRIPTING;
                if (ParentClass.Phase1RequirementsComplete) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_PHASE1_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
                } else {
                    BB_T;
                    if (ParentClass.IntroComplete) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE1_INTRO_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
                    } else {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE1_INTRO1", GROUP_SELECT_NONE)
                        ParentClass.IntroComplete = true;
                    }
                    Game->GiveHeroYesNoQuestion("TEXT_QST_014_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                    long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer < 0) {
                        Game->NewScriptFrame();
                        BB_T;
                        answer = Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    BB_T;
                    ParentClass.ResetRandomSpeechTime();
                    if (answer == 1) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE1_INTRO2", GROUP_SELECT_NONE)
                        if (!Game->IsWearingHairstyle(Game->GetHero(), pGlobalScriptDef->BeardyBaldyHaircutStyle1)) {
                            BB_T;
                            Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyHaircutCard1, -1, false);
                            ParentClass.RequiredHairdo = pGlobalScriptDef->BeardyBaldyHaircutStyle1;
                        } else {
                            BB_T;
                            Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyHaircutCard2, -1, false);
                            ParentClass.RequiredHairdo = pGlobalScriptDef->BeardyBaldyHaircutStyle2;
                        }
                        BB_SPEAK("TEXT_QST_014_PHASE1_INTRO3", GROUP_SELECT_NONE)
                        ParentClass.Phase1RequirementsComplete = true;
                    } else {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE1_DENIED", GROUP_SELECT_RANDOM_NO_REPEAT)
                    }
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
            if (ParentClass.Phase1RequirementsComplete) {
                BB_T;
                ParentClass.QuestPhase = 1;
                ParentClass.InitialisePhase = true;
            }
        } else if (ParentClass.QuestPhase == 1) {
            BB_T;
            if (ParentClass.InitialisePhase) {
                BB_T;
                ParentClass.SetWanderPointAndDistance(Me, Me.GetPos());
                ParentClass.InitialisePhase = false;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                BB_T;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                BB_START_SCRIPTING;
                if (ParentClass.Phase2RequirementsComplete) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_PHASE2_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
                } else {
                    BB_T;
                    if (Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredHairdo)) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE2_INTRO1", GROUP_SELECT_NONE)
                        if (!Game->IsWearingHairstyle(Game->GetHero(), pGlobalScriptDef->BeardyBaldyBeardStyle1)) {
                            BB_T;
                            Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyBeardCard1, -1, false);
                            ParentClass.RequiredBeard = pGlobalScriptDef->BeardyBaldyBeardStyle1;
                        } else {
                            BB_T;
                            Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyBeardCard2, -1, false);
                            ParentClass.RequiredBeard = pGlobalScriptDef->BeardyBaldyBeardStyle2;
                        }
                        BB_SPEAK("TEXT_QST_014_PHASE2_INTRO2", GROUP_SELECT_NONE)
                        ParentClass.Phase2RequirementsComplete = true;
                    } else {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE2_WRONG_HAIR", GROUP_SELECT_RANDOM_NO_REPEAT)
                    }
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
            if (ParentClass.Phase2RequirementsComplete) {
                BB_T;
                ParentClass.QuestPhase = 2;
                ParentClass.InitialisePhase = true;
            }
        } else if (ParentClass.QuestPhase == 2) {
            BB_T;
            if (ParentClass.InitialisePhase) {
                BB_T;
                ParentClass.SetWanderPointAndDistance(Me, Me.GetPos());
                ParentClass.InitialisePhase = false;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                BB_T;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                BB_START_SCRIPTING;
                if (ParentClass.Phase3RequirementsComplete) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_PHASE3_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
                } else {
                    BB_T;
                    if (!ParentClass.IsHeroWearingAnyBeard()) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE3_NO_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT)
                    } else if (Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredHairdo) && Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredBeard)) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE3_INTRO1", GROUP_SELECT_NONE)
                        if (ParentClass.IsHeroWearingAnyTash()) {
                            BB_T;
                            BB_SPEAK("TEXT_QST_014_PHASE3_INTRO2", GROUP_SELECT_NONE)
                        } else {
                            BB_T;
                            BB_SPEAK("TEXT_QST_014_PHASE3_INTRO3", GROUP_SELECT_NONE)
                        }
                        if (!Game->IsWearingHairstyle(Game->GetHero(), pGlobalScriptDef->BeardyBaldyTashStyle1)) {
                            BB_T;
                            Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyTashCard1, -1, false);
                            ParentClass.RequiredTash = pGlobalScriptDef->BeardyBaldyTashStyle1;
                        } else {
                            BB_T;
                            Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyTashCard2, -1, false);
                            ParentClass.RequiredTash = pGlobalScriptDef->BeardyBaldyTashStyle2;
                        }
                        BB_SPEAK("TEXT_QST_014_PHASE3_INTRO4", GROUP_SELECT_NONE)
                        ParentClass.Phase3RequirementsComplete = true;
                    } else if (Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredHairdo) && !Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredBeard)) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE3_WRONG_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT)
                    } else if (!Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredHairdo) && Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredBeard)) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE3_WRONG_HAIR", GROUP_SELECT_RANDOM_NO_REPEAT)
                    } else if (!Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredHairdo) && !Game->IsWearingHairstyle(Game->GetHero(), ParentClass.RequiredBeard)) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE3_WRONG_HAIR_AND_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT)
                    }
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
            if (ParentClass.Phase3RequirementsComplete) {
                BB_T;
                ParentClass.QuestPhase = 3;
                ParentClass.InitialisePhase = true;
            }
        } else if (ParentClass.QuestPhase == 3) {
            BB_T;
            if (ParentClass.InitialisePhase) {
                BB_T;
                ParentClass.SetWanderPointAndDistance(Me, Me.GetPos());
                ParentClass.InitialisePhase = false;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                BB_T;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                BB_START_SCRIPTING;
                if (!ParentClass.AllHairChanged) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_END_NO_TASH", GROUP_SELECT_RANDOM_NO_REPEAT)
                } else if (ParentClass.IsHeroWearingAnyBeard() && ParentClass.IsHeroWearingAnyTash() && ParentClass.IsHeroWearingAnyOddHairdo()) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_END_FINALE1", GROUP_SELECT_NONE)
                    Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyRewardObjectFullComplete, -1, false);
                    ParentClass.QuestComplete = true;
                } else if (ParentClass.IncorrectHairComboCount > 3 && ParentClass.AllHairChanged) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_END_FINALE2", GROUP_SELECT_NONE)
                    Game->GiveHeroObject(pGlobalScriptDef->BeardyBaldyRewardObjectHalfComplete, -1, false);
                    ParentClass.QuestComplete = true;
                } else {
                    BB_T;
                    ++ParentClass.IncorrectHairComboCount;
                    BB_SPEAK("TEXT_QST_014_END_WRONG", GROUP_SELECT_RANDOM_NO_REPEAT)
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
            if (ParentClass.QuestComplete) {
                BB_T;
                Game->GiveHeroExperience(pGlobalScriptDef->BeardyBaldyExperience);
                Game->ClearThingHasInformation(Me);
                BB_START_SCRIPTING;
                CScriptThing exitPoint = Game->GetNearestWithScriptName(Me, "M_BB_ExitPoint");
                if (exitPoint.IsAlive()) {
                    BB_T;
                    if (seh_me.IsValid()) {
                        BB_T;
                        seh_me.MoveToPosition(exitPoint.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me, exitPoint, 2.0f) && !Game->MsgOnRegionLoaded(unusedText)) {
                        Game->NewScriptFrame();
                        BB_T;
                    }
                    BB_T;
                    FadeOutAndKillEntity(Me, 1.0f, false, true);
                    ParentClass.BeardyBaldyLeft = true;
                }
            }
        }
    }
}
