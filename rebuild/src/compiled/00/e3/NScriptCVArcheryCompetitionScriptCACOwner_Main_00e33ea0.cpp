// NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main() -- the archery competition owner: start the parent's WatchForAttack thread, stock the two high-score prizes, then each frame say a random line, scold the hero and leave when attacked, run the welcome / rules / entry-fee talk, the countdown, watch the hero during the game (stepped out, magic, shot the owner) and hand out the high-score, match and normal prizes when it ends
// Inline-budget: `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the row-local CIsThingAlive ctor and the CSpawnedFunc1<CV_ArcheryCompetitionScript,CScriptThing> ctor specialization -- retail expands both ctors but CALLS the nested CScriptThing copy 0x004ABE90 inside them (0x00E33EC8 predicate, 0x00E33F8B Par1) while the by-value Me argument copy expands (0x00E33F13..0x00E33F38); the landed idiom of 0x00E2FF00 (CAS_Buddy::Main, CSpawnedFunc1) and 0x00E27E90 (predicate, FABLE_RETAIL_CPREDICATEBASE_NOVTABLE + FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY).
// Leaf: ?Main@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E33EA0, 9228 bytes (last insn `ret` at 0x00E362AB, int3 filler, GetRandomSpeech 0x00E362B0 follows), thiscall, frame 0x1F8, no EH frame.
// Identity: the CAC_Owner entity vtable slot at .rdata 0x012E30FC is the only reference; Ego_r.pdb (0x00E83BB0) and FableWin.pdb (0x039381B0) ?Main@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@EAEXXZ.
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840; SetPredicate 0x00F35B10, ~CIsThingAlive 0x00E241B0 (out of line); IsActiveThreadTerminating 0x00F35B30;
//   operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450, CCharString + 0x0099F570; GetRandomSpeech 0x00E362B0, ResetRandomSpeechTime 0x00E337C0 (landed),
//   DoCountDown 0x00E36750, DoTimesUp 0x00E36590; rand 0x00BFEB16; CDefString -> CCharString through GetString 0x00415D70 (FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING).
// EVIDENCED by retail bytes: EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false) in the random-speech and talk arms (push 0 ; push &Me ; call GetHero ; push eax at 0x00E34299 and 0x00E3472E);
//   each random prize index is computed before the prize vector is addressed: retail reloads pGlobalScriptDef after rand() (0x00E35BD3 size, 0x00E35BE7 rand, 0x00E35BF0 reload) -- the
//   index local `idx` per prize (FITTED spelling of that order: indexing the vector inline makes VC7.1 keep the def pointer in ebp across rand(), 9268 bytes).
// Header overlay: FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN (ResetRandomSpeechTime / WatchForAttack / DoTimesUp / CAC_Owner::GetRandomSpeech declarations, Ego_r + FableWin names)
//   and the repo opt-in FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_DOCOUNTDOWN.
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_DOCOUNTDOWN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ArcheryCompetitionScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CIsThingAlive.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern "C" int __cdecl rand();
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CV_ArcheryCompetitionScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_ArcheryCompetitionScript* pclass, void (NScript::CV_ArcheryCompetitionScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
#pragma inline_depth()
#define AC_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define AC_SPEAK(text, group) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), text, group, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            AC_T; \
        } \
        AC_T; \
    }
#define AC_START_SCRIPTING \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        AC_T; \
    } \
    AC_T
#define AC_WAIT2 Game->NewScriptFrame(); AC_T; Game->NewScriptFrame(); AC_T

void NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    AC_T;
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_ArcheryCompetitionScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForAttack"),0,&ParentClass,&CV_ArcheryCompetitionScript::WatchForAttack,Me),CCharString(""));
    CScriptGameResourceObjectScriptedThingBase seh_me;
    AC_START_SCRIPTING;
    if (Game->IsQuestActive("Q_HeroSoulsThunder")) {
        AC_T;
        Game->RemoveThing(Me, false, false);
    }
    CScriptThing highScorePost = Game->GetThingWithScriptName("HighscorePost");
    CScriptThing playerRing = Game->GetThingWithScriptName("AC_PlayerRing");
    float damage = 0.0f;
    if (!ParentClass.GivenReallyCoolPrize && !ParentClass.Pickpocketed) {
        AC_T;
        Game->AddItemToContainer(Me, pGlobalScriptDef->ArcheryReallyHighScorePrize);
    }
    if (!ParentClass.GivenFirstHighScorePrize && !ParentClass.Pickpocketed) {
        AC_T;
        Game->AddItemToContainer(Me, pGlobalScriptDef->ArcheryRewardObject);
    }
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_VIGNETTE");
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (ParentClass.RandomSpeechTimer.GetTime() == 0 && ParentClass.ContestState == CONTEST_WAITING && !ParentClass.AttackedByHero) {
            AC_T;
            CCharString speech = GetRandomSpeech();
            AC_WAIT2;
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
            AC_WAIT2;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, speech, false, Me, Game->GetHero());
            ParentClass.ResetRandomSpeechTime();
        }
        if (ParentClass.AttackedByHero && Game->GetHealth(Me) > 0.0001f) {
            AC_T;
            if (seh_me.IsValid()) seh_me.Release();
            AC_START_SCRIPTING;
            {
                CScriptGameResourceObjectMovieBase attack_movie;
                Game->StartMovieSequence("", attack_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                AC_SPEAK("TEXT_QST_004_OWNER_ATTACK", GROUP_SELECT_NONE)
            }
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                AC_T;
            }
            AC_T;
            CScriptThing exitPoint;
            exitPoint = Game->GetThingWithScriptName("M_AC_OwnerExitPoint");
            if (exitPoint.IsAlive()) {
                AC_T;
                if (seh_me.IsValid()) {
                    AC_T;
                    seh_me.MoveToPosition(exitPoint.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, false);
                }
                while (!IsDistanceBetweenThingsUnder(Me, exitPoint, 2.0f)) {
                    Game->NewScriptFrame();
                    AC_T;
                }
                AC_T;
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        if (ParentClass.ContestState == CONTEST_WAITING) {
            AC_T;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                AC_T;
                AC_WAIT2;
                Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Me, false);
                AC_WAIT2;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (!ParentClass.WelcomeComplete) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_INTRO", GROUP_SELECT_NONE)
                    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ARCHERY_COMPETITION", Game->GetActiveQuestName(), true);
                    ParentClass.WelcomeComplete = true;
                } else {
                    AC_T;
                    if (ParentClass.GiveAttackWarning) {
                        AC_T;
                        AC_SPEAK("TEXT_QST_004_BEHAVE_YOURSELF", GROUP_SELECT_RANDOM)
                        ParentClass.GiveAttackWarning = false;
                    } else {
                        AC_T;
                        AC_SPEAK("TEXT_QST_004_WELCOME_BACK", GROUP_SELECT_RANDOM)
                    }
                }
                ParentClass.ResetRandomSpeechTime();
                Game->GiveHeroYesNoQuestion("TEXT_QST_004_RULES_Q", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) {
                    Game->NewScriptFrame();
                    AC_T;
                    answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                AC_T;
                ParentClass.ResetRandomSpeechTime();
                if (answer == 1) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_RULES", GROUP_SELECT_NONE)
                }
                Game->NewScriptFrame();
                AC_T;
                Game->GiveHeroYesNoQuestion("TEXT_QST_004_START_Q", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) {
                    Game->NewScriptFrame();
                    AC_T;
                    answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                AC_T;
                ParentClass.ResetRandomSpeechTime();
                if (answer == 1) {
                    AC_T;
                    if (Game->GetHeroGold() >= -pGlobalScriptDef->ArcheryCompetitionCost) {
                        AC_T;
                        Game->GiveHeroGold(pGlobalScriptDef->ArcheryCompetitionCost);
                        ParentClass.ContestState = CONTEST_COUNTDOWN;
                    } else {
                        AC_T;
                        AC_SPEAK("TEXT_QST_004_CASH_POOR", GROUP_SELECT_RANDOM_NO_REPEAT)
                    }
                }
            }
        } else if (ParentClass.ContestState == CONTEST_COUNTDOWN) {
            AC_T;
            Game->FadeScreenOut(1.0f, 0.5f, CRGBColour(0, 0, 0, 255));
            Game->Pause(1.0f);
            Game->EntityTeleportToThing(Game->GetHero(), playerRing, false);
            Game->EntityUnsheatheRangedWeapon(Game->GetHero(), false);
            Game->SetHeroWillAsUsable(false);
            AC_WAIT2;
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(), Game->GetThingWithScriptName("M_AC_HeroFaceMe"), false);
            AC_WAIT2;
            {
                CScriptThing teleport = Game->GetThingWithScriptName("M_AC_OwnerTeleport");
                if (teleport.IsAlive()) {
                    AC_T;
                    Game->EntityTeleportToThing(Me, teleport, false);
                    AC_WAIT2;
                    Game->EntitySetFacingAngleTowardsThing(Me, playerRing, false);
                    AC_WAIT2;
                }
                Game->FadeScreenIn();
                Game->SetTeleportingAsActive(false);
                Game->Pause(0.5f);
                ParentClass.DoCountDown(3);
            }
            ParentClass.GameTimeLeft = pGlobalScriptDef->ArcheryGameTimeLimit;
            Game->DisplayQuestInfo(true);
            ParentClass.ContestState = CONTEST_STARTED;
        } else if (ParentClass.ContestState == CONTEST_STARTED) {
            AC_T;
            if (IsDistanceBetweenThingsOver(Game->GetHero(), playerRing, pGlobalScriptDef->ArcheryMaxOutsideRing)) {
                AC_T;
                ParentClass.ContestState = CONTEST_FINISHED;
                ParentClass.SteppedOut = true;
                ParentClass.GameTimeLeft = 0;
            } else if (Game->GetHero().MsgPerformedAnySpecialAbility() && !ParentClass.UsedMagic) {
                AC_T;
                ParentClass.ContestState = CONTEST_FINISHED;
                ParentClass.UsedMagic = true;
                ParentClass.GameTimeLeft = 0;
            } else if (Me.MsgIsHitByWithProjectileWeapon("SCRIPT_NAME_HERO", damage) && Game->GetHealth(Me) > 0.0001f) {
                AC_T;
                ParentClass.ContestState = CONTEST_FINISHED;
                ParentClass.ShotOwner = true;
                ParentClass.GameTimeLeft = 0;
                ParentClass.GiveAttackWarning = true;
            }
        } else if (ParentClass.ContestState == CONTEST_FINISHED) {
            AC_T;
            Game->EntitySheatheWeapons(Game->GetHero(), false);
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass.SteppedOut && !ParentClass.UsedMagic && !ParentClass.ShotOwner) {
                AC_T;
                ParentClass.DoTimesUp(Game->GetThingWithScriptName("AC_Owner"), seh_me);
                AC_START_SCRIPTING;
            }
            if (ParentClass.SteppedOut) {
                AC_T;
                AC_SPEAK("TEXT_QST_004_DISQUALIFIED_STEPPED_OUT", GROUP_SELECT_RANDOM)
            } else if (ParentClass.UsedMagic) {
                AC_T;
                AC_SPEAK("TEXT_QST_004_DISQUALIFIED_USED_MAGIC", GROUP_SELECT_RANDOM)
            } else if (ParentClass.ShotOwner) {
                AC_T;
                AC_SPEAK("TEXT_QST_004_DISQUALIFIED_SHOT_OWNER", GROUP_SELECT_RANDOM)
            } else {
                long score = ParentClass.Score;
                long highScore = PMasterData->ArcheryHighScore;
                if (score > highScore) {
                    AC_T;
                    PMasterData->ArcheryHighScore = ParentClass.Score;
                    ParentClass.UpdateHighScore(ParentClass.Score);
                    bool givePrize = !Game->IsObjectInThingsPossession(pGlobalScriptDef->ArcheryRewardObject, Game->GetHero()) && Game->IsObjectInThingsPossession(pGlobalScriptDef->ArcheryRewardObject, Me) && !ParentClass.GivenFirstHighScorePrize;
                    if (givePrize) {
                        AC_T;
                        AC_SPEAK("TEXT_QST_004_SCORE_HIGH_NEW", GROUP_SELECT_RANDOM)
                        Game->RemoveItemFromContainer(Me, pGlobalScriptDef->ArcheryRewardObject);
                        Game->GiveHeroObject(pGlobalScriptDef->ArcheryRewardObject, -1, false);
                        Game->GiveHeroExperience(pGlobalScriptDef->ArcheryExperience);
                        ParentClass.GivenFirstHighScorePrize = true;
                    } else {
                        givePrize = !Game->IsObjectInThingsPossession(pGlobalScriptDef->ArcheryReallyHighScorePrize, Game->GetHero()) && Game->IsObjectInThingsPossession(pGlobalScriptDef->ArcheryReallyHighScorePrize, Me) && !ParentClass.GivenReallyCoolPrize;
                        if (givePrize) {
                            AC_T;
                            AC_SPEAK("TEXT_QST_004_SCORE_HIGH_NEW_REALLY", GROUP_SELECT_NONE)
                            Game->RemoveItemFromContainer(Me, pGlobalScriptDef->ArcheryReallyHighScorePrize);
                            Game->GiveHeroObject(pGlobalScriptDef->ArcheryReallyHighScorePrize, -1, false);
                            ParentClass.GivenReallyCoolPrize = true;
                        } else {
                            AC_T;
                            AC_SPEAK("TEXT_QST_004_SCORE_HIGH_NEW", GROUP_SELECT_RANDOM)
                            {
                                unsigned long idx = rand() % pGlobalScriptDef->ArcheryNewHighScorePrizes.size();
                                Game->GiveHeroObject(pGlobalScriptDef->ArcheryNewHighScorePrizes[idx], -1, false);
                            }
                        }
                    }
                    Game->GiveHeroRenownPoints(pGlobalScriptDef->ArcheryRenown);
                } else if (score == highScore) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_HIGH_MATCH", GROUP_SELECT_RANDOM)
                    {
                        unsigned long idx = rand() % pGlobalScriptDef->ArcheryMatchHighScorePrizes.size();
                        Game->GiveHeroObject(pGlobalScriptDef->ArcheryMatchHighScorePrizes[idx], -1, false);
                    }
                } else if ((float)score >= (float)highScore * pGlobalScriptDef->ArcheryScoreForPrizeHigh) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW_PRIZE", GROUP_SELECT_RANDOM)
                    {
                        unsigned long idx = rand() % pGlobalScriptDef->ArcheryNormalPrizesHigh.size();
                        Game->GiveHeroObject(pGlobalScriptDef->ArcheryNormalPrizesHigh[idx], -1, false);
                    }
                } else if ((float)score >= (float)highScore * pGlobalScriptDef->ArcheryScoreForPrizeMedium) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW_PRIZE", GROUP_SELECT_RANDOM)
                    {
                        unsigned long idx = rand() % pGlobalScriptDef->ArcheryNormalPrizesMedium.size();
                        Game->GiveHeroObject(pGlobalScriptDef->ArcheryNormalPrizesMedium[idx], -1, false);
                    }
                } else if ((float)score >= (float)highScore * pGlobalScriptDef->ArcheryScoreForPrizeLow) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW_PRIZE", GROUP_SELECT_RANDOM)
                    {
                        unsigned long idx = rand() % pGlobalScriptDef->ArcheryNormalPrizesLow.size();
                        Game->GiveHeroObject(pGlobalScriptDef->ArcheryNormalPrizesLow[idx], -1, false);
                    }
                } else {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW", GROUP_SELECT_RANDOM)
                }
            }
            Game->SetTeleportingAsActive(true);
            Game->SetHeroWillAsUsable(true);
            ParentClass.ContestState = CONTEST_WAITING;
        }
    }
}