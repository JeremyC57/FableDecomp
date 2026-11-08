// NScript::CQ_BreakSiegeScript::CDisplacedTrader::Main() -- the displaced trader during the siege: live while alive, take scripted control; each frame: when talked to retake control and say TEXT_QST_018_TRADER_INTRO / _INTRO_REP / _ON_TALK_AGAIN (by TimesSpokenTo) under a paused speech movie; when hit by the hero (not a heal-life spell) panic, run to BS_RunOffMarker and fade out; when idle mutter TEXT_QST_018_TRADER_MUTTERS every 30 s and pace between BS_ImpatientMarker1 and BS_ImpatientMarker2
// Inline-split: one do { } while (0) around the TimesSpokenTo SAY switch in the talked-to branch (the c2 frequency lever of the landed CEndGameScythe::Main 0x00D2ED20: the speech-wait terminate returns become inner-loop exits, so VC7.1 expands the implicit MovieBase dtor once and tail-merges both pads into it, as retail); retail's construct is unproven
// Leaf: ?Main@CDisplacedTrader@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1FD30, 2255 bytes, thiscall, plain `ret` (one epilogue).
// Identity (proven): slot 1 (Main) of the CDisplacedTrader entity vtable stored by its landed Alloc (GetParentScript 0x00D1FCD0,
//   OnPersist 0x00D21B10 landed); Ego_r.pdb ?Main@CDisplacedTrader@CQ_BreakSiegeScript@NScript@@EAEXXZ.
// Callees: CIsThingAlive (vtable 0x012C32F0; ctor and dtor inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / ~ 0x0099A430, ScriptedThingBase IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / dtor 0x007E74D0 / GetScriptThing 0x007E7490 / MoveToPosition 0x007E72F0 / MoveToThing 0x007E7300 / Speak
//   0x007E7390 / IsPerformingScriptTask 0x007E7450, the MovieBase PImp dtor CCountedPointer<MovieBase>::~ 0x006E7AB0 (movie ctor and dtor
//   expanded), CCountedPointer<CScriptThing>::operator= 0x00704580 (the implicit CScriptThing::operator=), CScriptThing::NullCoord
//   0x0143E8E0 (GetPos expanded), CScriptThing::operator== expanded over slot 78 IsEqualTo, CTimer GetTime / operator= expanded over
//   pDSTGame 0x0143E8F8, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsDistanceFromThingToPositionOver 0x00CBE45C, FadeOutAndKillEntity
//   0x00CBE716 (__fastcall; both PDBs ?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z); CGameScriptInterfaceBase slots 7, 8, 70 GetHero,
//   72 GetThingWithScriptName, 264 GetHealth, 364 / 365 / 366 conversations, 368 IsConversationActive, 370 StartMovieSequence, 379
//   PauseAllNonScriptedEntities, 496 EntitySetAsScared.
// Layout: engine/CQ_BreakSiegeScript.h CDisplacedTrader (Ego_r.pdb, 36 bytes = retail new size): ParentClass +0x14, PMasterData +0x18,
//   TimesSpokenTo +0x1c, RandomMutteringsDelay (CTimer) +0x20.
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptThing;
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);   // 0x00704580
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);            // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;                                                                   // 0x0143E8F8
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_BreakSiegeScript::CDisplacedTrader::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing target = Game->GetThingWithScriptName("BS_ImpatientMarker2");
    CScriptThing marker1 = Game->GetThingWithScriptName("BS_ImpatientMarker1");
    CScriptThing marker2 = Game->GetThingWithScriptName("BS_ImpatientMarker2");
    long conversation = 0;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            do {
                switch (TimesSpokenTo) {
                case 0:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_018_TRADER_INTRO", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    break;
                case 1:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_018_TRADER_INTRO_REP", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    break;
                default:
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_018_TRADER_ON_TALK_AGAIN", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    break;
                }
            } while (0);
            ++TimesSpokenTo;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAsScared(Me, true);
            CScriptThing run_off = Game->GetThingWithScriptName("BS_RunOffMarker");
            C3DVector pos = run_off.GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                seh_me.MoveToPosition(pos, 0.0f, (EScriptEntityMoveType)1, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
        if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            if (RandomMutteringsDelay.GetTime() == 0 && !Game->IsConversationActive(conversation)) {
                if (IsActiveThreadTerminating())
                    return;
                conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_018_TRADER_MUTTERS", false, Me, Game->GetHero());
                RandomMutteringsDelay = 30;
            }
            seh_me.MoveToThing(target, 1.0f, (EScriptEntityMoveType)0, false, true, false, true);
            if (target == marker1) {
                if (IsActiveThreadTerminating())
                    return;
                target = marker2;
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                target = marker1;
            }
        }
        Game->NewScriptFrame();
    }
}