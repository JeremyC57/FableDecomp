// NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main() -- the caged bandit in Hobbe Cave: call for help until the side-room hobbes are dead, then the let-out cut scene and question (repeated on talk until let out); follow the hero commenting until the brat is released or the caves are left; if the nymph lives he is sacrificed and caged, when she dies he is freed; out of the caves he asks for his share and leaves, pays, or turns hostile
// Grade: DIFFER(9883v9821) -- 1170 masked lines (1932 before): the predicate lever of this chunk is applied (inline_depth(0) round the row-local predicate ctor / dtor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE) with FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY; left: the frame-slot order inside the 0xC8 frame (retail seh_me +2Ch, flags +20h, the unused CCharString +24h, timers +1Ch / +18h) and 62 bytes of exit expansions
// Next idea: the slot order first (packer replay), then the exits with in-body inline_depth(0) as in 0x00D5E0C0
// Leaf: ?Main@CBanditSacrifice@CQ_HobbeCaveScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CBANDITSACRIFICE_MAIN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_HobbeCaveScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/EHeroAbility.h"
#include "engine/fable_script_fade.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#pragma inline_depth()
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#define HC_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define HC_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        HC_T; \
    } \
    HC_T
#define HC_SAY(conversation, text) \
    Game->AddLineToConversation(conversation, text, false, Me, Game->GetHero())
// HIT: hit by the hero, or by one of his special abilities that is not the heal spell.
#define HC_HIT_BY_HERO (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))
#define HC_ON_HIT \
    if (HC_HIT_BY_HERO) { \
        HC_T; \
        Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero()); \
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me); \
        long conversation = Game->AddNewConversation(Me, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_HIT"); \
    }
#define HC_WAIT_FOR_ANSWER(answer) \
    long answer = Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer < 0) { \
        Game->NewScriptFrame(); \
        HC_T; \
        answer = Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    HC_T
#define HC_LEAVE \
    CScriptThing exitMarker = Game->GetThingWithScriptName("BanditExitMarker"); \
    C3DVector exitPos = exitMarker.GetPos(); \
    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), exitPos, 2.0f)) { \
        Game->NewScriptFrame(); \
        HC_T; \
        seh_me.MoveToPosition(exitPos, 0.0f, ENTITY_MOVE_RUN, false, true); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            HC_T; \
        } \
        HC_T; \
    } \
    HC_T; \
    FadeOutAndKillEntity(Me, pGlobalScriptDef->HC_BanditFadeOut, true, true); \
    for (;;) { \
        Game->NewScriptFrame(); \
        HC_T; \
    }

void NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    HC_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    HC_START_SCRIPTING(Me, seh_me);
    Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    Game->SetIsPushableByHero(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetAsRespondToHit(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_SCRIPT_NEUTRAL");
    CTimer CallForHelpTimer;
    while (!ParentClass.KilledSideRoomHobbes) {
        Game->NewScriptFrame();
        HC_T;
        if (CallForHelpTimer.GetTime() == 0) {
            HC_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_TRAPPED");
            CallForHelpTimer = pGlobalScriptDef->HC_BanditCallsForHelpDelay;
        }
        HC_ON_HIT
    }
    HC_T;
    Game->SetIsPushableByHero(Me, true);
    Game->SetIsThingForcePushable(Me, true);
    Game->EntitySetAsRespondToHit(Me, true);
    Game->EntitySetAsDamageable(Me, true);
    Game->EntitySetAsKillable(Me, true, true);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, true);
    ParentClass.InCutscene = true;
    bool letOut = false;
    Game->FadeScreenOut(1.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    Game->Pause(1.2f);
    Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("BanditOutPos"), false);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("HeroOutPos"), false);
    Game->FadeScreenIn();
    {
        CScriptGameResourceObjectMovieBase letout_movie;
        Game->StartMovieSequence("", letout_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        mymap["Bandit"] = seh_me;
        mymap["Hero"] = seh_hero;
        RunCutsceneMacro("CS_HOBBE_CAVE_BANDIT", mymap, 0, 0, false, true);
        Game->GiveHeroYesNoQuestion("TEXT_QST_036_BANDIT_LET_OUT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        HC_WAIT_FOR_ANSWER(answer);
        if (answer == 1) {
            HC_T;
            letOut = true;
        }
        ParentClass.InCutscene = false;
        Game->GiveHeroMorality(pGlobalScriptDef->HC_RescueBanditMoralityGain);
        Game->GiveHeroExperience(pGlobalScriptDef->HC_RescueBanditExperienceGain);
    }
    while (!letOut) {
        Game->NewScriptFrame();
        HC_T;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            HC_T;
            HC_ON_HIT
        }
        HC_T;
        Game->GiveHeroYesNoQuestion("TEXT_QST_036_BANDIT_LET_OUT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        HC_WAIT_FOR_ANSWER(answer);
        if (answer == 1) {
            HC_T;
            letOut = true;
        }
    }
    HC_T;
    Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->EntitySetOpinionReactionsEnabled(Me, false);
    Game->EntitySetDeedReactionsEnabled(Me, false);
    Game->DisplayQuestInfo(true);
    BanditHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 0, 255, 255), "HUD_QUEST_ICON_BANDIT", 1.0f);
    Game->EntitySetInFaction(Me, "FACTION_BODYGUARD");
    ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeCaveScript>(CCharString("ParentClass.") + CCharString("CheckIfGotTreasure"), 0, &ParentClass, &CQ_HobbeCaveScript::CheckIfGotTreasure), CCharString(""));
    CCharString unusedText;
    CTimer FollowCommentTimer;
    while (!ParentClass.ReleaseBrat && !Game->IsRegionLoaded("GreatwoodCaves")) {
        Game->NewScriptFrame();
        HC_T;
        HC_ON_HIT
        if (!MadeMainChamberHobbesComment && Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            HC_T;
            MadeMainChamberHobbesComment = ParentClass.MakeBanditComment("TEXT_QST_036_BANDIT_ENTERED_MAIN_CHAMBER", Me);
        }
        CurrentAIState = Me.GetCurrentStateGroupType();
        if (CurrentAIState != PreviousAIState && FollowCommentTimer.GetTime() == 0) {
            HC_T;
            FollowCommentTimer = 10;
            if (CurrentAIState == 2 && ParentClass.GotTreasure < 2) {
                HC_T;
                switch (++FollowingCommentsSaid) {
                case 1: ParentClass.MakeBanditComment("TEXT_QST_036_BANDIT_FOLLOWING_30", Me); break;
                case 2: ParentClass.MakeBanditComment("TEXT_QST_036_BANDIT_FOLLOWING_20", Me); break;
                default: ParentClass.MakeBanditComment("TEXT_QST_036_BANDIT_FOLLOWING_10", Me); break;
                }
            } else if (CurrentAIState == 1) {
                HC_T;
                ParentClass.MakeBanditComment("TEXT_QST_036_BANDIT_FIGHTING", Me);
            }
        }
        PreviousAIState = CurrentAIState;
    }
    HC_T;
    bool sacrificed = false;
    HC_T;
    while (!Game->GetThingWithScriptName("Nymph").IsDead()) {
        if (!sacrificed) {
            HC_T;
            Game->RemoveQuestInfoElement(BanditHealthID);
            sacrificed = true;
            BanditHealthID = 0;
            Game->EntityStopFollowing(Me);
            Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
            Game->GiveHeroExperience(pGlobalScriptDef->HC_SacrificeToNymphExperienceGain);
            Game->GiveHeroMorality(pGlobalScriptDef->HC_SacrificeBanditMoralityLoss);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_SACRIFICE_10");
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_SACRIFICE_20");
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_SACRIFICE_30");
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_SACRIFICE_40");
            HC_START_SCRIPTING(Me, seh_me);
            Game->SetIsPushableByHero(Me, false);
            CScriptThing sacrificePos = Game->GetThingWithScriptName("FollowerSacrificePos");
            CScriptThing teleportFX = Game->CreateEffect("HERO_TELEPORT_DISSAPEAR", Me.GetPos(), "", 0.0f, false, false);
            Game->Pause(0.3f);
            Game->RemoveThing(teleportFX, false, true);
            Game->EntityTeleportToThing(Me, sacrificePos, true);
            Game->NewScriptFrame();
            HC_T;
            teleportFX = Game->CreateEffect("HERO_TELEPORT_APPEAR", Me.GetPos(), "", 0.0f, false, false);
            Game->Pause(0.3f);
            Game->RemoveThing(teleportFX, false, true);
            ImprisonedFX = Game->CreateEffect("NYMPH_CAGE_IDLE", Me.GetPos(), "", 0.0f, false, false);
            Game->ActivateQuest("Q_HobbeCaveBarrier");
        }
        if (ImprisonedFX.IsDead()) {
            HC_T;
            ImprisonedFX = Game->CreateEffect("NYMPH_CAGE_IDLE", Me.GetPos(), "", 0.0f, false, false);
        }
        if (HC_HIT_BY_HERO) {
            HC_T;
            ++ParentClass.BanditHitCount;
        }
        Game->NewScriptFrame();
        HC_T;
    }
    HC_T;
    if (seh_me.IsValid()) seh_me.Release();
    Game->SetIsPushableByHero(Me, true);
    Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    Game->DisplayQuestInfo(true);
    if (sacrificed) {
        HC_T;
        Game->RemoveThing(ImprisonedFX, false, true);
        Game->DeactivateQuest("Q_HobbeCaveBarrier", 0);
        Game->CreateEffect("NYMPH_CAGE_FADE", Me.GetPos(), "", 0.0f, false, false);
        BanditHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 0, 255, 255), "HUD_QUEST_ICON_BANDIT", 1.0f);
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_RESCUE_FROM_NYMPH_10");
    }
    while (Game->IsRegionLoaded("GreatwoodCaves")) {
        Game->NewScriptFrame();
        HC_T;
    }
    HC_T;
    bool takeShare = false;
    Game->RemoveQuestInfoElement(BanditHealthID);
    HC_START_SCRIPTING(Me, seh_me);
    Game->EntityStopFollowing(Me);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
    Game->MiniMapRemoveMarker(Me);
    if (ParentClass.GotTreasure > 0) {
        HC_T;
        {
            Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("M_BanditOnBanditLeaving"), false);
            Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("M_HeroOnBanditLeaving"), false);
            CScriptThing james = Game->GetThingWithScriptName("James");
            CScriptGameResourceObjectScriptedThingBase seh_james;
            if (james.IsAlive()) {
                HC_T;
                ParentClass.WaitForJamesReleased = 1;
                while (ParentClass.WaitForJamesReleased != 2) {
                    Game->NewScriptFrame();
                    HC_T;
                }
                HC_T;
                Game->StartScriptingEntity(Game->GetThingWithScriptName("James"), seh_james, SCRIPT_AI_PRIORITY_HIGHEST);
                Game->EntityTeleportToThing(james, Game->GetThingWithScriptName("M_JamesOnBanditLeaving"), false);
            }
            CScriptGameResourceObjectMovieBase outro_movie;
            Game->StartMovieSequence("", outro_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->SheatheHeroWeapons();
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            mymap["Bandit"] = seh_me;
            mymap["Hero"] = seh_hero;
            RunCutsceneMacro("CS_HOBBE_CAVE_BANDIT_OUTRO", mymap, 0, 0, false, true);
            Game->GiveHeroYesNoQuestion("TEXT_QST_036_BANDIT_ON_PART_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            HC_WAIT_FOR_ANSWER(answer);
            if (james.IsAlive()) {
                HC_T;
                seh_james.Release();
            }
            if (answer == 1) {
                HC_T;
                takeShare = true;
            }
            if (james.IsAlive()) {
                HC_T;
                ParentClass.WaitForJamesReleased = 3;
            }
        }
        if (takeShare && Game->GetHeroGold() > pGlobalScriptDef->HC_BanditGoldDemand) {
            HC_T;
            Game->GiveHeroGold(-pGlobalScriptDef->HC_BanditGoldDemand);
            Game->GiveHeroMorality(pGlobalScriptDef->HC_LetBanditGoAndPayMoralityGain);
            Game->GiveHeroRenownPoints(pGlobalScriptDef->HC_LetBanditGoRenown);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_GIVE_MONEY");
            HC_LEAVE
        } else {
            HC_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->GiveHeroMorality(pGlobalScriptDef->HC_DontPayBanditMoralityLoss);
            if (takeShare) {
                HC_T;
                HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_LOST_MONEY");
            }
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_KEEP_MONEY_10");
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_KEEP_MONEY_20");
            Game->EntitySetInFaction(Me, "FACTION_BANDIT");
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            for (;;) {
                Game->NewScriptFrame();
                HC_T;
            }
        }
    } else {
        HC_T;
        Game->GiveHeroRenownPoints(pGlobalScriptDef->HC_LetBanditGoRenown);
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_NO_MONEY_10");
        HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_NO_MONEY_20");
        HC_LEAVE
    }
}
