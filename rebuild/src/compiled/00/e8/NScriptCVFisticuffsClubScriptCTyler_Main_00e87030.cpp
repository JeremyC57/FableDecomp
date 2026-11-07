// NScript::CV_FisticuffsClubScript::CTyler::Main() -- the punch-club master Tyler: runs a fight already in progress (counting the hero's hits on the quest bar, losing on time-out with the outside or round-lost cut scene, or winning the final round with the reward item), then stands at his watch position facing the ring, answers hits, checks the entry item and crimes, talks the hero through entry and the fee, starts the warm-up rounds and the Tyler fight, and walks off (or fades out) when the club closes or the hero has left
// Leaf: ?Main@CTyler@CV_FisticuffsClubScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E87030, 12551 bytes (to 0x00E8A137), thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012EB0C0; Ego_r.pdb and
//   FableWin.pdb both hold ?Main@CTyler@CV_FisticuffsClubScript@NScript@@EAEXXZ (private virtual).
// Model: the chunk 813 CFisticuffsMember::Main draft (predicate, includes); the landed CDarkwoodTrader::Main (map / movie / pause).
// Levers: a declared-only ~CCountedPointer<CScriptGameResourceObjectMovieBase> specialization (the first two MovieBase dtors
//   expand around the 0x006E7AB0 call, the rest call 0x006E7B80, as retail); CDefPointer<const CScriptDef> pGlobalScriptDef (the def
//   pointer is loaded into esi before the GetHealth / GetHeroGold calls); the ring-cleared start arm written first as
//   `if (!TimeOut)`: VC7.1 moves that no-return arm out of line and the lost arm's exit pads follow it, as retail.
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CV_FisticuffsClubScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/EOpinionDeedType.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
void FABLE_FASTCALL KillAllThingsInVector(CGameScriptInterfaceBase&, const std::vector<CScriptThing>&, bool);   // 0x00CBED82
namespace NScript {
class CIsVariableFalse : public CPredicateBase {           // vtable 0x012EA43C
public:
    const bool* Variable;                                  // +0x04
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual bool operator()() const;                       // slot 0, 0x00E7DDD0
    virtual CPredicateBase* Clone() const;                 // slot 1, 0x00E7DDE0
    virtual ~CIsVariableFalse() { }                        // slot 2, 0x00E7DE10
};
}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<class P1,class P2> inline NScript::CPredicate_And<P1,P2>::~CPredicate_And() {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
#define TY_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define TY_START(thing, seh, priority) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, priority)) { \
        Game->NewScriptFrame(); \
        TY_T; \
    } \
    TY_T
#define TY_SPEAK(target, key) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(target, ParentClass.GetQuestText(key), GROUP_SELECT_NONE, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            TY_T; \
        } \
        TY_T; \
    }
#define TY_SAY(text) { \
        long conversation = Game->AddNewConversation(Me, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        Game->AddLineToConversation(conversation, ParentClass.GetQuestText(text), false, Me, Game->GetHero()); \
    }
#define TY_CUTSCENE(name) RunCutsceneMacro(ParentClass.GetCutsceneName(name), mymap, 0, 0, false, true)

void NScript::CV_FisticuffsClubScript::CTyler::Main()
{
    TRetVoid _Fail_Val;
    if (AnsweredYes) {
        TY_T;
        ParentClass.HeroOfferedFisticuffsClubEntry = true;
    }
    SetPredicate(CIsThingAlive(Me) && CIsVariableFalse(ParentClass.EverybodyAttack));
    Game->NewScriptFrame();
    TY_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    if (!ParentClass.HeroInFisticuffsClub) {
        TY_T;
        Game->SetThingHasInformation(Me, false, true, false);
    } else {
        TY_T;
        Game->ClearThingHasInformation(Me);
    }
    if (ParentClass.TylerInFight) {
        TY_T;
        ParentClass.SetAllowedHits(ParentClass.FightCount);
        Game->DisplayQuestInfo(true);
        ParentClass.HitID = Game->AddQuestInfoBar((float)ParentClass.AllowedHits, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255), "HUD_HERO_ICON", "", 1.0f);
        Game->UpdateQuestInfoBar(ParentClass.HitID, (float)(ParentClass.AllowedHits - ParentClass.HitCount), (float)ParentClass.AllowedHits, -1.0f);
        Me.SetFriendsWithEverythingFlag(false);
    }
    Game->EntitySetOpinionDeedTypeEnabled(Me, OPINION_DEED_TYPE_CRIME_ASSAULT, false);
    while (ParentClass.TylerInFight && Game->GetHealth(Me) >= pGlobalScriptDef->FC_MinimumFighterHealth) {
        Game->NewScriptFrame();
        TY_T;
        if (Game->GetHero().MsgIsHitBy("Tyler")) {
            TY_T;
            if (!Game->IsPlayerCreatureBlocking()) {
                TY_T;
                ParentClass.HitCount++;
                Game->UpdateQuestInfoBar(ParentClass.HitID, (float)(ParentClass.AllowedHits - ParentClass.HitCount), -1.0f, -1.0f);
            }
        }
        if (ParentClass.HitCount >= ParentClass.AllowedHits) {
            TY_T;
            ParentClass.TimeOut = true;
        }
        if (ParentClass.TimeOut) {
            TY_T;
            ParentClass.TylerInFight = false;
            if (ParentClass.LostWhileOutside) {
                TY_T;
                TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["TYLER"] = seh_me;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                TY_CUTSCENE("RUNFORESTRUN");
                Game->CameraDefault();
                ParentClass.HeroInFisticuffsClub = false;
                PunchClubLeftReason = "TYLER_LOST_WHILE_OUTSIDE";
            } else {
                TY_T;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"] = seh_hero;
                mymap["TYLER"] = seh_me;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                TY_CUTSCENE("ROUNDLOST");
                PunchClubLeftReason = "TYLER_LOST_AGAINST_TYLER";
            }
            CleanupAfterFight();
            ParentClass.PlayerLost = true;
            Game->GiveHeroRenownPoints(pGlobalScriptDef->FC_LostRenownLoss);
            Game->EntitySetPersonalityOverride(Me, "OPINION_PERSONALITY_PUNCHCLUB_MEMBER_LOST");
            seh_me.Release();
        }
    }
    TY_T;
    Game->EntitySetOpinionDeedTypeEnabled(Me, OPINION_DEED_TYPE_CRIME_ASSAULT, true);
    if (Game->GetHealth(Me) < pGlobalScriptDef->FC_MinimumFighterHealth && ParentClass.TylerInFight) {
        TY_T;
        if (ParentClass.HitCount == 0 && !ParentClass.TimeOut) {
            TY_T;
            Game->AddScreenTitleMessage(ParentClass.GetQuestText("PERFECT_ROUND"), 2.0f, false);
        }
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TYLER"] = seh_me;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            CCharString cutscene = ParentClass.GetCutsceneName("FINALWIN");
            CScriptThing crate;
            if (cutscene == "CS_PUNCHCLUB_KG_FINALWIN") {
                TY_T;
                CScriptThing marker = Game->GetThingWithScriptName("P_CRATEMARKER");
                crate = Game->CreateObject("OBJECT_CRATE_PILE", marker.GetPos(), "");
            }
            RunCutsceneMacro(cutscene, mymap, 0, 0, false, true);
            if (crate.IsAlive()) {
                TY_T;
                Game->RemoveThing(crate, false, true);
            }
            if (ParentClass.RelevantRegion == REGION_KNOTHOLE) {
                TY_T;
                if (!ParentClass.CompletedThisPunchClub) {
                    TY_T;
                    ParentClass.CompletedKnotholePunchClub = true;
                    Game->GiveHeroObject(pGlobalScriptDef->FC_KnotholeGladeRewardItem, -1, false);
                }
            } else if (ParentClass.RelevantRegion == REGION_BOWERSTONE) {
                TY_T;
                if (!ParentClass.CompletedThisPunchClub) {
                    TY_T;
                    ParentClass.CompletedBowerstonePunchClub = true;
                    Game->GiveHeroObject(pGlobalScriptDef->FC_BowerstoneRewardItem, -1, false);
                }
            } else if (ParentClass.RelevantRegion == REGION_OAKVALE) {
                TY_T;
                if (!ParentClass.CompletedThisPunchClub) {
                    TY_T;
                    ParentClass.CompletedOakvalePunchClub = true;
                    Game->GiveHeroObject(pGlobalScriptDef->FC_OakvaleRewardItem, -1, false);
                }
            } else if (ParentClass.RelevantRegion == REGION_BANDITCAMP) {
                TY_T;
                if (!ParentClass.CompletedThisPunchClub) {
                    TY_T;
                    ParentClass.CompletedBanditPunchClub = true;
                    Game->GiveHeroObject(pGlobalScriptDef->FC_BanditCampRewardItem, -1, false);
                }
            }
            if (cutscene == "CS_PUNCHCLUB_KG_FINALWIN") {
                TY_T;
                Game->Pause(1.0f);
                Game->FadeScreenIn();
            }
        }
        CleanupAfterFight();
        ParentClass.TylerInFight = false;
        ParentClass.HeroBeatTyler = true;
        Game->GiveHeroGold(Money);
        Game->GiveHeroExperience(ParentClass.RelevantRegion * pGlobalScriptDef->FC_WinExperienceGain);
        Game->GiveHeroRenownPoints(ParentClass.RelevantRegion * pGlobalScriptDef->FC_WinRenownGain);
        Game->EntitySetPersonalityOverride(Me, "OPINION_PERSONALITY_PUNCHCLUB_MEMBER_WON");
        seh_me.Release();
        PunchClubLeftReason = "TYLER_WON_AGAINST_TYLER";
    }
    CScriptThing ring = Game->GetThingWithScriptName("FisticuffsClubRing");
    CCharString entry_item;
    switch (ParentClass.RelevantRegion) {
    case REGION_OAKVALE:
        entry_item = pGlobalScriptDef->FC_BowerstoneRewardItem;
        break;
    case REGION_BANDITCAMP:
        entry_item = pGlobalScriptDef->FC_OakvaleRewardItem;
        break;
    case REGION_KNOTHOLE:
        entry_item = pGlobalScriptDef->FC_BanditCampRewardItem;
        break;
    default:
        entry_item = pGlobalScriptDef->FC_KnotholeGladeRewardItem;
        break;
    }
    CScriptThing watch_pos = Game->GetThingWithScriptName("TylerWatchPos");
    bool paying = false;
    CCharString presented_item;
    TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
    bool moved = false;
    while (!IsActiveThreadTerminating()) {
        if (PunchClubLeftReason != "NONE") {
            TY_T;
            CScriptThing use_this = Game->GetRandomThingWithScriptName("FisticuffsUseThis");
            TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
            seh_me.ClearCommands();
            seh_me.MoveToThing(use_this, 2.0f, ENTITY_MOVE_WALK, false, true, false, true);
            while (IsDistanceBetweenThingsOver(Me, use_this, 3.0f)) {
                Game->NewScriptFrame();
                TY_T;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    TY_T;
                    seh_me.ClearCommands();
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, ParentClass.GetQuestText(PunchClubLeftReason), false, Me, Game->GetHero());
                    while (!Game->IsConversationActive(conversation)) {
                        Game->NewScriptFrame();
                        TY_T;
                    }
                    TY_T;
                    seh_me.MoveToThing(use_this, 2.0f, ENTITY_MOVE_WALK, false, true, false, true);
                }
            }
            TY_T;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
            return;
        }
        if (IsDistanceBetweenThingsOver(Me, watch_pos, 2.0f) && !Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !moved) {
            TY_T;
            seh_me.MoveToPosition(watch_pos.GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
            moved = true;
        }
        if (!seh_me.IsPerformingScriptTask()) {
            TY_T;
            Game->EntitySetFacingAngleTowardsThing(Me, ring, false);
        }
        if (!ParentClass.FisticuffsActive && !ParentClass.HeroInFisticuffsClub) {
            TY_T;
            CScriptThing use_this = Game->GetRandomThingWithScriptName("FisticuffsUseThis");
            TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
            seh_me.ClearCommands();
            seh_me.MoveToThing(use_this, 2.0f, ENTITY_MOVE_RUN, false, true, false, true);
            while (IsDistanceBetweenThingsOver(Me, use_this, 3.0f)) {
                Game->NewScriptFrame();
                TY_T;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    TY_T;
                    TY_SAY("TYLER_TOO_LATE_FOR_PUNCH_CLUB");
                }
            }
            TY_T;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
            return;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            TY_T;
            TY_SAY("TYLER_ON_HIT_10");
            ParentClass.BeenAttacked();
        }
        if (Me.MsgIsPresentedWithItem(presented_item) && presented_item == entry_item) {
            TY_T;
            ShownItem = true;
        } else {
            TY_T;
            ShownItem = false;
        }
        if (!OutstandingCrimes && Game->IsReportedOrUnreportedCrimeKnown(ParentClass.Village)) {
            TY_T;
            Game->ClearThingHasInformation(Me);
            OutstandingCrimes = true;
        } else if (OutstandingCrimes && !Game->IsReportedOrUnreportedCrimeKnown(ParentClass.Village)) {
            TY_T;
            Game->SetThingHasInformation(Me, false, true, false);
            OutstandingCrimes = false;
        }
        if (!OutstandingCrimes && (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || ShownItem)) {
            TY_T;
            seh_me.ClearCommands();
            moved = false;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.CompletedThisPunchClub) {
                TY_T;
                TY_SPEAK(Game->GetHero(), "TYLER_REENTER");
                AnsweredYes = true;
            } else if (!ShownItem && !Game->IsObjectInThingsPossession(entry_item, Game->GetHero()) && ParentClass.RelevantRegion != REGION_BOWERSTONE) {
                TY_T;
                TY_SPEAK(Game->GetHero(), "TYLER_NOT_GOT_ENTRY_REQUIREMENTS");
                AnsweredYes = false;
            } else {
                TY_T;
                if (ParentClass.RelevantRegion == REGION_BOWERSTONE) {
                    TY_T;
                    if (!IntroducedPunchClub) {
                        TY_T;
                        TY_SPEAK(Game->GetHero(), "TYLER_PUNCHCLUB_INTRO");
                        IntroducedPunchClub = true;
                    }
                }
                if (!ParentClass.HeroOfferedFisticuffsClubEntry) {
                    TY_T;
                    TY_SPEAK(Game->GetHero(), "TYLER_INTRO_10");
                    TY_SPEAK(Game->GetHero(), "TYLER_GOT_ENTRY_REQUIREMENTS");
                    TY_SPEAK(Game->GetHero(), "TYLER_INTRO_20");
                } else {
                    TY_T;
                    TY_SPEAK(Game->GetHero(), "TYLER_FIGHT_START_10");
                    ParentClass.TimeOut = false;
                }
                AnsweredYes = true;
            }
            if (AnsweredYes && !ParentClass.HeroOfferedFisticuffsClubEntry) {
                TY_T;
                ParentClass.HeroOfferedFisticuffsClubEntry = true;
                if (ParentClass.CompletedThisPunchClub) {
                    TY_T;
                    seh_me.ClearCommands();
                    TY_SPEAK(Game->GetHero(), "TYLER_INTRO_B_40");
                } else {
                    TY_T;
                    seh_me.ClearCommands();
                    TY_SPEAK(Game->GetHero(), "TYLER_INTRO_B");
                }
            }
            if (AnsweredYes && ParentClass.HeroOfferedFisticuffsClubEntry) {
                TY_T;
                Game->GiveHeroYesNoQuestion(ParentClass.GetQuestText("TYLER_QUESTION_MONEY"), "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) {
                    Game->NewScriptFrame();
                    TY_T;
                    answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                TY_T;
                if (answer == 1) {
                    TY_T;
                    paying = true;
                }
            }
            if (ParentClass.HeroOfferedFisticuffsClubEntry && paying) {
                TY_T;
                if (Game->GetHeroGold() >= pGlobalScriptDef->FC_MinimumFee) {
                    TY_T;
                    Game->GiveHeroGold(-pGlobalScriptDef->FC_MinimumFee);
                    Money = pGlobalScriptDef->FC_MinimumFee;
                    ParentClass.HeroPaidEntryFee = true;
                    std::vector<CScriptThing> creatures;
                    Game->GetAllThingsWithScriptName("HeroSummonnedCreature", creatures);
                    for (unsigned long i = 0; i < creatures.size(); i++) {
                        TY_T;
                        Game->RemoveThing(creatures[i], false, true);
                    }
                    TY_T;
                    Game->NewScriptFrame();
                    TY_T;
                    Game->NewScriptFrame();
                    TY_T;
                    {
                        CScriptGameResourceObjectScriptedThingBase seh_hero;
                        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                        seh_me.ClearCommands();
                        TY_SPEAK(Game->GetHero(), "HERO_START_10");
                        Game->SheatheHeroWeapons();
                        Game->ConfiscateAllHeroWeapons();
                        Game->SetHeroAsWearing("OBJECT_HERO_NO_SHIRT");
                        Game->SetHeroAsWearing("OBJECT_HERO_NO_GLOVES");
                        Game->SetHeroAsWearing("OBJECT_HERO_NO_HAT");
                        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                        mymap["HERO"] = seh_hero;
                        mymap["TYLER"] = seh_me;
                        TY_CUTSCENE("START");
                        ParentClass.HeroInFisticuffsClub = true;
                    }
                    Game->ClearThingHasInformation(Me);
                } else {
                    TY_T;
                    TY_SPEAK(Me, "TYLER_NOT_ENOUGH_MONEY");
                    paying = false;
                }
            }
        }
        if (!ParentClass.HeroOfferedFisticuffsClubEntry && Me.MsgReceivedMoney(Money)) {
            TY_T;
            Game->GiveHeroGold(Money);
            TY_SAY("TYLER_MONEY_UNWANTED");
        }
        if (ParentClass.HeroOfferedFisticuffsClubEntry && paying) {
            TY_T;
            FightWarmupRounds();
            if (!ParentClass.TimeOut) {
                TY_T;
                while (!ParentClass.RingCleared) {
                    Game->NewScriptFrame();
                    TY_T;
                }
                TY_T;
                {
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["TYLER"] = seh_me;
                    TY_CUTSCENE("TYLERSTART");
                }
                Game->PlayCriteriaSoundOnThing(Me, "BOXING_ROUND_BELL");
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                Game->ModifyThingHealth(Me, 100.0f, false);
                TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_LOWEST);
                ParentClass.TylerInFight = true;
                for (;;) {
                    Game->NewScriptFrame();
                    TY_T;
                }
            } else {
                TY_T;
                ParentClass.TylerInFight = false;
                if (!ParentClass.LostWhileOutside) {
                    TY_T;
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"] = seh_hero;
                    mymap["TYLER"] = seh_me;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    TY_CUTSCENE("ROUNDLOST");
                    PunchClubLeftReason = "TYLER_LOST_BEFORE_TYLER";
                } else {
                    TY_T;
                    std::vector<CScriptThing> fighters;
                    Game->GetAllThingsWithScriptName("RingFighter", fighters);
                    KillAllThingsInVector(*Game, fighters, false);
                    TY_START(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["TYLER"] = seh_me;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    TY_CUTSCENE("RUNFORESTRUN");
                    Game->CameraDefault();
                    PunchClubLeftReason = "TYLER_LOST_WHILE_OUTSIDE";
                }
                ParentClass.PlayerLost = true;
                ParentClass.HeroInFisticuffsClub = false;
                Game->EntitySetPersonalityOverride(Me, "OPINION_PERSONALITY_PUNCHCLUB_MEMBER_LOST");
                seh_me.Release();
            }
        }
        Game->NewScriptFrame();
    }
}