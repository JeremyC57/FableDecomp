// NScript::CQ_TraderEscortScript::CDarkwoodTrader::Main() -- a Darkwood escort trader: after the intro, the infected trader lies unconscious until the parent lets it up, then waits for the hero at the cut-scene trigger (fading out at the infected exit if the mission failed) and plays the infected cut scene with the other traders and the follow question (leaving costs the hero morality and removes it); then every trader follows the hero: it stops at the Barrow Fields marker and thanks the hero, runs off while traders are scared, enters and leaves the Darkwood safe zone, greets the camp trader, comments on infection, attacks, following and banter, and once the incubation runs out spawns the parent's TurnToBalv thread; when the end has started it walks to the end position and thanks the hero whenever spoken to
// Leaf: ?Main@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E07640, 11984 bytes (code to 0x00E0A4FE, then the 4-entry banter switch table 0x00E0A500..
//   0x00E0A510), thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012DF424 (slot 0 the landed
//   scalar deleting dtor 0x00E016F0, slot 2 Init 0x00E04BD0, slot 3 GetParentScript 0x00E016E0, slot 4 OnPersist 0x00E05330, slot 5
//   OnPredicateFail 0x00E01760, all CDarkwoodTrader); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CDarkwoodTrader@CQ_TraderEscortScript@NScript@@EAEXXZ.
// Callees: CDarkwoodTrader::SetBrainState 0x00E0A510 (catalog "CTraderScript::HandleTrader"; the PDB's only otherwise unplaced
//   CDarkwoodTrader member, private void(long), stores BrainState +0x20), CQ_TraderEscortScript::MakeTraderComment 0x00E01900,
//   CQ_TraderEscortScript::TurnToBalv 0x00E0A820 through CSpawnedFunc1<CQ_TraderEscortScript,CScriptThing> (vtable 0x012DF764).
// Model: the landed sibling CEndTrader::Main 0x00E055B0 (defines, includes, move-to-position block, speak block).
// Levers: CTimer operator< (long) for the incubation tests (argument preloaded as retail); the hero-behind test as one x87
//   expression me < hero; SetEntityAsRegionFollowing(hero, Me, b); the not-alive arm `T; for(;;){NSF; T;}` tail-merges into the
//   joined path's exit test as retail (a `while (!T) NSF;` duplicates the test).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_SETBRAINSTATE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_TraderEscortScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&,const C3DVector&,float);   // 0x00CBE45C
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&,const CScriptThing&);             // 0x00CBE512
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline bool CTimer::operator==(long time) const { return GetTime() == time; }
__forceinline bool CTimer::operator!=(long time) const { return GetTime() != time; }
__forceinline bool CTimer::operator<(long time) const { return GetTime() < time; }
#define DW_T if (IsActiveThreadTerminating()) return _Fail_Val.Return()
#define DW_START_SCRIPTING(thing, seh) \
    if (seh.IsValid()) seh.Release(); \
    while (!Game->StartScriptingEntity(thing, seh, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        DW_T; \
    } \
    DW_T
#define DW_MOVE_TO_POSITION(thing, range, movetype) { \
        float local_range = range; \
        C3DVector local_position = (thing).GetPos(); \
        EScriptEntityMoveType local_movetype = movetype; \
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range + 2.0)) { \
            Game->NewScriptFrame(); \
            DW_T; \
            seh_me.MoveToPosition(local_position, local_range, local_movetype, false, true); \
            while (seh_me.IsPerformingScriptTask()) { \
                Game->NewScriptFrame(); \
                DW_T; \
            } \
            DW_T; \
        } \
    }
#define DW_LINE(suffix) ("TEXT_QST_067_" + Me.GetDataString() + suffix)
#define DW_SAY(suffix) { \
        long conversation = Game->AddNewConversation(Me, false, false); \
        Game->AddPersonToConversation(conversation, Game->GetHero()); \
        Game->AddLineToConversation(conversation, DW_LINE(suffix), false, Me, Game->GetHero()); \
    }
#define DW_CUTSCENE(name) RunCutsceneMacro(name, mymap, 0, 0, false, true)

void NScript::CQ_TraderEscortScript::CDarkwoodTrader::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    DW_T;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    while (!ParentClass.IntroFinished) {
        Game->NewScriptFrame();
        DW_T;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            DW_T;
        }
    }
    DW_T;
    if (BrainState == 1) {
        DW_T;
        CCharString infected_text;
        DW_START_SCRIPTING(Me, seh_me);
        Game->SetThingAsConscious(Me, false, "");
        Game->EntitySetTargetable(Me, false);
        Game->EntitySetAsDamageable(Me, false);
        while (!ParentClass.InfectedTraderCanGetUp) {
            Game->NewScriptFrame();
            DW_T;
        }
        DW_T;
        Game->EntitySetAsDamageable(Me, true);
        Game->EntitySetTargetable(Me, true);
        Game->SetThingAsConscious(Me, true, "");
        CScriptThing trigger = Game->GetThingWithScriptName("MK_DTBE_CUTSCENETRIGGER");
        while (IsDistanceBetweenThingsOver(trigger, Game->GetHero(), 5.0f)) {
            Game->NewScriptFrame();
            DW_T;
            if (ParentClass.MissionFailed) {
                DW_T;
                CScriptThing infected_exit = Game->GetThingWithScriptName("MK_DTEB_INFEXIT");
                DW_MOVE_TO_POSITION(infected_exit, 0.0f, ENTITY_MOVE_RUN)
                DW_T;
                FadeOutAndKillEntity(Me, 2.0f, true, true);
            }
        }
        DW_T;
        bool joined = false;
        if (!Me.IsDead() && !(Game->GetHealth(Me) <= 0.0f)) {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            CScriptThing nearest = Game->GetNearestWithScriptName(Me, "DarkwoodTrader");
            CScriptThing furthest = Game->GetFurthestWithScriptName(Me, "DarkwoodTrader");
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_scared;
            CScriptGameResourceObjectScriptedThingBase seh_normal;
            DW_START_SCRIPTING(Game->GetHero(), seh_hero);
            if (nearest == furthest) {
                DW_T;
                furthest = CScriptThing();
            }
            if (nearest.GetDataString() != "SCARED" && furthest.IsAlive()) {
                DW_T;
                CScriptThing swap = nearest;
                nearest = furthest;
                furthest = swap;
            }
            Game->FixMovieSequenceCamera(true);
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["TRADERI"] = seh_me;
            if (nearest.IsAlive()) {
                DW_T;
                if (furthest.IsAlive()) {
                    DW_T;
                    DW_START_SCRIPTING(nearest, seh_scared);
                    DW_START_SCRIPTING(furthest, seh_normal);
                    mymap["TRADERS"] = seh_scared;
                    mymap["TRADERN"] = seh_normal;
                    DW_CUTSCENE("CS_DARKWOOD_TRADER_INFECTED_BOTH");
                } else if (nearest.GetDataString() == "SCARED") {
                    DW_T;
                    DW_START_SCRIPTING(nearest, seh_scared);
                    mymap["TRADERS"] = seh_scared;
                    DW_CUTSCENE("CS_DARKWOOD_TRADER_INFECTED_SCARED");
                } else {
                    DW_T;
                    DW_START_SCRIPTING(nearest, seh_normal);
                    mymap["TRADERN"] = seh_normal;
                    DW_CUTSCENE("CS_DARKWOOD_TRADER_INFECTED_NORMAL");
                }
            }
            Game->GiveHeroYesNoQuestion("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer = Game->MsgIsQuestionAnsweredYesOrNo();
            while (answer < 0) {
                Game->NewScriptFrame();
                DW_T;
                answer = Game->MsgIsQuestionAnsweredYesOrNo();
            }
            DW_T;
            if (answer == 1) {
                DW_T;
                joined = true;
                DW_CUTSCENE("CS_DARKWOOD_TRADER_INFECTED_JOINS");
            } else {
                DW_T;
                DW_CUTSCENE("CS_DARKWOOD_TRADER_INFECTED_LEAVES");
            }
            Game->FixMovieSequenceCamera(false);
        } else {
            DW_T;
            for (;;) {
                Game->NewScriptFrame();
                DW_T;
            }
        }
        if (joined) {
            DW_T;
            if (seh_me.IsValid()) seh_me.Release();
            Game->ClearThingHasInformation(Me);
        } else {
            DW_T;
            Game->GiveHeroMorality(pGlobalScriptDef->TraderEscortLeaveTraderMorality);
            Game->RemoveThing(Me, false, true);
            return _Fail_Val.Return();
        }
    }
    SetBrainState(2);
    ParentClass.TradersStillAliveCounter++;
    RegulateBanterComment = 30;
    for (;; Game->NewScriptFrame()) {
        DW_T;
        if (!LeadToCamp && Game->IsRegionLoaded("BarrowFields")) {
            DW_T;
            Game->EntityStopFollowing(Me);
            Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
            Game->EntitySetAsScared(Me, false);
            ParentClass.CommentTimer = 0;
            ParentClass.MakeTraderComment("IN_BARROW_FIELD", Me, TRADER_COMMENT_SINGLE);
            DW_START_SCRIPTING(Me, seh_me);
            CScriptThing stop_here = Game->GetThingWithScriptName("M_TradersStopHere");
            bool told_to_follow = false;
            long move_state = 0;
            seh_me.ClearCommands();
            seh_me.MoveToPosition(stop_here.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
            while (!IsDistanceBetweenThingsUnder(Me, stop_here, 2.0f)) {
                Game->NewScriptFrame();
                DW_T;
                bool hero_behind = GetDistanceBetweenThings(Me, stop_here) < GetDistanceBetweenThings(Game->GetHero(), stop_here);
                if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 9.0f) && hero_behind) {
                    DW_T;
                    seh_me.ClearCommands();
                    seh_me.ClearAllActions();
                    if (!told_to_follow) {
                        DW_T;
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                        DW_SAY("_FOLLOW_ME")
                        Game->Pause(1.0f);
                        told_to_follow = true;
                        move_state = 2;
                    }
                } else {
                    DW_T;
                    if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f) && hero_behind) {
                        DW_T;
                        if (move_state != 1) {
                            DW_T;
                            seh_me.ClearCommands();
                            seh_me.MoveToPosition(stop_here.GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
                            move_state = 1;
                        }
                    } else {
                        DW_T;
                        if (move_state != 0) {
                            DW_T;
                            seh_me.ClearCommands();
                            seh_me.MoveToPosition(stop_here.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                            move_state = 0;
                        }
                    }
                    told_to_follow = false;
                }
            }
            DW_T;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            ParentClass.MakeTraderComment("THANKS_10", Me, TRADER_COMMENT_SINGLE);
            LeadToCamp = true;
        }
        if (ParentClass.EndStarted) {
            DW_T;
            Game->RemoveQuestInfoElement(TraderHealthID);
            Game->MiniMapRemoveMarker(Me);
            CScriptThing end_pos = Game->GetThingWithScriptName("TraderEndPos");
            Game->EntityStopFollowing(Me);
            Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
            Game->EntitySetAsScared(Me, false);
            Game->ModifyThingHealth(Me, 30.0f - Game->GetHealth(Me), false);
            DW_START_SCRIPTING(Me, seh_me);
            while (IsDistanceBetweenThingsOver(Me, end_pos, 7.0f)) {
                Game->NewScriptFrame();
                DW_T;
                if (!seh_me.IsPerformingScriptTask()) {
                    DW_T;
                    seh_me.MoveToThing(end_pos, 4.0f, ENTITY_MOVE_WALK, false, true, false, true);
                }
            }
            DW_T;
            seh_me.Release();
            for (;; Game->NewScriptFrame()) {
                DW_T;
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    DW_T;
                    DW_START_SCRIPTING(Me, seh_me);
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), DW_LINE("_THANKS"), GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                DW_T;
                            }
                            DW_T;
                        }
                    }
                    seh_me.Release();
                }
            }
        }
        if (ParentClass.TradersShouldBeScared) {
            DW_T;
            CScriptThing run_off = Game->GetThingWithScriptName("DTE_RunOffPos");
            Game->EntityStopFollowing(Me);
            Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
            Game->EntitySetAsScared(Me, true);
            DW_START_SCRIPTING(Me, seh_me);
            DW_MOVE_TO_POSITION(run_off, 1.0f, ENTITY_MOVE_RUN)
            DW_T;
            while (ParentClass.TradersShouldBeScared) {
                Game->NewScriptFrame();
                DW_T;
            }
            DW_T;
            seh_me.Release();
            Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
            Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
            while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) {
                Game->NewScriptFrame();
                DW_T;
            }
            DW_T;
            DW_SAY("_KILLED_EARTH_TROLL")
        }
        if (!InSafeZone && Game->IsRegionLoaded("Darkwood4")) {
            DW_T;
            Game->EntitySetAsScared(Me, false);
            InSafeZone = true;
        } else if (InSafeZone && !Game->IsRegionLoaded("Darkwood4")) {
            DW_T;
            Game->EntitySetAsScared(Me, true);
            InSafeZone = false;
        }
        if (!GreetedBuddy && Game->IsRegionLoaded("Darkwood4")) {
            DW_T;
            CScriptThing nearest = Game->GetNearestWithScriptName(Game->GetHero(), "DarkwoodTrader");
            ParentClass.TraderToTalk = nearest.GetDataString();
            CScriptThing camp_trader = Game->GetThingWithScriptName("TE_CampTrader_A");
            if (camp_trader.IsAlive()) {
                DW_T;
                Game->EntityStopFollowing(Me);
                Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
                DW_START_SCRIPTING(Me, seh_me);
                DW_MOVE_TO_POSITION(camp_trader, 3.0f, ENTITY_MOVE_WALK)
                DW_T;
                CScriptThing heal_effect = Game->CreateEffect("HEAL_LEVEL2", Me.GetPos(), "", 0.0f, false, false);
                Game->ModifyThingHealth(Me, 1000.0f, false);
                if (ParentClass.TraderToTalk == Me.GetDataString()) {
                    DW_T;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, camp_trader);
                    Game->AddLineToConversation(conversation, DW_LINE("_GREET_ALLY_10"), false, Me, camp_trader);
                    Game->AddLineToConversation(conversation, DW_LINE("_GREET_ALLY_RESPONSE"), false, camp_trader, Me);
                    Game->AddLineToConversation(conversation, DW_LINE("_GREET_ALLY_20"), false, Me, camp_trader);
                    Game->Pause(2.0f);
                    Game->RemoveThing(heal_effect, false, true);
                    while (Game->IsConversationActive(conversation)) {
                        Game->NewScriptFrame();
                        DW_T;
                    }
                    DW_T;
                } else {
                    DW_T;
                    seh_me.PlayAnimation("STANDARD_WAVE", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        DW_T;
                    }
                    DW_T;
                    Game->Pause(2.0f);
                }
                GreetedBuddy = true;
                if (heal_effect.IsAlive()) {
                    DW_T;
                    Game->RemoveThing(heal_effect, false, true);
                }
                seh_me.Release();
                Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
                Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
            }
        }
        if (ParentClass.BalverinesToSurpriseHeroNeeded > 0 && !InSafeZone) {
            DW_T;
            Game->EntitySetAsScared(Me, true);
        }
        if (!Infected || !ParentClass.SavedInMiddle || Game->IsRegionLoaded("Darkwood4") || Game->IsRegionLoaded("BarrowFields")) {
            DW_T;
            IncubationTime = pGlobalScriptDef->TE_TraderToBalverineDelay;
        }
        if (IncubationTime != 0) {
            if (IncubationTime < ParentClass.IncubationTimeHigh && Infected < INFECTED_HIGH) {
                DW_T;
                if (ParentClass.MakeTraderComment("INFECTED_HIGH", Me, TRADER_COMMENT_SINGLE)) {
                    DW_T;
                    Infected = INFECTED_HIGH;
                }
            } else if (IncubationTime < ParentClass.IncubationTimeMedium && Infected < INFECTED_MEDIUM) {
                DW_T;
                if (ParentClass.MakeTraderComment("INFECTED_MEDIUM", Me, TRADER_COMMENT_SINGLE)) {
                    DW_T;
                    Infected = INFECTED_MEDIUM;
                }
            } else if (IncubationTime < ParentClass.IncubationTimeLow && Infected < INFECTED_LOW) {
                DW_T;
                if (ParentClass.MakeTraderComment("INFECTED_LOW", Me, TRADER_COMMENT_SINGLE)) {
                    DW_T;
                    Infected = INFECTED_LOW;
                }
            }
        } else {
            DW_T;
            ParentClass.TradersStillAliveCounter--;
            seh_me.Release();
            ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_TraderEscortScript, CScriptThing>(CCharString("ParentClass.") + CCharString("TurnToBalv"), 0, &ParentClass, &CQ_TraderEscortScript::TurnToBalv, Me), CCharString(""));
            while (true) {
                Game->NewScriptFrame();
                DW_T;
            }
        }
        if ((Me.MsgIsHitBy("") || Me.MsgIsHitByAnySpecialAbilityFrom("")) && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")) {
            DW_T;
            RegulateFollowStateComment = 5;
            if (Game->GetHealth(Me) > 5.0f && !Me.MsgIsHitBy("SCRIPT_NAME_HERO") && (!Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") || Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                DW_T;
                ParentClass.MakeTraderComment("UNDER_ATTACK", Me, TRADER_COMMENT_SINGLE);
                ParentClass.CommentTimer = 3;
            } else if (Game->GetHealth(Me) > 5.0f && (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
                DW_T;
                ParentClass.MakeTraderComment("HERO_HIT_ME", Me, TRADER_COMMENT_SINGLE);
                ParentClass.CommentTimer = 2;
            }
        }
        if (CurrentAIState != PreviousAIState) {
            DW_T;
            if (RegulateFollowStateComment == 0 && CurrentAIState == TRADER_STATE_FOLLOWING) {
                DW_T;
                ParentClass.MakeTraderComment("FOLLOWING", Me, TRADER_COMMENT_SINGLE);
                RegulateFollowStateComment = 10;
            }
        } else if (RegulateBanterComment == 0 && !LeadToCamp) {
            DW_T;
            bool bantered;
            if (ParentClass.TradersStillAliveCounter == 1) {
                DW_T;
                bantered = ParentClass.MakeTraderComment("BANTER_SOLO", Me, TRADER_COMMENT_SINGLE);
            } else {
                DW_T;
                switch (ParentClass.TimesBantered) {
                case 0:
                    bantered = ParentClass.MakeTraderComment("BANTER_FIRST", Me, TRADER_COMMENT_MULTIPLE);
                    break;
                case 1:
                    bantered = ParentClass.MakeTraderComment("BANTER_SECOND", Me, TRADER_COMMENT_MULTIPLE);
                    break;
                case 2:
                    bantered = ParentClass.MakeTraderComment("BANTER_THIRD", Me, TRADER_COMMENT_MULTIPLE);
                    break;
                case 3:
                    bantered = ParentClass.MakeTraderComment("BANTER_FOURTH", Me, TRADER_COMMENT_MULTIPLE);
                    break;
                default:
                    bantered = ParentClass.MakeTraderComment("BANTER", Me, TRADER_COMMENT_SINGLE);
                    break;
                }
            }
            if (bantered) {
                DW_T;
                RegulateBanterComment = rand() % 20 + 30;
                ParentClass.TimesBantered++;
            } else {
                DW_T;
                RegulateBanterComment = rand() % 15 + 25;
            }
        }
        PreviousAIState = CurrentAIState;
        CurrentAIState = Me.GetCurrentStateGroupType();
    }
}