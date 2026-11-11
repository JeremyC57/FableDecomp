// NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main() -- the archery competition owner: start the parent's WatchForAttack thread, stock the two prizes, then each frame say a random line, scold and leave when attacked, take the entry fee when talked to, run the countdown, watch the hero during the game (stepped out, magic, shot the owner) and hand out the high-score and normal prizes when it ends
// Grade: DIFFER(9303v9228) -- 323 masked lines, the chunk 815 draft unchanged: with the explicit CSpawnedFunc1<CV_ArcheryCompetitionScript,CScriptThing> ctor specialization under inline_depth(0) the row gets worse here (1028 lines, frame and colouring move), with the predicate ctor region as well 1018
// Next idea: apply the two levers one at a time and realign the frame after each (+4 bytes, 0x1FC v 0x1F8), as 0x00E2FF00 did for its spawn sites
// Leaf: ?Main@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@EAEXXZ
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CACOWNER_MAIN
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
inline CScriptThing::CScriptThing(const CScriptThing& o) : CBaseClass(o), PImp(o.PImp) { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
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
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
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
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
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
                            Game->GiveHeroObject(pGlobalScriptDef->ArcheryNewHighScorePrizes[rand() % pGlobalScriptDef->ArcheryNewHighScorePrizes.size()], -1, false);
                        }
                    }
                    Game->GiveHeroRenownPoints(pGlobalScriptDef->ArcheryRenown);
                } else if (score == highScore) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_HIGH_MATCH", GROUP_SELECT_RANDOM)
                    Game->GiveHeroObject(pGlobalScriptDef->ArcheryMatchHighScorePrizes[rand() % pGlobalScriptDef->ArcheryMatchHighScorePrizes.size()], -1, false);
                } else if ((float)score >= (float)highScore * pGlobalScriptDef->ArcheryScoreForPrizeHigh) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW_PRIZE", GROUP_SELECT_RANDOM)
                    Game->GiveHeroObject(pGlobalScriptDef->ArcheryNormalPrizesHigh[rand() % pGlobalScriptDef->ArcheryNormalPrizesHigh.size()], -1, false);
                } else if ((float)score >= (float)highScore * pGlobalScriptDef->ArcheryScoreForPrizeMedium) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW_PRIZE", GROUP_SELECT_RANDOM)
                    Game->GiveHeroObject(pGlobalScriptDef->ArcheryNormalPrizesMedium[rand() % pGlobalScriptDef->ArcheryNormalPrizesMedium.size()], -1, false);
                } else if ((float)score >= (float)highScore * pGlobalScriptDef->ArcheryScoreForPrizeLow) {
                    AC_T;
                    AC_SPEAK("TEXT_QST_004_SCORE_LOW_PRIZE", GROUP_SELECT_RANDOM)
                    Game->GiveHeroObject(pGlobalScriptDef->ArcheryNormalPrizesLow[rand() % pGlobalScriptDef->ArcheryNormalPrizesLow.size()], -1, false);
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
