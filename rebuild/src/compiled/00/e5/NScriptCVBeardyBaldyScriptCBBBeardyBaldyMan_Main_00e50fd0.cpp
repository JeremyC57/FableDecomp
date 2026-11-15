// NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main() -- Beardy Baldy in Bowerstone South: start the parent's WatchForAttack thread, then each frame scold the hero when attacked or say a random line when the speech timer runs out, and walk the hairdo / beard / moustache phases when talked to; when the quest is complete give the experience, run to the exit marker and fade out
// Grade: RELOCATION_MATCH (9885v9885) -- wfE3 #3. Main loop `for (; !false; Game->NewScriptFrame()) { T-return; ... }` (the CV_SwordInTheStone crowd twin 0x00ED5F50 shape): every ctor / dtor call-or-expand decision is retail's with no flag; one scoped inline_depth(0) region round the row-local CSpawnedFunc1 ctor specialization (Inline-budget line below). The last residue (retail's second entry zero store 0x00E50FE8 `mov [esp+34h],ebp` into the flags-home word, which moved the coalesced temp slot +0x34 -> +0x48 and three alignment fillers) is closed by the dead release asserts in BB_START_SCRIPTING / BB_SPEAK (Inferred line 3).
// Inferred: the debug build's per-use check that the scripted-thing handle drives this entity, compiled out (FABLE_RETAIL_ASSERT, fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM): `sehMe.IsValid() && sehMe.GetScriptThing() == Me` at the end of BB_START_SCRIPTING (6 uses) and before each BB_SPEAK (24 uses; every one follows a BB_START_SCRIPTING with no Release between). Its wording and places are INFERRED (no FableWin text exists here: FableWin.exe is not on this machine, ghidra_out holds no decomp of this function); what the retail bytes EVIDENCE is the count window: 30..35 conditional temporaries (one GetScriptThing result per check, 30 here) after the AddSpawnedFunction statement. VC7.1 numbers a conditional-temporary flag bit for every temporary created right of `&&` / `||` even inside the dead `if (0)` arm of the release assert, opens a second flags word once 32 bits are numbered, and c2 deletes the dead arm leaving only that word's entry `$T = 0` = retail's second entry store 0x00E50FE8 `mov [esp+34h],ebp`. Retail's first word is the ebx flags of the AddSpawnedFunction new-expression (bits 1,2,4, home store 0x00E50FDF from ebx), so the dead bits come after that statement: measured (row-local dead-test form) 29 DIFFER, 30 / 31 / 32 / 35 RELOCATION_MATCH after it; before it the two entry stores swap; the same check in BB_SPEAK only (24) or BB_START_SCRIPTING only (6): DIFFER 9869v9885. Asserts without a conditional temporary number no bits (40 x FABLE_RETAIL_ASSERT(x.IsValid()) leaves DIFFER 9869v9885). Under FABLE_PORTABLE / other compilers the asserts are ((void)0); they cannot change behaviour.
// Leaf: ?Main@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ
// Inline-budget: scoped `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the explicit specialization of the CSpawnedFunc1<CV_BeardyBaldyScript,CScriptThing> ctor (same symbol and body as the primary; VC7.1 instantiates class-template member bodies at the end of the TU, so a pragma round the template itself is ignored) -- keeps the by-value CScriptThing parameter's dtor out of line inside the expanded functor ctor as retail does (copy ctor 0x004ABE90 at 0x00E51090, ~CScriptThing call 0x004AA840 at 0x00E51099); without the region, or without the specialization: DIFFER 9854v9885. Same idiom in landed CUndeadFellow::Main 0x00EF87D0 (rebuild/src/compiled/00/ef/...CUndeadFellow_Main_00ef87d0.cpp). Also FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (the header's forced pause-entities ctor, expanded at every movie block as in the 14 Codex pass 13 NScript TUs): without it DIFFER 9803v9885. CTimer::GetTime is plain inline (forcing it: identical leaf).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E50FD0, 9885 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the CBB_BeardyBaldyMan entity vtable 0x012E5AEC stored by Alloc 0x00E507E0; Ego_r.pdb and FableWin.pdb ?Main@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXXZ. Locals: Anniversary PDB names (regionName, sehMe, randomSpeech, speech_movie, Exit).
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
#define FABLE_RETAIL_ASSERT_DEAD_ARM
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
#include "fable_assert.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
// SPEECH: the generated "say a line and wait for it" block (T-check at its end, after the WAIT loop).
// Inline-budget (see header): the functor ctor specialization under inline_depth(0), as in landed CUndeadFellow::Main 0x00EF87D0.
#pragma inline_depth(0)
template<> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CV_BeardyBaldyScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_BeardyBaldyScript* pclass, void (NScript::CV_BeardyBaldyScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
#pragma inline_depth()
// BB_SPEAK and BB_START_SCRIPTING carry the Inferred handle check (header line 3).
#define BB_SPEAK(text, group) \
    FABLE_RETAIL_ASSERT(sehMe.IsValid() && sehMe.GetScriptThing() == Me); \
    if (Game->GetHealth(sehMe.GetScriptThing()) > 0.0f) { \
        sehMe.Speak(Game->GetHero(), text, group, false, true, false); \
        while (sehMe.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
        } \
        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
    }
#define BB_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define BB_START_SCRIPTING \
    if (sehMe.IsValid()) sehMe.Release(); \
    while (!Game->StartScriptingEntity(Me, sehMe, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        BB_T; \
    } \
    BB_T; \
    FABLE_RETAIL_ASSERT(sehMe.IsValid() && sehMe.GetScriptThing() == Me)

void NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    CCharString regionName;
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_BeardyBaldyScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForAttack"),0,&ParentClass,&CV_BeardyBaldyScript::WatchForAttack,Me),CCharString(""));
    for (; !false; Game->NewScriptFrame()) {
        BB_T;
        if (ParentClass.AttackedByHero) {
            BB_T;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            BB_START_SCRIPTING;
            BB_SPEAK("TEXT_QST_014_ATTACK", GROUP_SELECT_RANDOM_NO_REPEAT)
            if (sehMe.IsValid()) sehMe.Release();
            ParentClass.AttackedByHero = false;
            ParentClass.ResetRandomSpeechTime();
        } else if (ParentClass.RandomSpeechTimer.GetTime() == 0) {
            BB_T;
            CCharString randomSpeech = GetRandomSpeech();
            Game->NewScriptFrame();
            BB_T;
            Game->NewScriptFrame();
            BB_T;
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            Game->NewScriptFrame();
            BB_T;
            Game->NewScriptFrame();
            BB_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, randomSpeech, false, Me, Game->GetHero());
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
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                BB_START_SCRIPTING;
                if (ParentClass.Phase1RequirementsComplete) {
                    BB_T;
                    BB_SPEAK("TEXT_QST_014_PHASE1_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
                } else {
                    BB_T;
                    if (!ParentClass.IntroComplete) {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE1_INTRO1", GROUP_SELECT_NONE)
                        ParentClass.IntroComplete = true;
                    } else {
                        BB_T;
                        BB_SPEAK("TEXT_QST_014_PHASE1_INTRO_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
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
                if (sehMe.IsValid()) sehMe.Release();
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
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
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
                if (sehMe.IsValid()) sehMe.Release();
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
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
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
                if (sehMe.IsValid()) sehMe.Release();
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
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
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
                if (sehMe.IsValid()) sehMe.Release();
            }
            if (ParentClass.QuestComplete) {
                BB_T;
                Game->GiveHeroExperience(pGlobalScriptDef->BeardyBaldyExperience);
                Game->ClearThingHasInformation(Me);
                BB_START_SCRIPTING;
                CScriptThing Exit = Game->GetNearestWithScriptName(Me, "M_BB_ExitPoint");
                if (Exit.IsAlive()) {
                    BB_T;
                    if (sehMe.IsValid()) {
                        BB_T;
                        sehMe.MoveToPosition(Exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me, Exit, 2.0f) && !Game->MsgOnRegionLoaded(regionName)) {
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