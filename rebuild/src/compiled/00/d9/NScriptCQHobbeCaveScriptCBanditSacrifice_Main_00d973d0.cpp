// NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main() -- the caged bandit in Hobbe Cave: call for help until the side-room hobbes are dead, then the let-out cut scene and question (repeated on talk until let out); follow the hero commenting until the brat is released or the caves are left; if the nymph lives he is sacrificed and caged, when she dies he is freed; out of the caves he asks for his share and leaves, pays, or turns hostile
// Matched (wfE2, fourth author): RELOCATION_MATCH. The two yes/no answers (let-out question in the cut scene and its repeat loop; the share question in the outro) live in ONE bool, saidYes (descriptive name: bool register locals are not in any PDB). Evidence: retail frame packing -- the flag sits in the first byte slot with sacrificed (init 0x00D9787D [esp+0x1F], reload 0x00D97B9A; share stores 0x00D98E56 / 0x00D9934B, reloads 0x00D993F7) and the Nymph-loop temps alone in the slot above seh_me; c2 orders byte items by memory-reference weight, and only the merged symbol (w6 = 2 + 4) is placed before the Nymph HIT temp (w3); two separate bools (w2 / w4) put the let-out flag in the Nymph slot (43 frame hunks). FITTED: sacrificed kept separate (folding it into the same bool is byte-identical too).
// Closed (wfE2 successor): Nymph loop as the sibling idiom `for (; !false; Game->NewScriptFrame()) { HC_T; if (Nymph dead) break; ...}` (retail T before the loop 0x00D9851F + rotated bottom T 0x00D98B99, no expanded ~CTimer) 9964 -> 9805; `while (!Game->IsRegionLoaded("GreatwoodCaves"))` (retail neg/sbb/inc of the result, 0x00D98DE3: the draft had the test inverted, a behaviour bug); `sacrificed = true` before RemoveQuestInfoElement (0x00D985AE); `PreviousAIState != CurrentAIState` (cmp [esi+0x20],eax 0x00D9839E). Locals renamed to the Anniversary PDB names. Health bar colour CRGBColour(255, 0, 0, 255) at both AddQuestInfoBarHealth sites (retail R byte +2 = 0xFF, B byte +0 = 0, 0x00D97FAD / 0x00D98D3E; the draft had blue, a behaviour bug).
// Leaf: ?Main@CBanditSacrifice@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Inline-budget: scoped `#pragma inline_depth(0)` round the row-local CIsThingAlive ctor / dtor definitions -- the landed CIsThingAlive predicate idiom (FABLE_RETAIL_CPREDICATEBASE_NOVTABLE, style_wfD rows): retail calls both at SetPredicate.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D973D0, 9821 bytes (ret 0x00D99A2C, int3 filler), thiscall, vtable slot Main of CBanditSacrifice.
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
#include "engine/EScriptEntityMoveType.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
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
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
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
    CScriptThing ExitMarker = Game->GetThingWithScriptName("BanditExitMarker"); \
    C3DVector local_position = ExitMarker.GetPos(); \
    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, 2.0f)) { \
        Game->NewScriptFrame(); \
        HC_T; \
        seh_me.MoveToPosition(local_position, 0.0f, ENTITY_MOVE_RUN, false, true); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            HC_T; \
        } \
        HC_T; \
    } \
    HC_T; \
    FadeOutAndKillEntity(Me, pGlobalScriptDef->HC_BanditFadeOut, true, true);

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
    CTimer Trapped;
    while (!ParentClass.KilledSideRoomHobbes) {
        Game->NewScriptFrame();
        HC_T;
        if (Trapped.GetTime() == 0) {
            HC_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_TRAPPED");
            Trapped = pGlobalScriptDef->HC_BanditCallsForHelpDelay;
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
    bool saidYes = false;
    Game->FadeScreenOut(1.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    Game->Pause(1.2f);
    Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("BanditOutPos"), false);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("HeroOutPos"), false);
    Game->FadeScreenIn();
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST);
        mymap["Bandit"] = seh_me;
        mymap["Hero"] = seh_Hero;
        RunCutsceneMacro("CS_HOBBE_CAVE_BANDIT", mymap, 0, 0, false, true);
        Game->GiveHeroYesNoQuestion("TEXT_QST_036_BANDIT_LET_OUT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        HC_WAIT_FOR_ANSWER(answer);
        if (answer == 1) {
            HC_T;
            saidYes = true;
        }
        ParentClass.InCutscene = false;
        Game->GiveHeroMorality(pGlobalScriptDef->HC_RescueBanditMoralityGain);
        Game->GiveHeroExperience(pGlobalScriptDef->HC_RescueBanditExperienceGain);
    }
    while (!saidYes) {
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
            saidYes = true;
        }
    }
    HC_T;
    seh_me.Release();
    Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->EntitySetOpinionReactionsEnabled(Me, false);
    Game->EntitySetDeedReactionsEnabled(Me, false);
    Game->DisplayQuestInfo(true);
    BanditHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_BANDIT", 1.0f);
    Game->EntitySetInFaction(Me, "FACTION_BODYGUARD");
    ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeCaveScript>(CCharString("ParentClass.") + CCharString("CheckIfGotTreasure"), 0, &ParentClass, &CQ_HobbeCaveScript::CheckIfGotTreasure), CCharString(""));
    CCharString LookingForHobbeMainChamber;
    CTimer AIComments;
    while (!ParentClass.ReleaseBrat && !Game->IsRegionLoaded("GreatwoodCaves")) {
        Game->NewScriptFrame();
        HC_T;
        HC_ON_HIT
        if (!MadeMainChamberHobbesComment && Game->IsRegionLoaded("HobbeCaveMainChamber")) {
            HC_T;
            MadeMainChamberHobbesComment = ParentClass.MakeBanditComment("TEXT_QST_036_BANDIT_ENTERED_MAIN_CHAMBER", Me);
        }
        CurrentAIState = Me.GetCurrentStateGroupType();
        if (PreviousAIState != CurrentAIState && AIComments.GetTime() == 0) {
            HC_T;
            AIComments = 10;
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
    for (; !false; Game->NewScriptFrame()) {
        HC_T;
        if (Game->GetThingWithScriptName("Nymph").IsDead()) break;
        if (!sacrificed) {
            HC_T;
            sacrificed = true;
            Game->RemoveQuestInfoElement(BanditHealthID);
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
            CScriptThing SacrificePos = Game->GetThingWithScriptName("FollowerSacrificePos");
            CScriptThing TempEffect = Game->CreateEffect("HERO_TELEPORT_DISSAPEAR", Me.GetPos(), "", 0.0f, false, false);
            Game->Pause(0.3f);
            Game->RemoveThing(TempEffect, false, true);
            Game->EntityTeleportToThing(Me, SacrificePos, true);
            Game->NewScriptFrame();
            HC_T;
            TempEffect = Game->CreateEffect("HERO_TELEPORT_APPEAR", Me.GetPos(), "", 0.0f, false, false);
            Game->Pause(0.3f);
            Game->RemoveThing(TempEffect, false, true);
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
        BanditHealthID = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_BANDIT", 1.0f);
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_RESCUE_FROM_NYMPH_10");
    }
    while (!Game->IsRegionLoaded("GreatwoodCaves")) {
        Game->NewScriptFrame();
        HC_T;
    }
    HC_T;
    saidYes = false;
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
            CScriptThing MyJames = Game->GetThingWithScriptName("James");
            CScriptGameResourceObjectScriptedThingBase seh_James;
            if (MyJames.IsAlive()) {
                HC_T;
                ParentClass.WaitForJamesReleased = 1;
                while (ParentClass.WaitForJamesReleased != 2) {
                    Game->NewScriptFrame();
                    HC_T;
                }
                HC_T;
                Game->StartScriptingEntity(Game->GetThingWithScriptName("James"), seh_James, SCRIPT_AI_PRIORITY_HIGHEST);
                Game->EntityTeleportToThing(MyJames, Game->GetThingWithScriptName("M_JamesOnBanditLeaving"), false);
            }
            CScriptGameResourceObjectMovieBase outro_movie;
            Game->StartMovieSequence("", outro_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->SheatheHeroWeapons();
            CScriptGameResourceObjectScriptedThingBase seh_Hero;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST);
            mymap["Bandit"] = seh_me;
            mymap["Hero"] = seh_Hero;
            RunCutsceneMacro("CS_HOBBE_CAVE_BANDIT_OUTRO", mymap, 0, 0, false, true);
            Game->GiveHeroYesNoQuestion("TEXT_QST_036_BANDIT_ON_PART_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            HC_WAIT_FOR_ANSWER(answer);
            if (MyJames.IsAlive()) {
                HC_T;
                seh_James.Release();
            }
            if (answer == 1) {
                HC_T;
                saidYes = true;
            }
            if (MyJames.IsAlive()) {
                HC_T;
                ParentClass.WaitForJamesReleased = 3;
            }
        }
        if (saidYes && Game->GetHeroGold() > pGlobalScriptDef->HC_BanditGoldDemand) {
            HC_T;
            Game->GiveHeroGold(-pGlobalScriptDef->HC_BanditGoldDemand);
            Game->GiveHeroMorality(pGlobalScriptDef->HC_LetBanditGoAndPayMoralityGain);
            Game->GiveHeroRenownPoints(pGlobalScriptDef->HC_LetBanditGoRenown);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_GIVE_MONEY");
            HC_LEAVE
            for (;;) {
                Game->NewScriptFrame();
                HC_T;
            }
        } else {
            HC_T;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->GiveHeroMorality(pGlobalScriptDef->HC_DontPayBanditMoralityLoss);
            if (saidYes) {
                HC_T;
                HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_LOST_MONEY");
            }
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_KEEP_MONEY_10");
            HC_SAY(conversation, "TEXT_QST_036_BANDIT_ON_PART_KEEP_MONEY_20");
            Game->EntitySetInFaction(Me, "FACTION_BANDIT");
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            seh_me.Release();
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
        for (;;) {
            Game->NewScriptFrame();
            HC_T;
        }
    }
}